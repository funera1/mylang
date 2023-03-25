#include "ast.hpp"
#include "ast_expr.cpp"

typedef struct while_node {
    // 条件式
    expr_node* conditional_expr_node;
    // 本文
    statement_node* statment_node;
} while_node;

while_node* init_while_node(expr_node* expr_node, statement_node* statement_node){
    while_node* node = new while_node;
    node->conditional_expr_node = expr_node;
    node->statment_node = statement_node;
    return node;
}

while_node* construct_while_node(nonterm_node*& node){
    // WHILE_STATEMENT
    nonterm_node* tmp_node = node;
    // WHILE 
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
    statement_node* statement_node = rst_to_ast(tmp_node);
    // RBRACKET
    tmp_node = get_adjacent_node(tmp_node, "right");
    node = tmp_node;
    
    while_node* while_node = init_while_node(cond_expr_node, statement_node);
    return while_node;
}