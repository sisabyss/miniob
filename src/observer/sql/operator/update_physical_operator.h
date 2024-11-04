/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#ifndef UPDATE_PHYSICAL_OPERATOR_H_
#define UPDATE_PHYSICAL_OPERATOR_H_

#include "sql/operator/physical_operator.h"

class UpdateStmt;
class Record;
class FieldMeta;

/**
 * @brief 更新物理算子
 * @ingroup PhysicalOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(
    Table *table,
    std::vector<std::unique_ptr<Expression>>&& expr_list,
    std::vector<FieldMeta> &&field_list
  );
  virtual ~UpdatePhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::UPDATE; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
  Tuple *current_tuple() override { return nullptr; }

private:
  RC set_expr_to_record(
    std::unique_ptr<Expression> const &expr,
    FieldMeta const &field,
    const Tuple *tuple,
    Record &record
);

private:
  Table              *table_ = nullptr;
  Trx                *trx_   = nullptr;

  std::vector<std::unique_ptr<Expression>> expr_list_;
  std::vector<FieldMeta>                   field_list_;
};


#endif // UPDATE_PHYSICAL_OPERATOR_H_
