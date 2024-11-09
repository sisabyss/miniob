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
// Created by Wangyunlai on 2024/05/29.
//

#pragma once

#include <vector>

#include "sql/expr/expression.h"

class BinderContext
{
public:
  using tablemap_t = std::unordered_map<std::string, Table *>;
  using aliasmap_t = std::unordered_map<std::string, std::string>;
  using tablenode_t = std::pair<std::string, Table *>;
  using aliasnode_t = std::pair<std::string, std::string>;
  BinderContext(const Db *db) : query_db_(db) {};
  virtual ~BinderContext() = default;

  void add_table(tablenode_t const &table)
  {
    query_tables_.push_back(table.second);
    query_table_map_.insert(table);
  }
  void add_table(std::string const &alias, Table *table) { add_table({alias, table}); }

  void add_alias(aliasnode_t const &alias) { alias_map_.insert(alias); }
  void add_alias(std::string table, std::string const &alias) { add_alias({table, alias}); }

  Table *find_table(const char *table_name) const;
  const char *find_table_alias(const char *table_name) const;

  const Db *query_db() const { return query_db_; }
  const tablemap_t &query_table_map() const { return query_table_map_; }
  const std::vector<Table *> &query_tables() const { return query_tables_; }

private:
  const Db  *query_db_;
  tablemap_t query_table_map_;
  aliasmap_t alias_map_;
  std::vector<Table *> query_tables_;
};

/**
 * @brief 绑定表达式
 * @details 绑定表达式，就是在SQL解析后，得到文本描述的表达式，将表达式解析为具体的数据库对象
 */
class ExpressionBinder
{
public:
  ExpressionBinder(BinderContext const &context) : context_(context) {}
  ExpressionBinder(BinderContext &&context) : context_(std::move(context)) {}
  virtual ~ExpressionBinder() = default;

  ExpressionBinder(ExpressionBinder const &) = default;

  RC bind_expression(std::unique_ptr<Expression> &expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);

private:
  RC bind_star_expression(
      std::unique_ptr<Expression> &star_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_unbound_field_expression(
      std::unique_ptr<Expression> &unbound_field_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_field_expression(
      std::unique_ptr<Expression> &field_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_value_expression(
      std::unique_ptr<Expression> &value_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_cast_expression(
      std::unique_ptr<Expression> &cast_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_comparison_expression(
      std::unique_ptr<Expression> &comparison_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_conjunction_expression(
      std::unique_ptr<Expression> &conjunction_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_arithmetic_expression(
      std::unique_ptr<Expression> &arithmetic_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_aggregate_expression(
      std::unique_ptr<Expression> &aggregate_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_subquery_expression(
      std::unique_ptr<Expression> &subquery_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);
  RC bind_exprlist_expression(
      std::unique_ptr<Expression> &exprlist_expr, std::vector<std::unique_ptr<Expression>> &bound_expressions);

private:
  BinderContext context_;
};
