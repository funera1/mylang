#include "interpreter.hpp"

void intp_while(statement_node* node){
    auto while_node = node->while_node;
    while(1){
        int value = calc_expr_node(while_node->conditional_expr_node);
        // 条件式の中身が1以上のときのみ実行する
        if(value > 0){
            interpreter(while_node->statment_node);
        }
        else {
            break;
        }
    }
}