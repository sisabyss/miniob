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
#include <cstring>

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

    /** 两种情况
     *  1. 插入的值长度小于field长度: 后面补0
     *  2. 插入的值长度大于field长度: 截断value
     */
    if (value_.length() >= field_len) {
      new_record.set_field(field_offset, field_len, value_.data());
    } else {
      // 插入的值长度小于 field_len，右侧补 0
      // 创建一个新的字符数组，长度为 field_len
      std::vector<char> padded_value(field_len, 0);  // 初始化一个长度为 field_len 的字符数组，填充 '0'

      // 将 value_ 中的有效数据拷贝到 padded_value 中
      std::memcpy(padded_value.data(), value_.data(), value_.length());  // 复制 value_ 的数据到 padded_value

      // 设置新记录，使用补零后的数据
      new_record.set_field(field_offset, field_len, padded_value.data());
    }
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
