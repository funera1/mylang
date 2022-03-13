#include "interpreter.hpp"

void intp_assign(statement_node* node){
    auto assign_node = node->assign_node;
    string name = assign_node->name;
    // TODO: int以外の拡張どうする？
    int value_int = calc_expr_node(assign_node->expr_node);
    if(!is_exist_in_variable_table(name)){
        cout << "まだ" << name << "は登録されていません" << endl;
        assert(0);
    }
    assign_value_int(name, value_int);
    // DEBUG
    cout << "DEBUG: " << name << " is " << value_int << endl;
}