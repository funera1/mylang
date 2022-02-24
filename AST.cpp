#include "tools.cpp"
#include "RST.cpp"

// 各種ノード
typedef struct expr_node {
    string token;
    int value;
    char op;
    struct expr_node* left_node;
    struct expr_node* right_node;
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
    struct statement_node* next_statement_node;
    assign_node* assign_node;
    declaration_node* declaration_node;
} statement_node;

// 各種ノードの初期化関数
expr_node* init_expr_node(string token, int value, char op){
    expr_node* node = new expr_node;
    node->token = token;
    node->value = value;
    node->op = op;
    node->left_node = nullptr;
    node->right_node = nullptr;
    return node;
}

assign_node* init_assign_node(string name, expr_node* expr_node = nullptr){
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

statement_node* init_statement_node(string statement_kind){
    statement_node* node = new statement_node;
    node->statement_kind = statement_kind;
    node->next_statement_node = nullptr;
    node->assign_node = nullptr;
    node->declaration_node = nullptr;
    return node;
}

// 各種statementのノード構築
expr_node* construct_expr_node(nonterm_node* node){
    // nonterm_nodeを走査して、数式(中値記法)を作る
    nonterm_node* root = node;
    vector<string> formula;
    node = get_next_node(node);
    while(root != node){
        if(node->term_node != nullptr){
            term_node* term_node = node->term_node;
            // number
            if(term_node->token == "#id"){
                formula.push_back(to_string(term_node->number));
            }
            // op
            else {
                formula.push_back(term_node->token);
            }
        }
        node = get_next_node(node);
    }
    
    // 中値記法を逆ポーランド記法に変換する
    vector<string> reverse_polish = convert_reverse_polish(formula);
    
    // 逆ポーランドを木にする
}

// 引数nodeのnode->tokenはASSIGN_STATEMENT
// ASSIGN_STATEMENT
// ID EXPR SEMICOLONの構造
assign_node* construct_assign_node(nonterm_node* node){
    string name = "";
    expr_node* expr_node = nullptr;
    nonterm_node* tmp_node = node;
    // ID
    tmp_node = get_adjacent_node(tmp_node, "child");
    name = get_id_from_nonterm_node(tmp_node);
    // expr
    tmp_node = get_adjacent_node(tmp_node, "left"); // ASIGN
    tmp_node = get_adjacent_node(tmp_node, "left"); // EXPR
    expr_node* expr_node = construct_expr_node(tmp_node);
    assign_node* assign_node = init_assign_node(name, expr_node);
    return assign_node;
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
    tmp_node = get_adjacent_node(tmp_node, "left");
    // IDやNUMBERなどの実値をRSTと結び付けられてない
    name = get_id_from_nonterm_node(tmp_node);

    declaration_node* declaration_node = init_declaration_node(type, name);
    return declaration_node;
}

// RSTをASTに変換する
void RST_to_AST(nonterm_node* root){
    // ASTを走査する
    nonterm_node* now_node = root;
    statement_node* statement_node = init_statement_node("ROOT");
    while(1){
        // nodeがstatement関連ならstatement_nodeを伸ばす
        if(is_statement(now_node->token)){
            auto next_statement_node = init_statement_node(now_node->token);
            statement_node->next_statement_node = next_statement_node;
        }
    }
}