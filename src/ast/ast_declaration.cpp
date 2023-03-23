#include "AST.hpp"

typedef struct declaration_node {
    string type;
    string name;
} declaration_node;

declaration_node* init_declaration_node(string type, string name){
    declaration_node* node = new declaration_node;
    node->type = type;
    node->name = name;
    return node;
}

// 引数nodeのnode->tokenはDECLARATION_STATEMENT
// DECLARATION_STATEMEN
// INT ID SEMICOLON の構造になっている
declaration_node* construct_declaration_node(nonterm_node* node){
    string type = "";
    string name = "";
    nonterm_node* tmp_node = node;

    // INT
    tmp_node = get_adjacent_node(tmp_node, "child");
    type = tmp_node->token;
    // ID
    tmp_node = get_adjacent_node(tmp_node, "right");
    // IDやNUMBERなどの実値をRSTと結び付けられてない
    name = get_id_from_nonterm_node(tmp_node);
    declaration_node* declaration_node = init_declaration_node(type, name);
    return declaration_node;
}