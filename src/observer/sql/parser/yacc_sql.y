
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <optional>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundAggregateExpr *create_aggregate_expression(const char *aggregate_name,
                                           Expression *child,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

%}

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        BY
        CREATE
        DROP
        GROUP
        TABLE
        TABLES
        INDEX
        CALC
        SELECT
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        FLOAT_T
        DATE_T
        TEXT_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EXPLAIN
        STORAGE
        FORMAT
        EQ
        LT
        GT
        LE
        GE
        NE
        MAX
        MIN
        SUM
        AVG
        COUNT
        INNER
        JOIN
        UNIQUE
        IS_SYM
        NOT
        LIKE
        NULL_SYM
        NULLABLE_SYM
        ORDER
        ASC

/** union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据 **/
%union {
  ParsedSqlNode *                            sql_node;
  ConditionSqlNode *                         condition;
  OrderSqlNode *                             order_by;
  std::vector<OrderSqlNode> *                order_by_list;
  Value *                                    value;
  enum CompOp                                comp;
  RelAttrSqlNode *                           rel_attr;
  std::vector<AttrInfoSqlNode> *             attr_infos;
  AttrInfoSqlNode *                          attr_info;
  Expression *                               expression;
  std::vector<std::unique_ptr<Expression>> * expression_list;
  std::vector<Value> *                       value_list;
  std::vector<ConditionSqlNode> *            condition_list;
  std::vector<RelAttrSqlNode> *              rel_attr_list;
  TableRefSqlNode *                          table_ref_list;
  std::vector<std::string> *                 id_list;
  char *                                     string;
  int                                        number;
  float                                      floats;
  bool                                       bools;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> SSS
//非终结符

/** type 定义了各种解析后的结果输出的是什么类型。类型对应了 union 中的定义的成员变量名称 **/
%type <order_by>            order_by
%type <order_by_list>       order_by_list
%type <order_by_list>       opt_order_by
%type <number>              type
%type <condition>           condition
%type <value>               value
%type <number>              number
%type <string>              relation
%type <comp>                comp_op
%type <rel_attr>            rel_attr
%type <attr_infos>          attr_def_list
%type <attr_info>           attr_def
%type <value_list>          value_list
%type <condition_list>      where
%type <condition_list>      condition_list
%type <string>              storage_format
%type <table_ref_list>      table_ref_list
%type <table_ref_list>      comma_ref_list
%type <table_ref_list>      join_ref_list
%type <expression>          expression
%type <expression_list>     expression_list
%type <expression_list>     group_by
%type <bools>               opt_unique
%type <bools>               opt_null;
%type <id_list>             ID_list;
%type <sql_node>            calc_stmt
%type <sql_node>            select_stmt
%type <sql_node>            insert_stmt
%type <sql_node>            update_stmt
%type <sql_node>            delete_stmt
%type <sql_node>            create_table_stmt
%type <sql_node>            drop_table_stmt
%type <sql_node>            show_tables_stmt
%type <sql_node>            desc_table_stmt
%type <sql_node>            create_index_stmt
%type <sql_node>            drop_index_stmt
%type <sql_node>            sync_stmt
%type <sql_node>            begin_stmt
%type <sql_node>            commit_stmt
%type <sql_node>            rollback_stmt
%type <sql_node>            load_data_stmt
%type <sql_node>            explain_stmt
%type <sql_node>            set_variable_stmt
%type <sql_node>            help_stmt
%type <sql_node>            exit_stmt
%type <sql_node>            command_wrapper
// commands should be a list but I use a single command instead
%type <sql_node>            commands

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | drop_table_stmt
  | show_tables_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:      
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SCF_EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SCF_HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    TRX_BEGIN  {
      $$ = new ParsedSqlNode(SCF_BEGIN);
    }
    ;

commit_stmt:
    TRX_COMMIT {
      $$ = new ParsedSqlNode(SCF_COMMIT);
    }
    ;

rollback_stmt:
    TRX_ROLLBACK  {
      $$ = new ParsedSqlNode(SCF_ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SCF_DROP_TABLE);
      $$->drop_table.relation_name = $3;
      free($3);
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC ID  {
      $$ = new ParsedSqlNode(SCF_DESC_TABLE);
      $$->desc_table.relation_name = $2;
      free($2);
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
    CREATE opt_unique INDEX ID ON ID LBRACE ID_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.unique = $2;
      create_index.index_name = $4;
      create_index.relation_name = $6;
      create_index.attribute_names.swap(*$8);
      free($4);
      free($6);
      delete $8;
    }
    ;

opt_unique:
    {
      $$ = false;
    }
    | UNIQUE {
      $$ = true;
    }

ID_list:
    ID
    {
      $$ = new std::vector<std::string>;
      $$->emplace_back($1);
      free($1);
    }
    | ID COMMA ID_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::string>;
      }
      $$->emplace($$->begin(), $1);
      free($1);
    }
    ;

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SCF_DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
      free($3);
      free($5);
    }
    ;
create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      free($3);

      std::vector<AttrInfoSqlNode> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*$5);
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;
      if ($8 != nullptr) {
        create_table.storage_format = $8;
        free($8);
      }
    }
    ;
attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<AttrInfoSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
    
attr_def:
    ID type LBRACE number RBRACE opt_null
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->nullable = $6;
      free($1);
    }
    | ID type opt_null
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      switch($$->type) {
        case AttrType::INTS:
        case AttrType::CHARS:
        case AttrType::FLOATS:
        case AttrType::DATES: {
          $$->length = 4;
        } break;
        case AttrType::TEXTS: {
          $$->length = 16;
        } break;
        default: {
          $$->length = 4;
        } break;
      }
      $$->nullable = $3;
      free($1);
    }
    ;
opt_null:
    {
      $$ = false;
    }
    | NULLABLE_SYM {
      $$ = true;
    }
    | NULL_SYM {
      $$ = true;
    }
    | NOT NULL_SYM {
      $$ = false;
    }
    ;
number:
    NUMBER {$$ = $1;}
    ;
type:
    INT_T      { $$ = static_cast<int>(AttrType::INTS); }
    | STRING_T { $$ = static_cast<int>(AttrType::CHARS); }
    | FLOAT_T  { $$ = static_cast<int>(AttrType::FLOATS); }
    | DATE_T  { $$ = static_cast<int>(AttrType::DATES); }
    | TEXT_T { $$ = static_cast<int>(AttrType::TEXTS); }
    ;
insert_stmt:        /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES LBRACE value value_list RBRACE 
    {
      $$ = new ParsedSqlNode(SCF_INSERT);
      $$->insertion.relation_name = $3;
      if ($7 != nullptr) {
        $$->insertion.values.swap(*$7);
        delete $7;
      }
      $$->insertion.values.emplace_back(*$6);
      std::reverse($$->insertion.values.begin(), $$->insertion.values.end());
      delete $6;
      free($3);
    }
    ;

value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value value_list  { 
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
value:
    '-' value{
      if($2->attr_type() == AttrType::INTS){
        $$ = new Value(-1 * int($2->get_int()));
      }
      else if($2->attr_type() == AttrType::FLOATS){
        $$ = new Value(-1.0f * float($2->get_float()));
      }
      else{
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("Cannot use negative operator to not number or not float one"));
        YYERROR;
      }
      delete $2;
    }
    | NULL_SYM {
      $$ = new Value;
      *($$) = Value::Null(); /* NULL value */
      @$ = @1;
    }
    | NUMBER {
      $$ = new Value((int)$1);
      @$ = @1;
    }
    | FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    }
    | SSS {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp);
      free(tmp);
      free($1);
    }
    ;
storage_format:
    /* empty */
    {
      $$ = nullptr;
    }
    | STORAGE FORMAT EQ ID
    {
      $$ = $4;
    }
    ;
    
delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID where 
    {
      $$ = new ParsedSqlNode(SCF_DELETE);
      $$->deletion.relation_name = $3;
      if ($4 != nullptr) {
        $$->deletion.conditions.swap(*$4);
        delete $4;
      }
      free($3);
    }
    ;
update_stmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ value where 
    {
      $$ = new ParsedSqlNode(SCF_UPDATE);
      $$->update.relation_name = $2;
      $$->update.attribute_name = $4;
      $$->update.value = *$6;
      if ($7 != nullptr) {
        $$->update.conditions.swap(*$7);
        delete $7;
      }
      free($2);
      free($4);
      delete $6;
    }
    ;
select_stmt:        /*  select 语句的语法解析树*/
    SELECT expression_list FROM table_ref_list where group_by opt_order_by
    {
      $$ = new ParsedSqlNode(SCF_SELECT);
      if ($2 != nullptr) {
        $$->selection.expressions.swap(*$2);
        delete $2;
      }

      if ($4 != nullptr) {
        $$->selection.relations.swap($4->relations);
        $$->selection.conditions.swap($4->conditions);
        delete $4;
      }

      if ($5 != nullptr) {
        $$->selection.conditions.insert($$->selection.conditions.end(), $5->begin(), $5->end());
        delete $5;
      }

      if ($6 != nullptr) {
        $$->selection.group_by.swap(*$6);
        delete $6;
      }

      if ($7 != nullptr) {
        $$->selection.order_sql_nodes.swap(*$7);
        delete $7;
      }
    }
    ;

opt_order_by:
  {
    $$ = nullptr;   // empty
  }
  | ORDER BY order_by_list
  {
    $$ = $3;
  }
  ;

order_by_list:
  order_by
  {
    $$ = new std::vector<OrderSqlNode>;
    $$->emplace_back(*$1);
    delete $1;
  }
  | order_by COMMA order_by_list
  {
    if ($3 != nullptr) {
        $$ = $3;
    }else {
        $$ = new std::vector<OrderSqlNode>;
    }
    $$->emplace($$->begin(), *$1);
    delete $1;
  }
  ;

order_by:
  expression
  {
    if($1 == nullptr || $1->type() != ExprType::UNBOUND_FIELD){
        delete $1;
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: order by field is invalid"));
        YYERROR;
    }
    $$ = new OrderSqlNode();
    $$->inc_order = true;
    $$->unbound_field_expr_ = $1;
    $1 = nullptr;
  }
  | expression ASC
  {
    if($1 == nullptr || $1->type() != ExprType::UNBOUND_FIELD){
        delete $1;
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: order by field is invalid"));
        YYERROR;
    }
    $$ = new OrderSqlNode();
    $$->inc_order = true;
    $$->unbound_field_expr_ = $1;
    $1 = nullptr;
  }
  | expression DESC
  {
   if($1 == nullptr || $1->type() != ExprType::UNBOUND_FIELD){
        delete $1;
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: order by field is invalid"));
        YYERROR;
    }
    $$ = new OrderSqlNode();
    $$->inc_order = false;
    $$->unbound_field_expr_ = $1;
    $1 = nullptr;
  }
  ;

calc_stmt:
    CALC expression_list
    {
      $$ = new ParsedSqlNode(SCF_CALC);
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;

expression_list:
    expression
    {
      $$ = new std::vector<std::unique_ptr<Expression>>;
      $$->emplace_back($1);
    }
    | expression COMMA expression_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::unique_ptr<Expression>>;
      }
      $$->emplace($$->begin(), $1);
    }
    ;
expression:
    '-' expression %prec UMINUS {
      ValueExpr* vepr = new ValueExpr(Value((int)0));
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, vepr, $2, sql_string, &@$);
    }
    | expression '+' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::ADD, $1, $3, sql_string, &@$);
    }
    | expression '-' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, $1, $3, sql_string, &@$);
    }
    | expression '*' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::MUL, $1, $3, sql_string, &@$);
    }
    | expression '/' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::DIV, $1, $3, sql_string, &@$);
    }
    | LBRACE expression RBRACE {
      $$ = $2;
      $$->set_name(token_name(sql_string, &@$));
    }
    | value {
      $$ = new ValueExpr(*$1);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    | rel_attr {
      RelAttrSqlNode *node = $1;
      $$ = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    | '*' {
      $$ = new StarExpr();
    }
    // your code here
    | MAX LBRACE expression RBRACE{
      if($3 -> type() != ExprType::UNBOUND_FIELD){
        delete $3;
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: can only support MAX(FIELD)"));
        YYERROR;
      }else{
        $$ = create_aggregate_expression("MAX", $3, sql_string, &@$);
      }
    }
    | MIN LBRACE expression RBRACE{
      if($3 -> type() != ExprType::UNBOUND_FIELD){
        delete $3;
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: can only support MIN(FIELD)"));
        YYERROR;
      }else{
        $$ = create_aggregate_expression("MIN", $3, sql_string, &@$);
      }
    }
    | SUM LBRACE expression RBRACE{
      if($3 -> type() != ExprType::UNBOUND_FIELD){
        delete $3;
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: can only support SUM(FIELD)"));
        YYERROR;
      }else{
        $$ = create_aggregate_expression("SUM", $3, sql_string, &@$);
      }
    }
    | AVG LBRACE expression RBRACE{
      if($3 -> type() != ExprType::UNBOUND_FIELD){
        delete $3;
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: can only support AVG(FIELD)"));
        YYERROR;
      }else{
        $$ = create_aggregate_expression("AVG", $3, sql_string, &@$);
      }
    }
    | COUNT LBRACE expression RBRACE{
      $$ = create_aggregate_expression("COUNT", $3, sql_string, &@$);
    }
    ;

rel_attr:
    ID {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      free($1);
    }
    | ID DOT ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      free($1);
      free($3);
    }
    ;

relation:
    ID {
      $$ = $1;
    }
    ;
table_ref_list:
    comma_ref_list {  // 返回逗号连接的表列表
      $$ = $1;
    }
    | join_ref_list { // 返回 INNER JOIN 的表列表
      $$ = $1;
    }
    ;
comma_ref_list:
    relation {
      $$ = new TableRefSqlNode();
      $$->relations.push_back($1);
      free($1);
    }
    | relation COMMA table_ref_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new TableRefSqlNode();
      }

      $$->relations.insert($$->relations.begin(), $1);
      free($1);
    }
    ;
join_ref_list:
    relation INNER JOIN relation ON condition_list {
      $$ = new TableRefSqlNode();

      $$->relations.push_back($1);  // 左侧的relation
      $$->relations.push_back($4);  // 右侧的relation
      free($1);
      free($4);

      // 将ON条件存储为 conditions
      if ($6 != nullptr) {
        $$->conditions.insert($$->conditions.end(), $6->begin(), $6->end());
        delete $6;
      }
    }
    | join_ref_list INNER JOIN relation ON condition_list {
      // 处理嵌套的 INNER JOIN
      if ($1 != nullptr) {
        $$ = $1;  // 左侧的rel_list
      } else {
        $$ = new TableRefSqlNode();
      }

      $$->relations.push_back($4);  // 右侧的relation
      free($4);

      // 将ON条件存储为 conditions
      if ($6 != nullptr) {
        $$->conditions.insert($$->conditions.end(), $6->begin(), $6->end());
        delete $6;
      }
    }
    ;

where:
    /* empty */
    {
      $$ = nullptr;
    }
    | WHERE condition_list {
      $$ = $2;  
    }
    ;
condition_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | condition {
      $$ = new std::vector<ConditionSqlNode>;
      $$->emplace_back(*$1);
      delete $1;
    }
    | condition AND condition_list {
      $$ = $3;
      $$->emplace_back(*$1);
      delete $1;
    }
    ;
condition:
     expression comp_op expression
     {
          $$ = new ConditionSqlNode;
          // 说明是 () op () 型的算数表达式,$1类型为 ArithmeticExpr*

          if($1->type() == ExprType::ARITHMETIC){
              ArithmeticExpr* a1 = static_cast<ArithmeticExpr*> ($1);
              $$ -> left_arith_exper = a1;
              $$ -> left_is_value = 0;
              $$ -> left_is_attr = 0;
          }else if($1->type() == ExprType::VALUE){
              ValueExpr* ve = static_cast<ValueExpr*>($1);
              $$->left_is_value = 1;
              $$->left_is_attr  = 0;
              $$->left_value = ve->get_value();
              delete $1;
          }else if($1->type() == ExprType::UNBOUND_FIELD){
              UnboundFieldExpr* ufe = static_cast<UnboundFieldExpr*>($1);
              RelAttrSqlNode node = RelAttrSqlNode();
              node.relation_name = ufe->table_name();
              node.attribute_name = ufe->field_name();

              $$->left_is_value = 0;
              $$->left_is_attr  = 1;
              $$->left_attr = node;
              delete $1;
          }

          if($3->type() == ExprType::ARITHMETIC){
              ArithmeticExpr* a2 = static_cast<ArithmeticExpr*> ($3);
              $$ -> right_arith_exper = a2;
              $$ -> right_is_value = 0;
              $$ -> right_is_attr = 0;
          }
          else if($3->type() == ExprType::VALUE){
              ValueExpr* ve = static_cast<ValueExpr*>($3);
              $$->right_is_value = 1;
              $$->right_is_attr  = 0;
              $$->right_value = ve->get_value();

              delete $3;
          }
          else if($3->type() == ExprType::UNBOUND_FIELD){
              UnboundFieldExpr* ufe = static_cast<UnboundFieldExpr*>($3);
              RelAttrSqlNode node = RelAttrSqlNode();
              node.relation_name = ufe->table_name();
              node.attribute_name = ufe->field_name();

              $$->right_is_value = 0;
              $$->right_is_attr  = 1;
              $$->right_attr = node;
              delete $3;
          }

          $$->comp = $2;
    }
    | rel_attr IS_SYM NULL_SYM
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->right_is_attr = 0;
      $$->right_value.set_null();
      $$->comp = IS_NULL_OP;

      delete $1;
    }
    | rel_attr IS_SYM NOT NULL_SYM
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->right_is_attr = 0;
      $$->right_value.set_null();
      $$->comp = NOT_NULL_OP;

      delete $1;
    }
    | value IS_SYM NULL_SYM
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      $$->right_is_attr = 0;
      $$->right_value.set_null();
      $$->comp = IS_NULL_OP;

      delete $1;
    }
    | value IS_SYM NOT NULL_SYM
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      $$->right_is_attr = 0;
      $$->right_value.set_null();
      $$->comp = NOT_NULL_OP;

      delete $1;
    }
    ;

comp_op:
      EQ { $$ = EQUAL_TO; }
    | LT { $$ = LESS_THAN; }
    | GT { $$ = GREAT_THAN; }
    | LE { $$ = LESS_EQUAL; }
    | GE { $$ = GREAT_EQUAL; }
    | NE { $$ = NOT_EQUAL; }
    | LIKE { $$ = LIKE_OP; }
    | NOT LIKE { $$ = NO_LIKE_OP; }
    ;

// your code here
group_by:
    /* empty */
    {
      $$ = nullptr;
    }
    | GROUP BY expression_list
    {
        $$ = $3;
    }
    ;
load_data_stmt:
    LOAD DATA INFILE SSS INTO TABLE ID 
    {
      char *tmp_file_name = common::substr($4, 1, strlen($4) - 2);
      
      $$ = new ParsedSqlNode(SCF_LOAD_DATA);
      $$->load_data.relation_name = $7;
      $$->load_data.file_name = tmp_file_name;
      free($7);
      free(tmp_file_name);
    }
    ;

explain_stmt:
    EXPLAIN command_wrapper
    {
      $$ = new ParsedSqlNode(SCF_EXPLAIN);
      $$->explain.sql_node = std::unique_ptr<ParsedSqlNode>($2);
    }
    ;

set_variable_stmt:
    SET ID EQ value
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      $$->set_variable.name  = $2;
      $$->set_variable.value = *$4;
      free($2);
      delete $4;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
