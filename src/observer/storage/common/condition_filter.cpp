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
// Created by Wangyunlai on 2021/5/7.
//

#include "condition_filter.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/parser/expression_binder.h"
#include "sql/parser/parse_defs.h"
#include "storage/record/record_manager.h"
#include "storage/table/table.h"
#include <math.h>
#include <memory>
#include <stddef.h>

using namespace common;

ConditionFilter::~ConditionFilter() {}

DefaultConditionFilter::~DefaultConditionFilter() = default;

RC DefaultConditionFilter::init(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right, CompOp comp_op)
{
  left_expr_  = std::move(left);
  right_expr_ = std::move(right);
  comp_op_    = comp_op;

  cmp_expr_ = std::make_unique<ComparisonExpr>(comp_op_, std::move(left_expr_), std::move(right_expr_));
  return RC::SUCCESS;
}

static int implicit_cast_cost(AttrType from, AttrType to)
{
  if (from == to) {
    return 0;
  }
  return DataType::type_instance(from)->cast_cost(to);
}

RC DefaultConditionFilter::init(const ConditionSqlNode &condition)
{
  RC rc = RC::SUCCESS;

  std::unique_ptr<Expression> left_bound_expr, right_bound_expr;
  std::unique_ptr<Expression> left_unbound_expr(condition.left_expr);
  std::unique_ptr<Expression> right_unbound_expr(condition.right_expr);

  // bind left expr
  {
    vector<unique_ptr<Expression>> bound_expressions;
    rc = expr_binder_.bind_expression(left_unbound_expr, bound_expressions);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to bind left hand expression, rc:%s", strrc(rc));
      return rc;
    }
    left_bound_expr = std::move(bound_expressions.front());
  }

  // bind right expr
  {
    vector<unique_ptr<Expression>> bound_expressions;
    rc = expr_binder_.bind_expression(right_unbound_expr, bound_expressions);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to bind left hand expression, rc:%s", strrc(rc));
      return rc;
    }
    right_bound_expr = std::move(bound_expressions.front());
  }

  // 校验和转换
  if (left_bound_expr->value_type() != right_bound_expr->value_type()
      /* Okay for null comparasion */
      && (left_bound_expr->value_type() != AttrType::NULLS && right_bound_expr->value_type() != AttrType::NULLS)
      /* Okay for subquery comparasion */
      && (condition.comp != IN_OP  && condition.comp != NO_IN_OP && condition.comp != XST_OP && condition.comp != NO_XST_OP)
    ) {
    auto left_to_right_cost = implicit_cast_cost(left_bound_expr->value_type(), right_bound_expr->value_type());
    auto right_to_left_cost = implicit_cast_cost(right_bound_expr->value_type(), left_bound_expr->value_type());
    if (left_to_right_cost <= right_to_left_cost && left_to_right_cost != INT32_MAX) {
      ExprType left_type = left_bound_expr->type();
      auto cast_expr = make_unique<CastExpr>(std::move(left_bound_expr), right_bound_expr->value_type());
      if (left_type == ExprType::VALUE) {
        Value left_val;
        if (OB_FAIL(rc = cast_expr->try_get_value(left_val)))
        {
          LOG_WARN("failed to get value from left child", strrc(rc));
          return rc;
        }
        left_bound_expr = make_unique<ValueExpr>(left_val);
      } else {
        left_bound_expr = std::move(cast_expr);
      }
    } else if (right_to_left_cost < left_to_right_cost && right_to_left_cost != INT32_MAX) {
      ExprType right_type = right_bound_expr->type();
      auto cast_expr = make_unique<CastExpr>(std::move(right_bound_expr), left_bound_expr->value_type());
      if (right_type == ExprType::VALUE) {
        Value right_val;
        if (OB_FAIL(rc = cast_expr->try_get_value(right_val)))
        {
          LOG_WARN("failed to get value from right child", strrc(rc));
          return rc;
        }
        right_bound_expr = make_unique<ValueExpr>(right_val);
      } else {
        right_bound_expr = std::move(cast_expr);
      }

    } else {
      rc = RC::UNSUPPORTED;
      LOG_WARN("unsupported cast from %s to %s", attr_type_to_string(left_bound_expr->value_type()), attr_type_to_string(right_bound_expr->value_type()));
      return rc;
    }
  }

  return init(std::move(left_bound_expr), std::move(right_bound_expr), condition.comp);
}

RC DefaultConditionFilter::filter(Tuple *tuple, bool &res) const
{
  RC rc = RC::SUCCESS;

  Value value;
  rc = cmp_expr_->get_value(*tuple, value);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to get value from comparision expression when filtering, rc:%s", strrc(rc));
    return rc;
  }

  res = value.get_boolean();
  return rc;
}

CompositeConditionFilter::~CompositeConditionFilter() = default;

RC CompositeConditionFilter::add_filter(std::unique_ptr<ConditionFilter> &&filter) {
  filters_.emplace_back(std::move(filter));
  return RC::SUCCESS;
}

RC CompositeConditionFilter::add_filter(ExpressionBinder const &binder, const ConditionSqlNode &condition) {
  RC rc = RC::SUCCESS;
  auto new_filter = std::make_unique<DefaultConditionFilter>(binder);
  rc = new_filter->init(condition);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to add filter: rc:%s", strrc(rc));
    return rc;
  }
  add_filter(std::move(new_filter));
  return RC::SUCCESS;
}

RC CompositeConditionFilter::filter(Tuple *tuple, bool &res) const
{
  RC rc = RC::SUCCESS;
  for (auto const &filter : filters_) {
    rc = filter->filter(tuple, res);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to filter in composite condition filter, rs:%s", strrc(rc));
      return rc;
    }
    if (!res) return rc;
  }
  return rc;
}
