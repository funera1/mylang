#include "include.cpp"
#include "global_values.hpp"
#include "parsing.cpp"

typedef struct node {
    // token
    string token;
    // 実値(これはtokenが#idか#numberのときのみ使う)
    // 複雑になるのでとりあえずなしで
    // int number;
    // string id;

    struct node* parent_node;
    struct node* child_node;
    struct node* right_node;
    struct node* left_node;

} node_t;

node_t* init_node(string token){
    node_t* node;
    node = new node_t;
    // TODO: strcpy使わんとダメ？
    node->token = token;

    node->parent_node = NULL;
    node->child_node = NULL;
    node->right_node = NULL;
    node->left_node = NULL;

    return node;
}

void connect_parent_and_child(node_t* parent_node, node_t* child_node){
    parent_node->child_node = child_node;
    child_node->parent_node = parent_node;
}

void connect_brother_and_brother(node_t* left_node, node_t* right_node){
    left_node->right_node = right_node;
    right_node->left_node = left_node;
}

// A->abcという変換があったとき、abcを兄弟としたグラフを構築し、aに当たるノードを返す
node_t* connect_brothers(vector<string> vs){
    vector<node_t*> nodes(sz(vs));
    // それぞれに対応するノードを構成
    for(int i = 0; i < sz(nodes); i++){
        nodes[i] = init_node(vs[i]);
    }
    // 隣り合う兄弟間を辺でつなぐ
    for(int i = 0; i < sz(nodes)-1; i++){
        connect_brother_and_brother(nodes[i], nodes[i+1]);
    }
    // 一番最初のノードを返す
    return nodes[0];
}

void release_node(node_t* node){
    if(node->parent_node != NULL || node->child_node != NULL | node->right_node != NULL || node->left_node != NULL){
        cout << "child_node or borhter_node is not released" << endl;
        assert(0);
    }
    free(node);
    // ダブルフリーを防ぐ
    node = NULL;
}

// TODO: どの子からでも親に移動する関数. 返り値は親のノード
node_t* get_parent_node(node_t* node){
    cout << "IN GET_PARENT_NODE: " << node->token << endl;
    // 一番上まで来たら終了
    if(node->token == "PROGRAM")return node;
    // 左のノードがないとき
    if(node->left_node == NULL){
        return node->parent_node;
    }
    else {
        return get_parent_node(node->left_node);
    }
}

// TODO: 次の頂点の移動先に移動する関数（子->弟->親の順）. 返り値は移動先のノード
node_t* get_next_node(node_t* node){
    // それぞれポインタ先の実値を指している
    if(node->child_node != NULL)return node->child_node;
    if(node->right_node != NULL)return node->right_node;


    node_t* parent_node = get_parent_node(node);
    if(parent_node->token != "PROGRAM"){
        return parent_node;
    }
    while(parent_node != NULL && parent_node->right_node == NULL)parent_node = get_parent_node(parent_node);
    return parent_node->right_node;
}

// 木の根を返す
// 木は非終端記号のノードしか持たない
node_t* create_RST(vector<string> token_stream, vector<string> input_stream){
    // 木の根と現在地
    node_t* root;
    node_t* now_node;

    // Parsing
    Parsing p;

    stack<string> parsing_stack;
    // 初期値
    parsing_stack.push("$");
    parsing_stack.push("PROGRAM");
    root = init_node("PROGRAM");
    // バグるかも?
    now_node = root;

    int token_stream_cursor = 0;
    while(token_stream_cursor < sz(token_stream)){
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
        }
        // stackのtopが#epsの場合
        else if(parsing_stack_top == "#eps"){
            // 入力を進めずにstackだけ抜く
            parsing_stack.pop();
        }
        // stackのtopが終端記号の場合
        else if(is_term(parsing_stack_top)){
            if(token_i == parsing_stack_top){
                token_stream_cursor++;
                parsing_stack.pop();
            }
            else {
                cout << "構文エラーです" << endl;
                assert(0);
            }
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

            // assert(now_node->token == src);
            
            // 木の操作は非終端記号についてのみ行う
            // そのため、nonterm->termのときはスルー
            // また、EPSのときもスルー
            if(size(dst) > 0 && (!is_term(dst[0]) || dst[0] == "EPS")){
                // DEBUG
                // cout << now_node->token << ", ";
                // cout << src << " -> ";
                // for(auto di : dst)cout << di << ", ";
                // cout << endl;
                // 子供ノードをつなげる
                node_t* child_node_top = connect_brothers(dst);
                connect_parent_and_child(now_node, child_node_top);
            }

            parsing_stack.pop();
            // stackには後ろから入れる
            for(int dst_cur = sz(dst) - 1; dst_cur >= 0; dst_cur--){
                parsing_stack.push(dst[dst_cur]);
            }
            cout << token_i << " " << parsing_stack_top << endl;
            now_node = get_next_node(now_node);
            cout << "END" << endl;
        }
        // DEBUG
        // cout << token_stream[token_stream_cursor] << endl;
        // all_watch_in_stack(parsing_stack);
    }
    return root;
}

// RSTをASTに変換する
void RST_to_AST(node_t* root){
    // ASTを走査する
}
// RSTを全部表示するプログラム
// 親->兄弟->子の順で見せていく
// void watch_all_RST(node_t* root){
//     node_t* now_node = root;
//     while(now_node != NULL){
//         node_t* brother_top_node = now_node;
//         // 兄弟をすべて出力
//         cout << now_node->token << " ";
//         while(now_node->right_node != NULL){
//             now_node = now_node->right_node;
//             cout << now_node->token << " ";
//         }
//         cout << endl;
//         now_node = brother_top_node->child_node;
//     }
// }