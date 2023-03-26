#include "ast.hpp"
#include "ast_expr.cpp"

typedef struct print_node {
    expr_node* __expr_node;
} print_node;

print_node* init_print_node(expr_node* expr_node){
    print_node* node = new print_node;
    node->__expr_node = expr_node;
    return node;
}

print_node* construct_print_node(nonterm_node* node){
    nonterm_node* tmp_node = node;

    // PRINT
    tmp_node = get_adjacent_node(tmp_node, "child");
    // LPAREN
    tmp_node = get_adjacent_node(tmp_node, "right");
    // EXPR
    tmp_node = get_adjacent_node(tmp_node, "right");
    expr_node* __expr_node = construct_expr_node(tmp_node);

    print_node* print_node = init_print_node(__expr_node);
    return print_node;
}