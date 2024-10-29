#pragma once

#include <memory>
#include <vector>

#include "sql/expr/expression.h"
#include "sql/operator/logical_operator.h"
#include "storage/field/field.h"
#include "sql/stmt/order_stmt.h"

/**
 * @brief order 表示排序
 * @ingroup LogicalOperator
 */
class OrderLogicalOperator : public LogicalOperator
{
public:
  OrderLogicalOperator(std::vector<std::unique_ptr<OrderUnit>> &&order_units) : order_units_(std::move(order_units)) {};
  virtual ~OrderLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::ORDER_BY; }

  std::vector<std::unique_ptr<Expression>>       &expressions() { return expressions_; }
  const std::vector<std::unique_ptr<Expression>> &expressions() const { return expressions_; }

  const std::vector<std::unique_ptr<OrderUnit>> &OrderUnits() const { return order_units_; }
  std::vector<std::unique_ptr<OrderUnit>> &&OrderUnits() { return std::move(order_units_); }

private:
  std::vector<std::unique_ptr<OrderUnit>> order_units_;
};
