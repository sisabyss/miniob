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
// Created by WangYunlai on 2022/07/01.
//

#include "sql/operator/project_physical_operator.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include <unordered_set>

using namespace std;

ProjectPhysicalOperator::ProjectPhysicalOperator(vector<unique_ptr<Expression>> &&expressions)
  : expressions_(std::move(expressions)), tuple_(expressions_)
{
}

RC ProjectPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  PhysicalOperator *child = children_[0].get();
  RC                rc    = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  return RC::SUCCESS;
}

RC ProjectPhysicalOperator::next()
{
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }
  return children_[0]->next();
}

RC ProjectPhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}
Tuple *ProjectPhysicalOperator::current_tuple()
{
  tuple_.set_tuple(children_[0]->current_tuple());
  return &tuple_;
}

RC ProjectPhysicalOperator::tuple_schema(TupleSchema &schema) const
{
  std::unordered_set<std::string_view> mult_table;

  // Helper lambda to cast and check if the expression is a FieldExpr
  auto get_field_expr = [](const std::unique_ptr<Expression> &expr) -> FieldExpr* {
    return expr->type() == ExprType::FIELD ? dynamic_cast<FieldExpr*>(expr.get()) : nullptr;
  };

  // First pass: Collect table names
  for (const auto &expression : expressions_) {
    if (FieldExpr* fieldPtr = get_field_expr(expression)) {
      LOG_TRACE("ProjectPhysicalOperator::tuple_schema: %s.%s",
               fieldPtr->table_name(), fieldPtr->field_name());
      mult_table.insert(fieldPtr->table_name());
    }
  }

  bool is_mult_table = mult_table.size() > 1;

  // Second pass: Construct schema based on table count
  for (const auto &expression : expressions_) {
    if (FieldExpr* fieldPtr = get_field_expr(expression)) {
      if (is_mult_table) {
        // Avoid extra allocation by using string_view
        std::string name = std::string(fieldPtr->table_name()) + "." + fieldPtr->field_name();
        schema.append_cell(name.data());
      } else {
        schema.append_cell(fieldPtr->field_name());
      }
    } else {
      schema.append_cell(expression->name());
    }
  }

  return RC::SUCCESS;
}
