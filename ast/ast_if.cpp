#include "AST.hpp"
#include "ast_expr.cpp"

typedef struct if_node {
    // 条件式
    expr_node* conditional_expr_node;
    // 本文
    statement_node* statment_node;
} if_node;

if_node* init_if_node(expr_node* expr_node, statement_node* statement_node){
    if_node* node = new if_node;
    node->conditional_expr_node = expr_node;
    node->statment_node = statement_node;
    return node;
}

if_node* construct_if_node(nonterm_node*& node){
    // IF_STATEMENT
    nonterm_node* tmp_node = node;
    // IF
    tmp_node = get_adjacent_node(tmp_node, "child");
    // LPAREN
    tmp_node = get_adjacent_node(tmp_node, "right");
    // EXPR
    tmp_node = get_adjacent_node(tmp_node, "right");
    expr_node* cond_expr_node = construct_expr_node(tmp_node);
    // RPAREN
    tmp_node = get_adjacent_node(tmp_node, "right");
    // LBRACKET
    tmp_node = get_adjacent_node(tmp_node, "right");
    // COMPOUND
    tmp_node = get_adjacent_node(tmp_node, "right");
    statement_node* statement_node = RST_to_AST(tmp_node);
    // RBRACKET
    tmp_node = get_adjacent_node(tmp_node, "right");
    node = tmp_node;
    
    if_node* if_node = init_if_node(cond_expr_node, statement_node);
    return if_node;
}