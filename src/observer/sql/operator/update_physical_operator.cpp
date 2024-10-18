/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "update_physical_operator.h"
#include "common/log/log.h"
#include "sql/expr/tuple.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, Value &&value, Field &&field)
    : table_(table), value_(std::move(value)), field_(std::move(field))
{}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  LOG_INFO("UpdatePhysicalOperator::open(): begin");
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  std::unique_ptr<PhysicalOperator> &child = children_[0];
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  while (OB_SUCC(rc = child->next())) {
    LOG_INFO("UpdatePhysicalOperator::open(): collect record");
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record   &record    = row_tuple->record();
    records_.emplace_back(std::move(record));
  }

  child->close();

  // 先收集记录再更新
  for (Record &record : records_) {
    LOG_INFO("UpdatePhysicalOperator::open(): update record");
    rc = trx_->delete_record(table_, record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record: %s", strrc(rc));
      return rc;
    }

    Record new_record;
    rc = new_record.copy_data(record.data(), record.len());
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to copy record: %s", strrc(rc));
      return rc;
    }

    int const field_offset = field_.meta()->offset();
    int const field_len    = field_.meta()->len();
    new_record.set_field(field_offset, field_len, value_.data());
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to set field record: %s", strrc(rc));
      return rc;
    }

    rc = trx->insert_record(table_, new_record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record: %s", strrc(rc));
      return rc;
    }
  }

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }
