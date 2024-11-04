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
  UpdateLogicalOperator(
    Table *table,
    std::vector<std::unique_ptr<Expression>> &&expr_list,
    std::vector<FieldMeta> &&field_list
  );
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

  Table                    *table() const { return table_; }

  const std::vector<FieldMeta> &field_list() const { return field_list_; }
  std::vector<FieldMeta> &field_list() { return field_list_; }
  const std::vector<std::unique_ptr<Expression>> &expr_list() const { return expr_list_; }
  std::vector<std::unique_ptr<Expression>> &expr_list() { return expr_list_; }

  size_t size() const { return expr_list_.size(); }

private:
  Table  *table_ = nullptr;
  std::vector<std::unique_ptr<Expression>> expr_list_;
  std::vector<FieldMeta> field_list_;

};

#endif // UPDATE_LOGICAL_OPERATOR_H_
