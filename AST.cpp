#include "tools.cpp"
#include "RST.cpp"

typedef struct expr_node {
    int value;
    char op;
    struct expr_node* next_node;
} expr_node;

typedef struct assign_node {
    string name;
    expr_node* expr_node;
} assign_node;

typedef struct declaration_node {
    string type;
    string name;
} declaration_node;

typedef struct statement_node {
    string statement_kind;
} statement_node;

expr_node* init_expr_node(int value, char op){
    expr_node* node = new expr_node;
    node->value = value;
    node->op = op;
    node->next_node = NULL;
    return node;
}

assign_node* init_assign_node(string name, expr_node* expr_node = NULL){
    assign_node* node = new assign_node;
    node->name = name;
    node->expr_node = expr_node;
    return node;
}

declaration_node* init_declaration_node(string type, string name){
    declaration_node* node = new declaration_node;
    node->type = type;
    node->name = name;
    return node;
}

// RSTをASTに変換する
void RST_to_AST(node_t* root){
    // ASTを走査する
    node_t* now_node = root;
    statement_node* statement_root;
    while(1){
        // nodeがstatement関連ならstatement_nodeを伸ばす
        if(is_statement(now_node->token)){

        }
    }
}