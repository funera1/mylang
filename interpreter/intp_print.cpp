#include "interpreter.hpp"

void intp_print(statement_node* node){
    auto print_node = node->print_node;
    int value_int = calc_expr_node(print_node->__expr_node);
    cout << value_int << endl;
}