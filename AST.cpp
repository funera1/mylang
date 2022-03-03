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
        assert(state_node->assign_node != nullptr);
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
        // まずrp_iをstackに入れる
        // rp_iがop
        // opの時はstackから2つ取り出して子にする
        // そしてstackに入れる
        if(is_in_set(op_set, rp_i)){
            char op = rp_i[0];
            expr_node* node = init_expr_node("op", 0, op);
            expr_node* right_node = expr_node_stack.top();
            expr_node_stack.pop();
            expr_node* left_node = expr_node_stack.top();
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
    // cout << endl << "### CONSTRUCT EXPR NODE ###" << endl;
    // cout << root_nonterm_node->token << endl;
    node = get_next_node(node, root_nonterm_node);
    while(root_nonterm_node != node){
        // cout << node->token << endl;
        if(node->term_node != nullptr){
            term_node* term_node = node->term_node;
            // cout << "in term: term is ";
            // number
            if(term_node->token == "#id"){
                // cout << term_node->id << endl;
                formula.push_back(term_node->id);
            }
            // op
            else if(term_node->token == "#number"){
                // cout << term_node->number << endl;
                formula.push_back(to_string(term_node->number));
            }
            else {
                // cout << term_node->token << endl;
                formula.push_back(term_node->token);
            }
        }
        node = get_next_node(node, root_nonterm_node);
    }
    // DEBUG
    // cout << endl << "### formula is check ###" << endl;
    // for(auto fi : formula)cout << fi << " ";
    // cout << endl << "### end ###" << endl;
    
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
    // expr
    tmp_node = get_adjacent_node(tmp_node, "right"); // ASIGN
    tmp_node = get_adjacent_node(tmp_node, "right"); // EXPR
    expr_node_ = construct_expr_node(tmp_node);
    assign_node* assign_node_ = init_assign_node(name, expr_node_);
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
    // ID
    tmp_node = get_adjacent_node(tmp_node, "right");
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
    while(root_nonterm_node != now_nonterm_node){
        // nodeがstatement関連ならstatement_nodeを伸ばす
        if(is_statement(now_nonterm_node->token)){
            // DEBUG
            // cout << "statement is " << now_nonterm_node->token << endl;
            statement_node* next_statement_node = init_statement_node(now_nonterm_node);
            now_statement_node->next_statement_node = next_statement_node;
            now_statement_node = next_statement_node;
        }
        now_nonterm_node = get_next_node(now_nonterm_node);
    }
    cout << "AST作成完了" << endl;
    return root_statement_node;
}

void watch_expr_node(expr_node* node){
    if(node == nullptr){
        cout << endl;
        return;
    }
    if(node->token == "number")cout << node->value << endl;
    if(node->token == "op")cout << node->op << endl;
    cout << "left is ";
    watch_expr_node(node->left_node);
    cout << "right is ";
    watch_expr_node(node->right_node);
    cout << "end" << endl;
}

void all_watch_AST(statement_node* node){
    cout << "### AST ###" << endl;
    while(node != nullptr){
        cout << node->token << endl;
        if(node->token == "ASSIGN_STATEMENT"){
            auto a = node->assign_node;
            cout << "name is " << a->name << endl;
            watch_expr_node(a->expr_node);
        }
        if(node->token == "DECLARATION_STATEMENT"){
            auto d = node->declaration_node;
            cout << "type is " << d->type << endl;
            cout << "name is " << d->name << endl;
        }
        if(node->next_statement_node == nullptr)break;
        node = node->next_statement_node;
    }
    cout << "### END ###" << endl;
}

statement_node* get_next_statement_node(statement_node* node){
    if(node->next_statement_node == nullptr)return nullptr;
    return node->next_statement_node;
}

// 逆ポーランドなのでdfsで行けそう
int calc_expr_node(expr_node* node){
    int res = 0;
    if(node->op == '+'){
        res = calc_expr_node(node->left_node) + calc_expr_node(node->right_node);
    }
    else if(node->op == '-'){
        res = calc_expr_node(node->left_node) - calc_expr_node(node->right_node);
    }
    else if(node->op == '*'){
        res = calc_expr_node(node->left_node) * calc_expr_node(node->right_node);
    }
    else if(node->op == '/'){
        // ゼロ除算禁止
        if(calc_expr_node(node->right_node) == 0){
            cout << "ゼロ除算!!" << endl;
            assert(0);
        }
        res = calc_expr_node(node->left_node) / calc_expr_node(node->right_node);
    }
    else {
        return node->value;
    }
    return res;
}