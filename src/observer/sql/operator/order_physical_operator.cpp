#include "sql/operator/order_physical_operator.h"
#include "common/log/log.h"
#include "common/type/attr_type.h"
#include "sql/expr/tuple.h"
#include "sql/stmt/order_stmt.h"
#include "storage/field/field.h"
#include "storage/record/record.h"
#include <algorithm>

class cmp
{
private:
  vector<bool> sortRules; // 多少个SortUnit 就有多少个规则，大于0表示升序
public:
  cmp(vector<bool> const &sortRules){
    this->sortRules = sortRules;
  }

  bool operator () (SortTarget A, SortTarget B) const {     // whether B should put ahead of A
    for(size_t i = 0; i < sortRules.size(); i++ ){
      // 获得当前排序规则
      bool rule = sortRules.at(i);

      // 取出 A 和 B 当前value
      Value va = A.value.at(i);
      Value vb = B.value.at(i);

      // 获得 A 和 B 的类型
      AttrType aa = va.attr_type();
      AttrType ba = vb.attr_type();

      if (aa == AttrType::NULLS && ba == AttrType::NULLS) {
        continue;
      } else if (aa == AttrType::NULLS) {
        return rule ? true : false;
      } else if (ba == AttrType::NULLS) {
        return rule ? false : true;
      } else{
        int cmpRes = va.compare(vb);
        // -1  ->  left < right
        // 0   ->  left = right
        // 1   ->  left > right

        if(cmpRes == 0){ continue; }
        if(cmpRes == 1){ return rule? false:true;}
        return rule? true:false;
      }
    }
    return false;    // 所有条件都等于，保留原顺序
  }
};

RC OtherTuple2ValueListTuple(ValueListTuple *resTuple, Tuple* oriTuple){
    RC rc = RC::SUCCESS;

    if(oriTuple->getType() == TupleType::ROW_TUPLE){
      oriTuple = static_cast<RowTuple*> (oriTuple);
    }
    else if(oriTuple->getType() == TupleType::JOINED_TUPLE){
      oriTuple = static_cast<JoinedTuple*> (oriTuple);
    }
    else{
      return RC::INTERNAL;
    }

    std::vector<Value> cells;              // 存储字段的值
    std::vector<TupleCellSpec> specs;      // 存储字段的类型
    for(int i = 0; i < oriTuple->cell_num(); i++){
        Value tmp;

        rc = oriTuple->cell_at(i,tmp);
        if(rc != RC::SUCCESS){
          LOG_WARN("fail to get value of RowTuple");
          return RC::INTERNAL;
        }

        TupleCellSpec tmp2;
        rc = oriTuple->spec_at(i,tmp2);
        if(rc != RC::SUCCESS){
          LOG_WARN("fail to get tupleCellSpec of RowTuple");
          return RC::INTERNAL;
        }

        cells.push_back(tmp);
        specs.push_back(tmp2);
    }
    resTuple->set_cells(cells);
    resTuple->set_names(specs);
    return RC::SUCCESS;
}

RC OrderPhysicalOperator::open(Trx *trx)
{
  // 1 - 获得排序规则
  vector<bool> sortRules;
  for(auto const &ou: order_units_){
    sortRules.push_back(ou->inc_order_);
  }

  // 2 - 递归处理子算子，要求保证本层在倒数第二层，即上层有且仅能有 过滤算子
  if (children_.size() != 1) {
    LOG_WARN("order operator must has one child");
    return RC::INTERNAL;
  }

  PhysicalOperator &child = *children_[0];
  RC                rc    = child.open(trx);
  if (OB_FAIL(rc)) {
    LOG_INFO("failed to open child operator. rc=%s", strrc(rc));
    return rc;
  }

// 记录一条一条扫上来
  while (OB_SUCC(rc = child.next())) {
    Tuple *child_tuple = child.current_tuple();
    if (nullptr == child_tuple) {
      LOG_WARN("failed to get tuple from child operator. rc=%s", strrc(rc));
      return RC::INTERNAL;
    }


    // 3 - 保存原始 tuple， 以 ValueListTuple 的形式
    // TODO 优化：将 ValueListTuple 对象存储在文件之中，在磁盘进行排序 Big Data Order
    ValueListTuple resTuple;

    if(child_tuple->getType() != TupleType::ROW_TUPLE && child_tuple->getType() != TupleType::JOINED_TUPLE){
      LOG_WARN("order operator tuple must be rowTuple or joinedTuple");
      return RC::INTERNAL;
    }
    else{
      rc = OtherTuple2ValueListTuple(&resTuple, child_tuple);
      if(rc != RC::SUCCESS){
        LOG_WARN("Failed to transfer childTuple to rowTuple or joinedTuple");
        return RC::INTERNAL;
      }
    }
    scanned_tuples_.emplace_back(resTuple);

    // 4 - 筛选出需要排序的值
    vector<Value> values;
    for(auto const &ou: order_units_){
        auto const &fieldExpr = ou->field_expr_;
        Value curValue;
        rc = fieldExpr->get_value(*child_tuple, curValue);
        if(rc != RC::SUCCESS){
            LOG_WARN("failed to get value from child tuple");
            return RC::INTERNAL;
        }
        values.push_back(curValue);
    }
    // 保存需要排序的值（以封装的对象形式）
    addSortTarget(values, scanned_tuples_.size()-1);
  }

    // 5 - 对所有记录加以排序
    std::sort(sort_targer_.begin(), sort_targer_.end(), cmp(sortRules));

    it_ = sort_targer_.begin();

    return RC::SUCCESS;
}

RC OrderPhysicalOperator::next()
{
    if (sort_targer_.end() != it_) {
      auto current_idx = (*it_).scanned_tuples_idx;
      tuple_ = &scanned_tuples_.at(current_idx);
      it_++;
      return RC::SUCCESS;
    }
    return RC::RECORD_EOF;
}

Tuple *OrderPhysicalOperator::current_tuple() {
  return tuple_;
}

RC OrderPhysicalOperator::close()
{
  return children_[0]->close();
}

RC OrderPhysicalOperator::tuple_schema(TupleSchema &schema) const
{
  return children_[0]->tuple_schema(schema);
}

void OrderPhysicalOperator::addSortTarget(vector<Value>& values, int idx){
    SortTarget sort_targer;
    sort_targer.scanned_tuples_idx = idx;
    sort_targer.value = values;
    sort_targer_.push_back(sort_targer);
}
