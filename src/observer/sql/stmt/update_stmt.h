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

#pragma once

#include "common/rc.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field.h"

class Table;
class FilterStmt;
class Expression;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt() = default;
  UpdateStmt(
    Table *table,
    std::vector<FieldMeta> &&field_list,
    std::vector<std::unique_ptr<Expression>>&& expr_list,
    FilterStmt *filter_stmt
  );
  virtual ~UpdateStmt();

  StmtType type() const override { return StmtType::UPDATE; }

public:
  static RC create(const Db *db, UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  Table       *table() const { return table_; }
  FilterStmt  *filter_stmt() const { return filter_stmt_; }

  const std::vector<FieldMeta> &field_list() const { return field_list_; }
  std::vector<FieldMeta> &field_list() { return field_list_; }
  const std::vector<std::unique_ptr<Expression>> &expr_list() const { return expr_list_; }
  std::vector<std::unique_ptr<Expression>> &expr_list() { return expr_list_; }

  size_t size() const { return expr_list_.size(); }

private:
  Table      *table_        = nullptr;
  std::vector<FieldMeta>                   field_list_;
  std::vector<std::unique_ptr<Expression>> expr_list_;
  FilterStmt *filter_stmt_  = nullptr;
};
