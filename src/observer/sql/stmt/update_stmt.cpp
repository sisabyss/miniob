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
#include "common/lang/string.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/field/field.h"
#include "storage/table/table_meta.h"


UpdateStmt::UpdateStmt(
  Table *table,
  std::vector<FieldMeta> &&field_list,
  std::vector<std::unique_ptr<Expression>>&& expr_list,
  FilterStmt *filter_stmt
) : table_(table),
    field_list_(std::move(field_list)),
    expr_list_(std::move(expr_list)),
    filter_stmt_(filter_stmt)
{
}


UpdateStmt::~UpdateStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC UpdateStmt::create(const Db *db, UpdateSqlNode &update, Stmt *&stmt)
{
  RC rc = RC::SUCCESS;

  // collect tables in `from` statement
  if (common::is_blank(update.relation_name.c_str())) {
    LOG_WARN("update table name must not be emplty.");
    return RC::INTERNAL;
  }
  Table *table = db->find_table(update.relation_name.c_str());
  if (table == nullptr) {
    LOG_WARN("table %s not exists", update.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  unordered_map<string, Table *> table_map;
  table_map.emplace(update.relation_name, table);

  // collect attr field to be updated
  std::vector<FieldMeta> fields;
  const TableMeta &table_meta = table->table_meta();
  for (auto const &attr : update.attr_list) {
    const FieldMeta* update_field = table_meta.field(attr.c_str());
    if (update_field == nullptr) {
      LOG_WARN("field %s not exist in table %s", attr.c_str(), update.relation_name.c_str());
      return RC::SCHEMA_FIELD_NOT_EXIST;
    }
    fields.push_back(*update_field);
  }

  // collect and bind expressions to be emplace
  std::vector<std::unique_ptr<Expression>> exprs;

  BinderContext binder_context(db);
  binder_context.add_table(table);
  ExpressionBinder binder(binder_context);
  for (auto &expr : update.expr_list) {
    vector<unique_ptr<Expression>> bound_expressions;
    rc = binder.bind_expression(expr, bound_expressions);
    exprs.emplace_back(std::move(bound_expressions.front()));
  }
  update.expr_list.clear();

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;

  if (!update.conditions.empty()) {
    BinderContext binder_context(db);
    binder_context.add_table(table);
    ExpressionBinder binder(binder_context);

    rc          = FilterStmt::create(
        binder,
        update.conditions.data(),
        static_cast<int>(update.conditions.size()),
        filter_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct filter stmt");
      return rc;
    }
  }

  // everything alright
  auto *update_stmt         = new UpdateStmt();

  update_stmt->table_       = table;
  update_stmt->expr_list_   = std::move(exprs);
  update_stmt->filter_stmt_ = filter_stmt;
  update_stmt->field_list_  = std::move(fields);
  stmt = update_stmt;
  return RC::SUCCESS;
}
