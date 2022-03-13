#include "interpreter.hpp"

void intp_while(statement_node* node){
    auto if_node = node->if_node;
    while(1){
        int value = calc_expr_node(if_node->conditional_expr_node);
        // 条件式の中身が1以上のときのみ実行する
        if(value > 0){
            interpreter(if_node->statment_node);
        }
        else {
            break;
        }
    }
}