#include "interpreter.hpp"

void intp_declaration(nonterm_node* node){
    auto declaration_node = node->declaration_node;
    string type = declaration_node->type;
    string name = declaration_node->name;
    // 変数テーブルにすでに同名の変数が存在するかチェック
    // 存在したらアウト
    if(is_exist_in_variable_table(name)){
        cout << name << "はすでに登録されています" << endl;
        assert(0);
    }
    else {
        register_variable(type, name);
    }
}