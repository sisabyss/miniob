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
// Created by Wangyunlai on 2021/5/7.
//

#pragma once

#include "common/type/attr_type.h"
#include "sql/parser/expression_binder.h"
#include "sql/parser/parse.h"
#include "sql/expr/expression.h"
#include <memory>
#include <vector>

class Record;
class Table;

class ConditionFilter
{
public:
  virtual ~ConditionFilter();

  /**
   * Filter one record
   * @param rec
   * @return true means match condition, false means failed to match.
   */
  virtual RC filter(Tuple *tuple, bool &res) const = 0;
};

class ExpressionBinder;
class DefaultConditionFilter : public ConditionFilter
{
public:
  DefaultConditionFilter(ExpressionBinder const &binder)
    : expr_binder_(binder) {};
  virtual ~DefaultConditionFilter();

  RC init(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right, CompOp comp_op);
  RC init(const ConditionSqlNode &condition);

  virtual RC filter(Tuple *tuple, bool &res) const override;

public:
  BinderContext createBinderContext(Table *table);
  const std::unique_ptr<Expression> &left() const { return left_expr_; }
  const std::unique_ptr<Expression> &right() const { return right_expr_; }
  const std::unique_ptr<ComparisonExpr> &cmp_expr() const { return cmp_expr_; }

  CompOp   comp_op() const { return comp_op_; }
  AttrType attr_type() const
  {
    if (cmp_expr_ != nullptr) {
      return cmp_expr_->value_type();
    } else {
      return AttrType::UNDEFINED;
    }
  }

private:
  std::unique_ptr<Expression>     left_expr_;
  std::unique_ptr<Expression>     right_expr_;
  CompOp   comp_op_   = NO_OP;
  std::unique_ptr<ComparisonExpr> cmp_expr_;

  ExpressionBinder expr_binder_;
};

class CompositeConditionFilter : public ConditionFilter
{
public:
  CompositeConditionFilter() = default;
  CompositeConditionFilter(CompositeConditionFilter &&) = default;
  virtual ~CompositeConditionFilter();

  virtual RC filter(Tuple *tuple, bool &res) const override;

  RC add_filter(std::unique_ptr<ConditionFilter> &&filter);
  RC add_filter(ExpressionBinder const &binder, const ConditionSqlNode &condition);

public:
  size_t                 filter_num() const { return filters_.size(); }
  const ConditionFilter &filter(int index) const { return *filters_.at(index); }

private:
  std::vector<std::unique_ptr<ConditionFilter>> filters_;
};
