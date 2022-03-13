#pragma once
#include "../include.cpp"
#include "../global_values.hpp"
#include "../ast/AST.cpp"
#include "intp_declaration.cpp"
#include "intp_assign.cpp"
#include "intp_if.cpp"
#include "intp_for.cpp"

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
    assert(variable_table[name].type == "INT");
    variable_table[name].value_int = value;
}
int get_variable_value_int(string id){
    if(variable_table[id].type != "INT"){
        cout << variable_table[id].type << "型は数値計算できません" << endl;
        assert(0);
    }
    return variable_table[id].value_int;
}
void interpreter(statement_node* root){
    // ASTを走査
    auto node = root;
    while(node = get_next_statement_node(node)){
        if(node->token == "DECLARATION_STATEMENT"){
            intp_declaration(node);
        }
        if(node->token == "ASSIGN_STATEMENT"){
            intp_assign(node);
        }
        if(node->token == "IF_STATEMENT"){
            intp_if(node);
        }
        if(node->token == "FOR_STATEMENT"){
            intp_for(node);
        }
    }
}