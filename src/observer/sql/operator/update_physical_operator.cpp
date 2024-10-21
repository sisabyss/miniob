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
#include "common/rc.h"
#include "common/text.hpp"
#include "common/type/attr_type.h"
#include "sql/expr/tuple.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include <cstring>

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, Value &&value, Field &&field)
    : table_(table), value_(std::move(value)), field_(std::move(field))
{}

RC UpdatePhysicalOperator::open(Trx *trx)
{
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

    if (field_.meta()->type() == value_.attr_type() || (field_.meta()->type() == AttrType::TEXTS && value_.attr_type() == AttrType::CHARS)) {
      rc = set_value_to_record(new_record.data(), value_, field_.meta());
    } else {
      Value real_value;
      rc = Value::cast_to(value_, field_.meta()->type(), real_value);
      if (OB_FAIL(rc)) {
        LOG_WARN("failed to cast value. field name:%s, value:%s ",
            field_.meta()->name(), value_.to_string().c_str());
      }
      rc = set_value_to_record(new_record.data(), real_value, field_.meta());
    }

    if (OB_FAIL(rc)) {
      LOG_WARN("failed to update record.");
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

RC UpdatePhysicalOperator::set_value_to_record(char *record_data, const Value &value, const FieldMeta *field) const
{
  size_t       copy_len = field->len();
  const size_t data_len = value.length();
  RC           rc       = RC::SUCCESS;

  if (field->type() == AttrType::TEXTS) {
    Text text;
#ifdef __TEST__
    text.len = std::min(MAX_TEXT_LENGTH, value.length());
#else
    text.len = value.length();
#endif
    rc = table_->new_text(&text.id, value.data(), text.len);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to write text into table, rc: %s", strrc(rc));
      return rc;
    }
    memcpy(record_data + field->offset(), &text, sizeof(text));
    return RC::SUCCESS;
  }

  if (field->type() == AttrType::CHARS) {
    if (copy_len > data_len) {
      memset(record_data + field->offset(), 0, copy_len);
      copy_len = data_len + 1;
    }
  }
  memcpy(record_data + field->offset(), value.data(), copy_len);
  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }
