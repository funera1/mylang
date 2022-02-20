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


// RSTをASTに変換する
void RST_to_AST(node_t* root){
    // ASTを走査する
    node_t* now_node = root;
    statement_node* statement_root;
    while(1){
        if(is_statement(now_node->token))
    }
}