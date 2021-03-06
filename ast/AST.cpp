#pragma once
#include "AST.hpp"
#include "ast_expr.cpp"
#include "ast_assign.cpp"
#include "ast_declaration.cpp"
#include "ast_if.cpp"
#include "ast_while.cpp"
#include "ast_print.cpp"

typedef struct statement_node {
    string token;
    struct statement_node* next_statement_node;
    struct assign_node* assign_node;
    struct declaration_node* declaration_node;
    struct if_node* if_node;
    struct while_node* while_node;
    struct print_node* print_node;
} statement_node;

// 各種ノードの初期化関数

statement_node* init_root_statement_node(){
    statement_node* state_node = new statement_node; 
    state_node->token = "ROOT";
    state_node->next_statement_node = nullptr;
    state_node->assign_node = nullptr;
    state_node->declaration_node = nullptr;
    state_node->if_node = nullptr;
    state_node->while_node = nullptr;
    state_node->print_node = nullptr;
    return state_node;
}

statement_node* init_statement_node(nonterm_node*& nonterm_node){
    string token = nonterm_node->token;
    statement_node* state_node = new statement_node;
    state_node->token = token;
    state_node->next_statement_node = nullptr;
    state_node->assign_node = nullptr;
    state_node->declaration_node = nullptr;
    state_node->if_node = nullptr;
    state_node->while_node = nullptr;
    state_node->print_node = nullptr;

    if(token == "ASSIGN_STATEMENT"){
        state_node->assign_node = construct_assign_node(nonterm_node);
        assert(state_node->assign_node != nullptr);
    }
    if(token == "DECLARATION_STATEMENT"){
        state_node->declaration_node = construct_declaration_node(nonterm_node);
    }
    if(token == "IF_STATEMENT"){
        state_node->if_node = construct_if_node(nonterm_node);
    }
    if(token == "WHILE_STATEMENT"){
        state_node->while_node = construct_while_node(nonterm_node);
    }
    if(token == "PRINT_STATEMENT"){
        state_node->print_node = construct_print_node(nonterm_node);
    }
    return state_node;
}

// cstをASTに変換する
// statement_node* cst_to_AST(nonterm_node* root_nonterm_node){
//     // ASTを走査する
//     nonterm_node* now_nonterm_node = root_nonterm_node;
//     statement_node* root_statement_node = init_root_statement_node();
//     statement_node* now_statement_node = root_statement_node;
//     now_nonterm_node = get_next_node(now_nonterm_node);
//     while(root_nonterm_node != now_nonterm_node){
//         // nodeがstatement関連ならstatement_nodeを伸ばす
//         if(is_statement(now_nonterm_node->token)){
//             // DEBUG
//             // cout << "statement is " << now_nonterm_node->token << endl;
//             statement_node* next_statement_node = init_statement_node(now_nonterm_node);
//             now_statement_node->next_statement_node = next_statement_node;
//             now_statement_node = next_statement_node;
//         }
//         now_nonterm_node = get_next_node(now_nonterm_node, root_nonterm_node);
//     }
//     cout << "AST作成完了" << endl;
//     return root_statement_node;
// }
// cstをastに変換する
vector<string> convert_cst_to_ast(vector<string> cst){
    // cstを走査する
    // nonterm_node* now_nonterm_node = root_nonterm_node;
    // statement_node* root_statement_node = init_root_statement_node();
    // statement_node* now_statement_node = root_statement_node;
    // now_nonterm_node = get_next_node(now_nonterm_node);
    // while(root_nonterm_node != now_nonterm_node){
    //     // nodeがstatement関連ならstatement_nodeを伸ばす
    //     if(is_statement(now_nonterm_node->token)){
    //         // DEBUG
    //         // cout << "statement is " << now_nonterm_node->token << endl;
    //         statement_node* next_statement_node = init_statement_node(now_nonterm_node);
    //         now_statement_node->next_statement_node = next_statement_node;
    //         now_statement_node = next_statement_node;
    //     }
    //     now_nonterm_node = get_next_node(now_nonterm_node, root_nonterm_node);
    // }
    vector<string> ast;
    for(int ci = 0; ci < sz(cst); ci++){
        auto cst_i = cst[ci];
        if(cst_i == "DECLARATION_STATEMENT"){
            // DECLARATION_STATEMENT [[ hoge ]]となっているのでhogeの部分のみ走査する
            int nest_count = 0;
            while(ci++){
                // hoge部分の走査
                if(cst[ci] == "[[")nest_count++;
                if(cst[ci] == "]]")nest_count--;
                if(nest_count == 0)break;

            }
        }

    }
    return ast;
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
