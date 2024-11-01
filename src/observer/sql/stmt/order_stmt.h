#pragma once

#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include <vector>
#include <unordered_map>
#include "sql/parser/expression_binder.h"


struct OrderUnit
{
public:
  explicit OrderUnit() = default;
  ~OrderUnit() {}

  bool inc_order_;   // 是否为升序
  unique_ptr<Expression> field_expr_;      // 待排序字段
};

/**
 * @brief Order排序语句
 * @ingroup Statement
 */
class OrderStmt
{
public:
  OrderStmt(){};
  ~OrderStmt(){}

public:
  const std::vector<std::unique_ptr<OrderUnit>> &order_units() const { return order_units_; }
  std::vector<std::unique_ptr<OrderUnit>> &&order_units() { return std::move(order_units_); }

public:
  static RC create(const Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables, const OrderSqlNode *orders, int ordernum, OrderStmt *&stmt, ExpressionBinder& expression_binder);

  static RC create_order_unit(const Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables, const OrderSqlNode& orderSqlNode, OrderUnit*& order_unit, ExpressionBinder& expression_binder);

private:
  std::vector<std::unique_ptr<OrderUnit>> order_units_;
};
