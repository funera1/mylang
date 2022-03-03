#pragma once
#include "../tools.cpp"
#include "../RST.cpp"

typedef struct expr_node {
    string token;
    int value;
    char op;
    struct expr_node* left_node;
    struct expr_node* right_node;
} expr_node;

expr_node* init_expr_node(string token, int value, char op){
    expr_node* node = new expr_node;
    node->token = token;
    node->value = value;
    node->op = op;
    node->left_node = nullptr;
    node->right_node = nullptr;
    return node;
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