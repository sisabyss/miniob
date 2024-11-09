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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/order_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "sql/parser/expression_binder.h"

using namespace std;
using namespace common;

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }

  if (nullptr != orders_by_) {
    delete orders_by_;
    orders_by_ = nullptr;
  }
}

RC SelectStmt::create(const Db *db, SelectSqlNode &select_sql, Stmt *&stmt)
{
  RC rc = RC::SUCCESS;
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  BinderContext binder_context(db);

  // check alias frist
  rc = check_alias(select_sql.relations, select_sql.expressions);
  if (OB_FAIL(rc)) {
    LOG_INFO("alias check error, strrc:%s", strrc(rc));
    return rc;
  }

  // collect tables in `from` statement
  vector<Table *>                tables;
  unordered_map<string, Table *> table_map;
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const string &table_name = select_sql.relations[i].relation;
    if (nullptr == table_name.c_str()) {
      LOG_WARN("invalid argument. relation name is null. index=%d", i);
      return RC::INVALID_ARGUMENT;
    }

    Table *table = db->find_table(table_name.c_str());
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name.c_str());
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    binder_context.add_table(table_name, table);
    tables.push_back(table);
    table_map.insert({table_name, table});

    // handle table alias
    const string &table_alias = select_sql.relations[i].alias;
    if (!table_alias.empty()) {
      binder_context.add_table(table_alias, table);
      binder_context.add_alias(table_name, table_alias);
      table_map.insert({table_alias, table});
    }
  }

  // collect query fields in `select` statement
  vector<unique_ptr<Expression>> bound_expressions;
  ExpressionBinder expression_binder(binder_context);
  
  for (unique_ptr<Expression> &expression : select_sql.expressions) {
    RC rc = expression_binder.bind_expression(expression, bound_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }

  vector<unique_ptr<Expression>> group_by_expressions;
  for (unique_ptr<Expression> &expression : select_sql.group_by) {
    RC rc = expression_binder.bind_expression(expression, group_by_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  if (!select_sql.conditions.empty()) {
    rc          = FilterStmt::create(
        expression_binder,
        select_sql.conditions.data(),
        static_cast<int>(select_sql.conditions.size()),
        filter_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct filter stmt");
      return rc;
    }
  }

  // 创建排序stmt
  OrderStmt* order_stmt = nullptr;
  rc = OrderStmt::create(db,
      default_table,
      &table_map,
      select_sql.order_sql_nodes.data(),
      static_cast<int>(select_sql.order_sql_nodes.size()),
      order_stmt,
      expression_binder
      );
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct order by stmt");
    return rc;
  }

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();

  select_stmt->tables_.swap(tables);
  select_stmt->query_expressions_.swap(bound_expressions);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->group_by_.swap(group_by_expressions);
  select_stmt->orders_by_   = order_stmt;
  stmt                      = select_stmt;
  return RC::SUCCESS;
}

RC SelectStmt::check_alias(
  std::vector<RelationRef> const &table_list,
  std::vector<std::unique_ptr<Expression>> const &expr_list
) {
  std::unordered_set<std::string> table_alias_set; // 检测表别名重复
  for (auto const &table_ref : table_list) {
    if (table_ref.alias.empty()) continue;
    if (table_alias_set.count(table_ref.alias) != 0) {
      return RC::INVALID_ARGUMENT;
    }
    table_alias_set.insert(table_ref.alias);
  }
  /* Maybe unused */(void)expr_list;
  return RC::SUCCESS;
}
