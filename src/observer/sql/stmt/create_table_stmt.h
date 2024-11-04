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
// Created by Wangyunlai on 2023/6/13.
//

#pragma once

#include <string>
#include <vector>

#include "common/types.h"
#include "sql/stmt/stmt.h"

class Db;

/**
 * @brief 表示创建表的语句
 * @ingroup Statement
 * @details 虽然解析成了stmt，但是与原始的SQL解析后的数据也差不多
 */
class CreateTableStmt : public Stmt
{
public:
  CreateTableStmt(
      Db *db,
      const std::string &table_name,
      const std::vector<AttrInfoSqlNode> &attr_infos,
      StorageFormat storage_format,
      Stmt *select_stmt)
      : db_(db),
        table_name_(table_name),
        attr_infos_(attr_infos),
        storage_format_(storage_format),
        select_stmt_(select_stmt)
  {}

  virtual ~CreateTableStmt()
  {
    if (nullptr != select_stmt_) {
      delete select_stmt_;
    }
  }

  StmtType type() const override { return StmtType::CREATE_TABLE; }

  std::string                  &table_name() { return table_name_; }
  std::vector<AttrInfoSqlNode> &attr_infos() { return attr_infos_; }
  StorageFormat                 storage_format() const { return storage_format_; }
  Db                           *db() { return db_; }
  Stmt                         *create_select() { return select_stmt_; }

  static RC            create(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt, SelectSqlNode &select_sql);
  static StorageFormat get_storage_format(const char *format_str);

private:
  Db                          *db_ = nullptr;
  std::string                  table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
  StorageFormat                storage_format_;
  Stmt                        *select_stmt_ = nullptr;
};
