/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/07/05.
//

#include "sql/expr/expression.h"
#include <memory>
#include "common/like.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "common/lang/defer.h"
#include "common/type/attr_type.h"
#include "sql/optimizer/logical_plan_generator.h"
#include "sql/parser/parse_defs.h"
#include "sql/expr/tuple.h"
#include "sql/expr/arithmetic_operator.hpp"
#include "sql/operator/logical_operator.h"
#include "sql/operator/physical_operator.h"
#include "sql/optimizer/logical_plan_generator.h"
#include "sql/optimizer/physical_plan_generator.h"
#include "sql/stmt/select_stmt.h"

using namespace std;

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(table_name(), field_name()), value);
}

bool FieldExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::FIELD) {
    return false;
  }
  const auto &other_field_expr = static_cast<const FieldExpr &>(other);
  return table_name() == other_field_expr.table_name() && field_name() == other_field_expr.field_name();
}

// TODO: 在进行表达式计算时，`chunk` 包含了所有列，因此可以通过 `field_id` 获取到对应列。
// 后续可以优化成在 `FieldExpr` 中存储 `chunk` 中某列的位置信息。
RC FieldExpr::get_column(Chunk &chunk, Column &column)
{
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    column.reference(chunk.column(field().meta()->field_id()));
  }
  return RC::SUCCESS;
}

bool ValueExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::VALUE) {
    return false;
  }
  const auto &other_value_expr = static_cast<const ValueExpr &>(other);
  return value_.compare(other_value_expr.get_value()) == 0;
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value) const
{
  value = value_;
  return RC::SUCCESS;
}

RC ValueExpr::get_column(Chunk &chunk, Column &column)
{
  column.init(value_);
  return RC::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
CastExpr::CastExpr(unique_ptr<Expression> child, AttrType cast_type) : child_(std::move(child)), cast_type_(cast_type)
{}

CastExpr::~CastExpr() {}

RC CastExpr::cast(const Value &value, Value &cast_value) const
{
  RC rc = RC::SUCCESS;
  if (this->value_type() == value.attr_type()) {
    cast_value = value;
    return rc;
  }
  rc = Value::cast_to(value, cast_type_, cast_value);
  return rc;
}

RC CastExpr::get_value(const Tuple &tuple, Value &result) const
{
  Value value;
  RC rc = child_->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

RC CastExpr::try_get_value(Value &result) const
{
  Value value;
  RC rc = child_->try_get_value(value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

////////////////////////////////////////////////////////////////////////////////

ComparisonExpr::ComparisonExpr(CompOp comp, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : comp_(comp), left_(std::move(left)), right_(std::move(right))
{}

ComparisonExpr::~ComparisonExpr() {}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const
{
  RC  rc         = RC::SUCCESS;

  // LIKE and NOT LIKE Expression
  if (comp() == LIKE_OP || comp() == NO_LIKE_OP) {
    auto left_type = left.attr_type();
    auto right_type = right.attr_type();

    // Check if both operands are of CHARS type
    if (left_type != AttrType::CHARS || right_type != AttrType::CHARS) {
        LOG_WARN("LIKE and NOT LIKE operations require both operands to be of CHARS type. "
                  "Received left operand type: %s, right operand type: %s.",
                  attr_type_to_string(left_type),
                  attr_type_to_string(right_type));
        return RC::INVALID_ARGUMENT;
    }

    // Evaluate LIKE/NOT LIKE expression
    result = is_like(left.get_string().data(), right.get_string().data());
    if (comp() == NO_LIKE_OP) {
        result = !result; // Negate result for NOT LIKE
    }

    return rc;
  }

  if (comp_ == IS_NULL_OP || comp_ == NOT_NULL_OP) {
    if (right.attr_type() != AttrType::NULLS)
      LOG_ERROR("IS(NOT) expression only support right value is `NULL`, but get %s", right.to_string().data());

    result = (left.attr_type() == AttrType::NULLS);
    if (comp_ == NOT_NULL_OP) result = !result;

    LOG_TRACE("ComparisionExpr::compare: %s %s", comp_ == IS_NULL_OP ? "IS" : "IS NOT", result ? "TRUE" : "FALSE");

    return rc;
  }

  /* For vanilla comparision with NULL return false */
  if (left.attr_type() == AttrType::NULLS || right.attr_type() == AttrType::NULLS) {
    result = false;
    return RC::SUCCESS;
  }

  int cmp_result = left.compare(right);
  result         = false;
  switch (comp_) {
    case EQUAL_TO: {
      result = (0 == cmp_result);
    } break;
    case LESS_EQUAL: {
      result = (cmp_result <= 0);
    } break;
    case NOT_EQUAL: {
      result = (cmp_result != 0);
    } break;
    case LESS_THAN: {
      result = (cmp_result < 0);
    } break;
    case GREAT_EQUAL: {
      result = (cmp_result >= 0);
    } break;
    case GREAT_THAN: {
      result = (cmp_result > 0);
    } break;
    default: {
      LOG_WARN("unsupported comparison. %d", comp_);
      rc = RC::INTERNAL;
    } break;
  }

  return rc;
}

RC ComparisonExpr::try_get_value(Value &cell) const
{
  if (left_->type() == ExprType::VALUE && right_->type() == ExprType::VALUE) {
    ValueExpr *  left_value_expr  = static_cast<ValueExpr *>(left_.get());
    ValueExpr *  right_value_expr = static_cast<ValueExpr *>(right_.get());
    const Value &left_cell        = left_value_expr->get_value();
    const Value &right_cell       = right_value_expr->get_value();

    bool value = false;
    RC   rc    = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const
{
  Value left_value;
  Value right_value;

  // FIXME: nullptr
  left_->open(nullptr);
  RC rc = left_->get_value(tuple, left_value);
  if (left_->is_multi_valued(tuple)) {
    LOG_INFO("left-hand expression only support single value");
    return RC::INVALID_ARGUMENT;
  }
  left_->close();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  // Handle IN_OP and NO_IN_OP
  if (comp_ == IN_OP || comp_ == NO_IN_OP) {
    if (right_->type() != ExprType::SUBQUERY) {
      LOG_WARN("IN compop only support right-hand subquery");
      return RC::INVALID_ARGUMENT;
    }

    // NULL (NOT)IN SUBQUERY
    if (left_value.attr_type() == AttrType::NULLS) {
      value.set_boolean(false);
      return RC::SUCCESS;
    }

    // loop for right-hand sub-query
    bool has_same = false, has_null = false;
    // FIXME: only open null pointer
    right_->open(nullptr);
    while (OB_SUCC(rc = right_->get_value(tuple, right_value))) {
      if (right_value.attr_type() == AttrType::NULLS) {
        has_null = false;
      } else if (left_value.compare(right_value) == 0) {
        has_same = true;
      }
    }
    right_->close();
    value.set_boolean(comp_ == IN_OP ? has_same : (has_null ? false : !has_same));
    return rc == RC::RECORD_EOF ? RC::SUCCESS : rc;
  }

  // handle XST_OP and NO_XST_OP
  if (comp_ == XST_OP || comp_ == NO_XST_OP) {
    if (right_->type() != ExprType::SUBQUERY) {
      LOG_WARN("EXISTS compop only support right-hand subquery");
      return RC::INVALID_ARGUMENT;
    }

    // FIXME: same with former FIXME
    right_->open(nullptr);
    rc = right_->get_value(tuple, right_value);
    right_->close();

    value.set_boolean(comp_ == XST_OP ? rc == RC::SUCCESS : rc == RC::RECORD_EOF);
    return RC::RECORD_EOF == rc ? RC::SUCCESS : rc;
  }

  // FIXME: nullptr
  right_->open(nullptr);
  rc = right_->get_value(tuple, right_value);
  // FIXME: remove this branch
  if (right_->is_multi_valued(tuple)) {
    LOG_INFO("right-hand expression only support single value except IN/EXISTS comp");
    return RC::INVALID_ARGUMENT;
  }
  right_->close();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }

  bool bool_value = false;

  rc = compare_value(left_value, right_value, bool_value);
  if (rc == RC::SUCCESS) {
    value.set_boolean(bool_value);
  }
  return rc;
}

RC ComparisonExpr::eval(Chunk &chunk, std::vector<uint8_t> &select)
{
  RC     rc = RC::SUCCESS;
  Column left_column;
  Column right_column;

  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  if (left_column.attr_type() != right_column.attr_type()) {
    LOG_WARN("cannot compare columns with different types");
    return RC::INTERNAL;
  }
  if (left_column.attr_type() == AttrType::INTS) {
    rc = compare_column<int>(left_column, right_column, select);
  } else if (left_column.attr_type() == AttrType::FLOATS) {
    rc = compare_column<float>(left_column, right_column, select);
  } else {
    // TODO: support string compare
    LOG_WARN("unsupported data type %d", left_column.attr_type());
    return RC::INTERNAL;
  }
  return rc;
}

template <typename T>
RC ComparisonExpr::compare_column(const Column &left, const Column &right, std::vector<uint8_t> &result) const
{
  RC rc = RC::SUCCESS;

  bool left_const  = left.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    compare_result<T, true, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else if (left_const && !right_const) {
    compare_result<T, true, false>((T *)left.data(), (T *)right.data(), right.count(), result, comp_);
  } else if (!left_const && right_const) {
    compare_result<T, false, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else {
    compare_result<T, false, false>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(Type type, vector<unique_ptr<Expression>> &children)
    : conjunction_type_(type), children_(std::move(children))
{}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    value.set_boolean(true);
    return rc;
  }

  Value tmp_value;
  for (const unique_ptr<Expression> &expr : children_) {
    rc = expr->get_value(tuple, tmp_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value by child expression. rc=%s", strrc(rc));
      return rc;
    }
    bool bool_value = tmp_value.get_boolean();
    if ((conjunction_type_ == Type::AND && !bool_value) || (conjunction_type_ == Type::OR && bool_value)) {
      value.set_boolean(bool_value);
      return rc;
    }
  }

  bool default_value = (conjunction_type_ == Type::AND);
  value.set_boolean(default_value);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////

ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, Expression *left, Expression *right)
    : arithmetic_type_(type), left_(left), right_(right)
{}
ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right))
{}

bool ArithmeticExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (type() != other.type()) {
    return false;
  }
  auto &other_arith_expr = static_cast<const ArithmeticExpr &>(other);
  return arithmetic_type_ == other_arith_expr.arithmetic_type() && left_->equal(*other_arith_expr.left_) &&
         right_->equal(*other_arith_expr.right_);
}
AttrType ArithmeticExpr::value_type() const
{
  if (!right_) {
    return left_->value_type();
  }

  if (left_->value_type() == AttrType::INTS && right_->value_type() == AttrType::INTS &&
      arithmetic_type_ != Type::DIV) {
    return AttrType::INTS;
  }

  return AttrType::FLOATS;
}

RC ArithmeticExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const
{
  RC rc = RC::SUCCESS;

  // 表达式一方存在 null，结果为null
  if(left_value.attr_type() == AttrType::NULLS || right_value.attr_type() == AttrType::NULLS){
    value.set_type(AttrType::NULLS);
    return RC::SUCCESS;
  }

  const AttrType target_type = value_type();
  value.set_type(target_type);

  switch (arithmetic_type_) {
    case Type::ADD: {
      Value::add(left_value, right_value, value);
    } break;

    case Type::SUB: {
      Value::subtract(left_value, right_value, value);
    } break;

    case Type::MUL: {
      Value::multiply(left_value, right_value, value);
    } break;

    case Type::DIV: {
      Value::divide(left_value, right_value, value);
    } break;

    case Type::NEGATIVE: {
      Value::negative(left_value, value);
    } break;

    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported arithmetic type. %d", arithmetic_type_);
    } break;
  }
  return rc;
}

template <bool LEFT_CONSTANT, bool RIGHT_CONSTANT>
RC ArithmeticExpr::execute_calc(
    const Column &left, const Column &right, Column &result, Type type, AttrType attr_type) const
{
  RC rc = RC::SUCCESS;
  switch (type) {
    case Type::ADD: {
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, AddOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, AddOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
    } break;
    case Type::SUB:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, SubtractOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, SubtractOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::MUL:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, MultiplyOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, MultiplyOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::DIV:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, DivideOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, DivideOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::NEGATIVE:
      if (attr_type == AttrType::INTS) {
        unary_operator<LEFT_CONSTANT, int, NegateOperator>((int *)left.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        unary_operator<LEFT_CONSTANT, float, NegateOperator>(
            (float *)left.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    default: rc = RC::UNIMPLEMENTED; break;
  }
  if (rc == RC::SUCCESS) {
    result.set_count(result.capacity());
  }
  return rc;
}

RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_value(left_value, right_value, value);
}

RC ArithmeticExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
    return rc;
  }
  Column left_column;
  Column right_column;

  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_column(left_column, right_column, column);
}

RC ArithmeticExpr::calc_column(const Column &left_column, const Column &right_column, Column &column) const
{
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();
  column.init(target_type, left_column.attr_len(), std::max(left_column.count(), right_column.count()));
  bool left_const  = left_column.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right_column.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    column.set_column_type(Column::Type::CONSTANT_COLUMN);
    rc = execute_calc<true, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (left_const && !right_const) {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<true, false>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (!left_const && right_const) {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, false>(left_column, right_column, column, arithmetic_type_, target_type);
  }
  return rc;
}

RC ArithmeticExpr::try_get_value(Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}

////////////////////////////////////////////////////////////////////////////////

UnboundAggregateExpr::UnboundAggregateExpr(const char *aggregate_name, Expression *child)
    : aggregate_name_(aggregate_name), child_(child)
{}

////////////////////////////////////////////////////////////////////////////////
AggregateExpr::AggregateExpr(Type type, Expression *child) : aggregate_type_(type), child_(child) {}

AggregateExpr::AggregateExpr(Type type, unique_ptr<Expression> child) : aggregate_type_(type), child_(std::move(child))
{}

RC AggregateExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    rc = RC::INTERNAL;
  }
  return rc;
}

bool AggregateExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != type()) {
    return false;
  }
  const AggregateExpr &other_aggr_expr = static_cast<const AggregateExpr &>(other);
  return aggregate_type_ == other_aggr_expr.aggregate_type() && child_->equal(*other_aggr_expr.child());
}

unique_ptr<Aggregator> AggregateExpr::create_aggregator() const
{
  unique_ptr<Aggregator> aggregator;
  switch (aggregate_type_) {
    case Type::SUM: {
      aggregator = make_unique<SumAggregator>();
      break;
    }
    case Type::MAX: {
      aggregator = make_unique<MaxAggregator>();
      break;
    }
    case Type::MIN: {
      aggregator = make_unique<MinAggregator>();
      break;
    }
    case Type::AVG: {
      aggregator = make_unique<AvgAggregator>();
      break;
    }
    case Type::COUNT: {
      aggregator = make_unique<CountAggregator>();
      break;
    }
    default: {
      ASSERT(false, "unsupported aggregate type");
      break;
    }
  }
  return aggregator;
}

RC AggregateExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(name()), value);
}

RC AggregateExpr::type_from_string(const char *type_str, AggregateExpr::Type &type)
{
  RC rc = RC::SUCCESS;
  if (0 == strcasecmp(type_str, "count")) {
    type = Type::COUNT;
  } else if (0 == strcasecmp(type_str, "sum")) {
    type = Type::SUM;
  } else if (0 == strcasecmp(type_str, "avg")) {
    type = Type::AVG;
  } else if (0 == strcasecmp(type_str, "max")) {
    type = Type::MAX;
  } else if (0 == strcasecmp(type_str, "min")) {
    type = Type::MIN;
  } else {
    rc = RC::INVALID_ARGUMENT;
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
SubQueryExpr::SubQueryExpr(SelectSqlNode &&sql_node)
  : sql_node_(std::make_unique<SelectSqlNode>(std::move(sql_node)))
{}

RC SubQueryExpr::open(Trx* trx)
{
  return physical_oper_ ? physical_oper_->open(trx) : RC::INTERNAL;
}

RC SubQueryExpr::close()
{
  return physical_oper_ ? physical_oper_->close() : RC::SUCCESS;
}

bool SubQueryExpr::is_multi_valued(const Tuple& tuple) const
{
  return physical_oper_ && physical_oper_->next() != RC::RECORD_EOF;
}

RC SubQueryExpr::get_value(const Tuple& tuple, Value& value) const
{
  if (!physical_oper_) {
    return RC::INTERNAL;
  }
  if (RC rc = physical_oper_->next(); rc != RC::SUCCESS) {
    return rc;
  }
  return physical_oper_->current_tuple()->cell_at(0, value);
}

ExprType SubQueryExpr::type() const
{
  return ExprType::SUBQUERY;
}

AttrType SubQueryExpr::value_type() const
{
  return value_type_;
}

RC SubQueryExpr::build(const Db* db)
{
  try {
    if (RC rc = build_stmt(db); rc != RC::SUCCESS) {
      LOG_WARN("Failed to build subquery stmt: %s", strrc(rc));
      return rc;
    }
    if (RC rc = build_logical_oper(); rc != RC::SUCCESS) {
      LOG_WARN("Failed to build subquery logical operator: %s", strrc(rc));
      return rc;
    }
    if (RC rc = build_physical_oper(); rc != RC::SUCCESS) {
      LOG_WARN("Failed to build subquery physical operator: %s", strrc(rc));
      return rc;
    }
    return RC::SUCCESS;
  } catch (const std::exception& e) {
    LOG_WARN("Exception during subquery build: %s", e.what());
    return RC::INTERNAL;
  }
}

RC SubQueryExpr::build_stmt(const Db *db)
{
  Stmt * stmt = nullptr;
  if (RC rc = SelectStmt::create(db, *sql_node_.get(), stmt); OB_FAIL(rc)) {
    LOG_WARN("Failed to generate subquery select stmt: %s", strrc(rc));
    return rc;
  }

  if (stmt->type() != StmtType::SELECT) {
    LOG_WARN("Subquery stmt must be SELECT");
    if (stmt != nullptr) {
      delete stmt;
    }
    return RC::INVALID_ARGUMENT;
  }

  SelectStmt* select_stmt = static_cast<SelectStmt*>(stmt);
  if (select_stmt->query_expressions().size() > 1) {
    LOG_WARN("Subquery expression must have one column.");
    if (stmt != nullptr) {
      delete stmt;
    }
    return RC::INVALID_ARGUMENT;
  }

  // set value type
  value_type_ = select_stmt->query_expressions().front()->value_type();

  stmt_ = std::unique_ptr<SelectStmt>(select_stmt);
  return RC::SUCCESS;
}

RC SubQueryExpr::build_logical_oper()
{
  LogicalPlanGenerator plan_generator;
  if (RC rc = plan_generator.create(stmt_.get(), logical_oper_); OB_FAIL(rc)) {
    LOG_WARN("Failed to generate logical operator: %s", strrc(rc));
    return rc;
  }
  return RC::SUCCESS;
}

RC SubQueryExpr::build_physical_oper()
{
  PhysicalPlanGenerator plan_generator;
  if (RC rc = plan_generator.create(*logical_oper_, physical_oper_); OB_FAIL(rc)) {
    LOG_WARN("Failed to generate physical operator: %s", strrc(rc));
    return rc;
  }

  // FIXME: post-build check
  // 1. for star expression, check the tuple size > 1;
  // 2. ...
  /*
  physical_oper_->open(nullptr);
  if (!OB_SUCC(physical_oper_->next())) {
    LOG_INFO("post build physical oper check failed.");
    Tuple *tuple = physical_oper_->current_tuple();
    if (tuple->cell_num() > 1) {
      LOG_WARN("subquery expression should only have one column. but get [%s]", tuple->to_string().data());
      return RC::INVALID_ARGUMENT;
    }
  }
  physical_oper_->close();
  */

  return RC::SUCCESS;
}
