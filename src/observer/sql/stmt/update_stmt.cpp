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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/field/field.h"

UpdateStmt::UpdateStmt(Table *table, Value value, Field field, FilterStmt *filter_stmt)
    : table_(table), value_(value), field_(field), filter_stmt_(filter_stmt)
{}

UpdateStmt::~UpdateStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  RC rc = RC::SUCCESS;

  // collect tables in `from` statement
  Table *table = db->find_table(update.relation_name.c_str());
  if (table == nullptr) {
    LOG_WARN("table %s not exists", update.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  unordered_map<string, Table *> table_map;
  table_map.emplace(update.relation_name, table);

  // collect attr field to be updated
  const FieldMeta *field_meta = table->table_meta().field(update.attribute_name.c_str());
  if (field_meta == nullptr) {
    LOG_WARN("field %s not exist in table %s", update.attribute_name.c_str(), update.relation_name.c_str());
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  if (field_meta->type() == update.value.attr_type()
      || (field_meta->nullable() && update.value.attr_type() == AttrType::NULLS)
      || (field_meta->type() == AttrType::TEXTS && update.value.attr_type() == AttrType::CHARS)
      || (field_meta->type() == AttrType::DATES && update.value.attr_type() == AttrType::CHARS)) {
    /* skip */
  } else {
    LOG_WARN("update value failed to cast into target type, src=%s, target=%s",
        attr_type_to_string(update.value.attr_type()), attr_type_to_string(field_meta->type()));
    return RC::INVALID_ARGUMENT;
  }

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;

  BinderContext binder_context;
  binder_context.add_table(table);
  ExpressionBinder expression_binder(binder_context);

  rc          = FilterStmt::create(db,
      table,
      &table_map,
      update.conditions.data(),
      static_cast<int>(update.conditions.size()),
      filter_stmt,
      expression_binder);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  // everything alright
  auto *update_stmt         = new UpdateStmt();

  update_stmt->table_       = table;
  update_stmt->value_       = update.value;
  update_stmt->filter_stmt_ = filter_stmt;
  update_stmt->field_       = Field(table, field_meta);
  stmt = update_stmt;
  return RC::SUCCESS;
}
