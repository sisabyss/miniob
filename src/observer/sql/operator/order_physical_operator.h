#pragma once

#include "sql/expr/expression.h"
#include "sql/operator/physical_operator.h"
#include "sql/stmt/order_stmt.h"
#include "sql/expr/tuple.h"
#include <memory>

struct SortTarget{
    int scanned_tuples_idx;     // 对应的元组下标
    vector<Value> value;       // 即将参与排序的值集合
};


class OrderPhysicalOperator : public PhysicalOperator
{
public:
  OrderPhysicalOperator(vector<unique_ptr<OrderUnit>> &&order_units) : order_units_(std::move(order_units)) {};

  virtual ~OrderPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::ORDER_BY; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

  RC tuple_schema(TupleSchema &schema) const override;

private:
  void addSortTarget(vector<Value>& values, int idx);

  vector<unique_ptr<OrderUnit>> order_units_;  // 排序规则的算子单元
  vector<ValueListTuple> scanned_tuples_;   // 扫描出的所有元组
  vector<SortTarget> sort_targer_;  // 排序即对此集合排序
  vector<SortTarget>::iterator it_;

  Tuple *tuple_ = nullptr;
};
