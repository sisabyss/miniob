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

#include "sql/stmt/filter_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/parser/expression_binder.h"
#include "storage/common/condition_filter.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

RC FilterStmt::create(ExpressionBinder const &binder, const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt)
{
  RC rc = RC::SUCCESS;
  stmt  = nullptr;

  FilterStmt *tmp_stmt = new FilterStmt();
  for (int i = 0; i < condition_num; i++) {
    rc = create_filter_unit(binder, conditions[i], tmp_stmt->filters_);
    if (OB_FAIL(rc)) {
      delete tmp_stmt;
      LOG_WARN("failed to create filter unit. condition index=%d", i);
      return rc;
    }
  }

  stmt = tmp_stmt;
  return rc;
}

RC FilterStmt::create_filter_unit(ExpressionBinder const &binder, const ConditionSqlNode &condition, CompositeConditionFilter &filters)
{
  RC rc = RC::SUCCESS;
  rc = filters.add_filter(binder, condition);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to create filter unit, rc:%s", strrc(rc));
  }
  return rc;
}
