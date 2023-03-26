#include "ast.hpp"

typedef struct assign_node {
    string name;
    struct expr_node* expr_node;
} assign_node;

assign_node* init_assign_node(string name, expr_node* expr_node = nullptr){
    assign_node* node = new assign_node;
    node->name = name;
    node->expr_node = expr_node;
    return node;
}

// 引数nodeのnode->tokenはASSIGN_STATEMENT
// ASSIGN_STATEMENT
// ID EXPR SEMICOLONの構造
assign_node* construct_assign_node(nonterm_node* node){
    string name = "";
    expr_node* expr_node_ = nullptr;
    nonterm_node* tmp_node = node;
    // ID
    tmp_node = get_adjacent_node(tmp_node, "child");
    name = get_id_from_nonterm_node(tmp_node);
    // expr
    tmp_node = get_adjacent_node(tmp_node, "right"); // ASIGN
    tmp_node = get_adjacent_node(tmp_node, "right"); // EXPR
    expr_node_ = construct_expr_node(tmp_node);
    assign_node* assign_node_ = init_assign_node(name, expr_node_);
    return assign_node_;
}