#pragma once
#include "../include.cpp"
#include "../global_values.hpp"
#include "../AST.cpp"

struct variable_info {
    string type;
    int value_int;
};

map<string, variable_info> variable_table;

// すでに変数が存在するかどうか
bool is_exist_in_variable_table(string name){
    return ( variable_table.find(name) != variable_table.end() );
}
variable_info init_variable_info(){
    variable_info v_info;
    v_info.type = "";
    v_info.value_int = 0;
    return v_info;
}
void register_variable(string type, string name){
    auto v_info = init_variable_info();
    v_info.type = type;
    variable_table[name] = v_info;
}
void assign_value_int(string name, int value){
    auto v_info = variable_table[name];
    v_info.value_int = value;
}
void interpreter(statement_node* root){
    // ASTを走査
    auto node = root;
    while(node = get_next_statement_node(node)){
        if(node->token == "DECLARATION_STATEMENT"){
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
        if(node->token == "ASSIGN_STATEMENT"){
            auto assign_node = node->assign_node;
            string name = assign_node->name;
            // TODO: int以外の拡張どうする？
            int value_int = calc_expr_node(assign_node->expr_node);
            if(!is_exist_in_variable_table(name)){
                cout << "まだ" << name << "は登録されていません" << endl;
                assert(0);
            }
            assign_value_int(name, value_int);
            // テスト
            cout << "DEBUG: " << name << " is " << value_int << endl;
        }
    }
}