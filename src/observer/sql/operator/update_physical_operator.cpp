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

UpdatePhysicalOperator::UpdatePhysicalOperator(
  Table *table,
  std::vector<std::unique_ptr<Expression>> &&expr_list,
  std::vector<FieldMeta> &&field_list
)
    : table_(table), expr_list_(std::move(expr_list)), field_list_(std::move(field_list))
{
}

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
    // BUG: record maybe no owner
    Record    new_record(record);

    for (size_t i = 0; i < field_list_.size(); i++) {
      auto &expr = expr_list_.at(i);
      auto &field = field_list_.at(i);
      rc = set_expr_to_record(expr, field, tuple, new_record);
      if (OB_FAIL(rc)) {
        LOG_WARN("failed to set expr into record, rc:%s", strrc(rc));
        return rc;
      }
    }

    table_->update_record(record, new_record);
  }

  child->close();

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::set_expr_to_record(
  std::unique_ptr<Expression> const &expr,
  FieldMeta const &field,
  const Tuple *tuple,
  Record &record
) {
  RC rc = RC::SUCCESS;
  const int sys_field_num = table_->table_meta().sys_field_num();
  const int field_index = field.field_id() + sys_field_num;

  Value value;
  expr->open(trx_);
  if (expr->has_multi_valued()) {
    LOG_WARN("update expression must have single value");
    return RC::INVALID_ARGUMENT;
  }
  expr->get_value(*tuple, value);
  expr->close();

  // update stage
  const FieldMeta *null_bitmap_field = table_->table_meta().null_field();
  common::Bitmap null_bitmap(record.data() + null_bitmap_field->offset(), null_bitmap_field->len());
  if (value.attr_type() == AttrType::NULLS) {
    if (field.nullable()) {
      null_bitmap.set_bit(field_index);
    } else {
      LOG_WARN("update value null but field %s is not nullable.", field.name());
      return RC::INVALID_ARGUMENT;
    }
  } else if (field.type() == value.attr_type()) {
      rc = table_->set_value_to_record(record.data(), value, &field);
  } else if ((field.type() == AttrType::DATES && value.attr_type() == AttrType::CHARS)
          || (field.type() == AttrType::TEXTS && value.attr_type() == AttrType::CHARS)) {
      Value real_value;
      rc = Value::cast_to(value, field.type(), real_value);
      if (OB_FAIL(rc)) {
        LOG_WARN("update value failed to cast value. table name:%s, field name:%s, value:%s ",
            table_->table_meta().name(), field.name(), value.to_string().c_str());
        return RC::INVALID_ARGUMENT;
      } else {
        rc = table_->set_value_to_record(record.data(), real_value, &field);
      }
  } else {
    LOG_WARN("update value failed to cast into target type, src=%s, target=%s",
        attr_type_to_string(value.attr_type()), attr_type_to_string(field.type()));
    return RC::INVALID_ARGUMENT;
  }
  return rc;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}
