#pragma once

#include "common/types.h"
#ifndef CREATE_TABLE_LOGICAL_OPERATOR_H_
#define CREATE_TABLE_LOGICAL_OPERATOR_H_

#include "sql/operator/logical_operator.h"

class Db;

/**
 * @brief CreateTable逻辑算子
 * @ingroup LogicalOperator
 */
class CreateTableLogicalOperator : public LogicalOperator
{
public:
  CreateTableLogicalOperator(Db *db, const std::string &table_name, const std::vector<AttrInfoSqlNode> &attr_infos, StorageFormat storage_format)
    : db_(db),table_name_(table_name), attr_infos_(attr_infos), storage_format_(storage_format)
  {}

  virtual ~CreateTableLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::CREATE_TABLE; }

  std::string &table_name() { return table_name_; }

  std::vector<AttrInfoSqlNode> &attr_infos() { return attr_infos_; }

  StorageFormat storage_format() { return storage_format_; }

  Db *db() { return db_; }

private:
  Db *db_ = nullptr;
  std::string table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
  StorageFormat storage_format_;
};

#endif // CREATE_TABLE_LOGICAL_OPERATOR_H_
