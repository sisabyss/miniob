/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#ifndef UPDATE_LOGICAL_OPERATOR_H_
#define UPDATE_LOGICAL_OPERATOR_H_

#include "sql/operator/logical_operator.h"

/**
 * @brief 更新逻辑算子
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, Value value, Field field);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

  Table                    *table() const { return table_; }
  const Value &value() const { return value_; }
  Value       &value() { return value_; }
  const Field &field() const { return field_; }
  Field       &field() { return field_; }

private:
  Table  *table_ = nullptr;
  Value   value_;
  Field   field_;
};

#endif // UPDATE_LOGICAL_OPERATOR_H_
