#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "parsing.cpp"

typedef struct term_node {
    string token;

    int number;
    string id;
} term_node;

typedef struct nonterm_node {
    // token
    string token;

    struct nonterm_node* parent_node;
    struct nonterm_node* child_node;
    struct nonterm_node* right_node;
    struct nonterm_node* left_node;
    struct term_node* term_node;
} nonterm_node;

nonterm_node* init_nonterm_node(string token){
    nonterm_node* node;
    node = new nonterm_node;
    node->token = token;

    node->parent_node = nullptr;
    node->child_node = nullptr;
    node->right_node = nullptr;
    node->left_node = nullptr;
    node->term_node = nullptr;

    return node;
}

term_node* init_term_node(string token, int number, string id){
    term_node* node;
    node = new term_node;
    node->token = token;
    node->number = number;
    node->id = id;
    
    return node;
}

void connect_parent_and_child(nonterm_node* parent_node, nonterm_node* child_node){
    parent_node->child_node = child_node;
    child_node->parent_node = parent_node;
}

void connect_brother_and_brother(nonterm_node* left_node, nonterm_node* right_node){
    left_node->right_node = right_node;
    right_node->left_node = left_node;
}

// A->abcという変換があったとき、abcを兄弟としたグラフを構築し、aに当たるノードを返す
nonterm_node* connect_brothers(vector<string> vs){
    vector<nonterm_node*> nodes(sz(vs));
    // それぞれに対応するノードを構成
    for(int i = 0; i < sz(nodes); i++){
        nodes[i] = init_nonterm_node(vs[i]);
    }
    // 隣り合う兄弟間を辺でつなぐ
    for(int i = 0; i < sz(nodes)-1; i++){
        connect_brother_and_brother(nodes[i], nodes[i+1]);
    }
    // 一番最初のノードを返す
    return nodes[0];
}

void release_node(nonterm_node* node){
    if(node->parent_node != nullptr || node->child_node != nullptr | node->right_node != nullptr || node->left_node != nullptr){
        cout << "child_node or borhter_node is not released" << endl;
        assert(0);
    }
    free(node);
    // ダブルフリーを防ぐ
    node = nullptr;
}

// どの子からでも親に移動する関数. 返り値は親のノード
nonterm_node* get_parent_node(nonterm_node* node){
    // DEBUG
    // cout << "IN GET_PARENT_NODE: " << node->token << endl;
    // 一番上まで来たら終了
    if(node->token == "PROGRAM")return node;
    // 左のノードがないとき
    if(node->left_node == nullptr){
        return node->parent_node;
    }
    else {
        return get_parent_node(node->left_node);
    }
}

// nodeをヌルポチェックして返す関数
nonterm_node* check_nullptr_nonterm_node(nonterm_node* node){
    if(node == nullptr){
        cout << "　 ∧＿∧ 　　" << endl << 
                "　（　´∀｀）＜　ぬるぽ" << endl;
        assert(0);
    }
    return node;
}
// 隣接しているnodeをNULLポチェックして返す
nonterm_node* get_adjacent_node(nonterm_node* node, string direction){
    if(direction == "parent") return check_nullptr_nonterm_node(node->parent_node);
    if(direction == "child") return check_nullptr_nonterm_node(node->child_node);
    if(direction == "left") return check_nullptr_nonterm_node(node->left_node);
    if(direction == "right") return check_nullptr_nonterm_node(node->right_node);
}
// 次の頂点の移動先に移動する関数（子->弟->親の順）. 返り値は移動先のノード
nonterm_node* get_next_node(nonterm_node* node, nonterm_node* root = nullptr){
    if(node->child_node != nullptr)return node->child_node;
    if(node->right_node != nullptr)return node->right_node;

    // nonterm_node* parent_node = get_parent_node(node);
    // if(root != nullptr)cout << "parenet_node is " <<  parent_node->token << ", root_node is " << root->token << endl;
    while(node != nullptr && node->right_node == nullptr){
        node = get_parent_node(node);
        if(root != nullptr && root == node){
            return node;
        }
        if(node->token == "PROGRAM"){
            return node;
        }
    }
    return node->right_node;
}

// 木の根を返す
// 木は非終端記号のノードしか持たない
nonterm_node* create_RST(vector<string> token_stream, vector<string> input_stream){
    // 木の根と現在地
    nonterm_node* root;
    nonterm_node* now_node;

    // Parsing
    Parsing p;

    stack<string> parsing_stack;
    // 初期値
    parsing_stack.push("$");
    parsing_stack.push("PROGRAM");
    root = init_nonterm_node("PROGRAM");

    now_node = root;

    int token_stream_cursor = 0;
    while(token_stream_cursor < sz(token_stream)){
        // DEBUG
        // cout << "[now_node token is " << now_node->token << "]" << endl;
        // cout << token_stream[token_stream_cursor] << endl;
        // all_watch_in_stack(parsing_stack);
        // cout << endl;

        auto token_i = token_stream[token_stream_cursor];
        string parsing_stack_top = parsing_stack.top();
        // stackのtopが$の場合
        if(parsing_stack_top == "$"){
            // 入力バッファとスタックどちらも$のとき
            if(token_i == parsing_stack_top){
                parsing_stack.pop();
                token_stream_cursor++;
                cout << "OK:RST作成終了" << endl;
                return root;
            }
            // それ以外のとき
            else {
                cout << "構文エラーです" << endl;
                assert(0);
            }
            now_node = get_next_node(now_node);
        }
        // stackのtopが#epsの場合
        else if(parsing_stack_top == "#eps"){
            // 入力を進めずにstackだけ抜く
            parsing_stack.pop();
            now_node = get_next_node(now_node);
        }
        // stackのtopが終端記号の場合
        else if(is_term(parsing_stack_top)){
            // #idと#numberのときだけRSTのノードをつなげる
            if(token_i == "#id"){
                term_node* term_node = init_term_node(token_i, 0, input_stream[token_stream_cursor]);
                now_node->term_node = term_node;
            }
            else if(token_i == "#number"){
                int number = to_num(input_stream[token_stream_cursor]);
                term_node* term_node = init_term_node(token_i, number, "");
                now_node->term_node = term_node;
            }
            else {
                term_node* term_node = init_term_node(token_i, 0, "");
                now_node->term_node = term_node;
            }

            if(token_i == parsing_stack_top){
                token_stream_cursor++;
                parsing_stack.pop();
            }
            else {
                cout << "構文エラーです" << endl;
                assert(0);
            }
            now_node = get_next_node(now_node);
        }
        // stackのtopが非終端記号の場合
        else {
            // RSTの操作はこの中だけでする
            int transition_num = p.get_ll_parsing_table(parsing_stack_top, token_i);
            if(transition_num < 0){
                cout << "構文エラーです" << endl;
                assert(0);
            }
            auto [src, dst] = bnf_transition_list[transition_num];

            assert(now_node->token == src);
            
            // 木の操作は非終端記号についてのみ行う
            // そのため、nonterm->termのときはスルー
            if(size(dst) > 0 && !is_term(dst[0])){
                // DEBUG
                // cout << now_node->token << ", ";
                // cout << src << " -> ";
                // for(auto di : dst)cout << di << ", ";
                // cout << endl;
                // 子供ノードをつなげる
                nonterm_node* child_nonterm_nodeop = connect_brothers(dst);
                connect_parent_and_child(now_node, child_nonterm_nodeop);
                now_node = get_next_node(now_node);
            }

            parsing_stack.pop();
            // stackには後ろから入れる
            for(int dst_cur = sz(dst) - 1; dst_cur >= 0; dst_cur--){
                parsing_stack.push(dst[dst_cur]);
            }
            // cout << token_i << " " << parsing_stack_top << endl;
        }
    }
    return root;
}

nonterm_node* get_next_node_greedy(nonterm_node* node, map<nonterm_node*, bool> seen){
    // 子, 弟, 兄, 親の順
    if(!seen[node->child_node] && node->child_node != nullptr){
        cout << "child ";
        return node->child_node;
    }
    if(!seen[node->right_node] && node->right_node != nullptr){
        cout << "right ";
        return node->right_node;
    }
    if(node->left_node != nullptr){
        cout << "left ";
        return node->left_node;
    }
    if(node->parent_node != nullptr){
        cout << "parent ";
        return node->parent_node;
    }
    cout << "何もない" << endl;
    assert(0);
}

// RSTを全部表示するプログラム
// 親->兄弟->子の順で見せていく
void all_watch_RST(nonterm_node* root){
    map<nonterm_node*, bool> seen;
    cout << "###### RST ######" << endl;
    nonterm_node* now_node = root;
    int cnt = 0;
    while(now_node != nullptr && cnt < 2){
        seen[now_node] = true;
        if(now_node->token == "PROGRAM")cnt++;
        cout << now_node->token << endl;
        if(cnt >= 2)break;
        if(now_node->term_node != nullptr){
            term_node* term_node = now_node->term_node;
            if(term_node->token == "#id")cout << term_node->id << endl;
            if(term_node->token == "#number")cout << term_node->number << endl;
        }
        now_node = get_next_node_greedy(now_node, seen);
    }
    cout << "###### END ######" << endl;
}

string get_id_from_nonterm_node(nonterm_node* node){
    if(node->term_node == nullptr){
        cout << "idはありません" << endl;
        return "";
    }
    term_node* term_node = node->term_node;
    return term_node->id;
}