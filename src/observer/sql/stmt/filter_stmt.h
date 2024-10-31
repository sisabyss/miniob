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

#include "sql/parser/parse_defs.h"
#include "sql/parser/expression_binder.h"
#include "sql/stmt/stmt.h"
#include "storage/common/condition_filter.h"
#include <unordered_map>

class Db;
class Table;
class FieldMeta;

/**
 * @brief Filter/谓词/过滤语句
 * @ingroup Statement
 */
class FilterStmt
{
public:
  FilterStmt() = default;
  virtual ~FilterStmt() = default;

public:
  const CompositeConditionFilter &filter_units() const { return filters_; }
  CompositeConditionFilter &&filter_units() { return std::move(filters_); }

public:
  static RC create(ExpressionBinder const &binder, const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt);

  static RC create_filter_unit(ExpressionBinder const &binder, const ConditionSqlNode &condition, CompositeConditionFilter &filters);

private:
  CompositeConditionFilter filters_;
};
