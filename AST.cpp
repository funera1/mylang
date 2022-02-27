#pragma once
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
    struct expr_node* expr_node;
} assign_node;

typedef struct declaration_node {
    string type;
    string name;
} declaration_node;

typedef struct statement_node {
    string token;
    struct statement_node* next_statement_node;
    struct assign_node* assign_node;
    struct declaration_node* declaration_node;
} statement_node;

// 宣言
assign_node* construct_assign_node(nonterm_node* node);
declaration_node* construct_declaration_node(nonterm_node* node);

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

statement_node* init_root_statement_node(){
    statement_node* state_node = new statement_node; 
    state_node->token = "ROOT";
    state_node->next_statement_node = nullptr;
    state_node->assign_node = nullptr;
    state_node->declaration_node = nullptr;
    return state_node;
}
statement_node* init_statement_node(nonterm_node* nonterm_node){
    string token = nonterm_node->token;
    statement_node* state_node = new statement_node;
    state_node->token = token;
    state_node->next_statement_node = nullptr;
    state_node->assign_node = nullptr;
    state_node->declaration_node = nullptr;

    if(token == "ASSIGN_STATEMENT"){
        state_node->assign_node = construct_assign_node(nonterm_node);
    }
    if(token == "DECLARATION_STATEMENT"){
        state_node->declaration_node = construct_declaration_node(nonterm_node);
    }
    return state_node;
}

// 逆ポーランド法を引数に与えて木を返す
expr_node* reverse_polish_to_tree(vector<string> reverse_polish){
    // DEBUG
    cout << "### start reverse polis to tree ###" << endl;
    stack<expr_node*> expr_node_stack;
    set<string> op_set = {"+", "-", "*", "/"};
    for(auto rp_i : reverse_polish){
        cout << rp_i << endl;
        // まずrp_iをstackに入れる
        // rp_iがop
        // opの時はstackから2つ取り出して子にする
        // そしてstackに入れる
        if(is_in_set(op_set, rp_i)){
            char op = rp_i[0];
            expr_node* node = init_expr_node("op", 0, op);
            expr_node* left_node = expr_node_stack.top();
            expr_node_stack.pop();
            expr_node* right_node = expr_node_stack.top();
            expr_node_stack.pop();

            node->left_node = left_node;
            node->right_node = right_node;
            expr_node_stack.push(node);
        }
        // rp_iがnumber
        else {
            int number = to_num(rp_i);
            expr_node* node = init_expr_node("number", number, '\0');
            expr_node_stack.push(node);
        }
    }
    // 最後は一つになるはず
    assert(expr_node_stack.size() == 1);
    cout << "### end reverse polis to tree ###" << endl;
    return expr_node_stack.top();
}

// 各種statementのノード構築
expr_node* construct_expr_node(nonterm_node* node){
    // nonterm_nodeを走査して、数式(中値記法)を作る
    nonterm_node* root_nonterm_node = node;
    vector<string> formula;
    cout << endl << "### CONSTRUCT EXPR NODE ###" << endl;
    cout << root_nonterm_node->token << endl;
    node = get_next_node(node, root_nonterm_node);
    while(root_nonterm_node != node){
        if(node->term_node != nullptr){
            cout << "in term" << endl;
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
        node = get_next_node(node, root_nonterm_node);
    }
    // DEBUG
    cout << endl << "### formula is check ###" << endl;
    for(auto fi : formula)cout << fi << " ";
    cout << endl << "### end ###" << endl;
    
    // 中値記法を逆ポーランド記法に変換する
    vector<string> reverse_polish = convert_reverse_polish(formula);
    
    // 逆ポーランドを木にする
    expr_node* root_expr_node = reverse_polish_to_tree(reverse_polish);
    return root_expr_node;
}

string get_id_from_nonterm_node(nonterm_node* node){
    if(node->term_node == nullptr){
        cout << "idはありません" << endl;
        return "";
    }
    term_node* term_node = node->term_node;
    return term_node->id;
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
    cout << "name is OK" << endl;
    // expr
    tmp_node = get_adjacent_node(tmp_node, "right"); // ASIGN
    cout << "assign is OK" << endl;
    tmp_node = get_adjacent_node(tmp_node, "right"); // EXPR
    cout << "expr is OK" << endl;
    expr_node_ = construct_expr_node(tmp_node);
    assign_node* assign_node_ = init_assign_node(name, expr_node_);
    cout << "init_assign_node is OK" << endl;
    return assign_node_;
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
    cout << "type is OK" << endl;
    // ID
    tmp_node = get_adjacent_node(tmp_node, "right");
    cout << "name is OK" << endl;
    // IDやNUMBERなどの実値をRSTと結び付けられてない
    name = get_id_from_nonterm_node(tmp_node);
    declaration_node* declaration_node = init_declaration_node(type, name);
    return declaration_node;
}

// RSTをASTに変換する
statement_node* RST_to_AST(nonterm_node* root_nonterm_node){
    // ASTを走査する
    nonterm_node* now_nonterm_node = root_nonterm_node;
    statement_node* root_statement_node = init_root_statement_node();
    statement_node* now_statement_node = root_statement_node;
    now_nonterm_node = get_next_node(now_nonterm_node);
    cout << now_nonterm_node->token << endl;
    while(root_nonterm_node != now_nonterm_node){
        cout << now_nonterm_node->token << endl;
        // nodeがstatement関連ならstatement_nodeを伸ばす
        if(is_statement(now_nonterm_node->token)){
            // DEBUG
            cout << "statement is " << now_nonterm_node->token << endl;
            statement_node* next_statement_node = init_statement_node(now_nonterm_node);
            now_statement_node->next_statement_node = next_statement_node;
            now_statement_node = next_statement_node;
        }
        now_nonterm_node = get_next_node(now_nonterm_node);
    }
    cout << "AST作成完了" << endl;
    return root_statement_node;
}

void all_watch_AST(statement_node* node){
    while(node->next_statement_node != nullptr){
        cout << node->token << endl;
        node = node->next_statement_node;
    }
}