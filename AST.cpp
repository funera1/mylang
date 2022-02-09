#include "../include.cpp"
#include "../global_values.hpp"

typedef struct node {
    // token
    string token;
    // 実値(これはtokenが#idか#numberのときのみ使う)
    int number;
    string id;
    // 親 node
    struct node* parent_node;
    // 子 node
    vector<struct node*> child_nodes;  
} node_t;

node_t* init_node(string token, int number=0, string id=""){
    node_t* node;
    node = (node_t*)malloc(sizeof(node_t));
    node->token = token;
    if(node->token == "#id")node->id = id;
    if(node->token == "#number")node->number = number;
    return node;
}

void add_child(node_t* parent_node, string token, int number=0, string id=""){
    node_t* child_node = init_node(token, number, id);
    // 自分自身のポインタってどう表す？-> 親の子供として表す
    child_node->parent_node = parent_node;
    // 子ノードを追加
    parent_node->child_nodes.push_back(child_node);
}


int main(){
    // テスト
    node_t head;
    node_t* root = init_node("PROGRAM");
    add_child(root, "LBRACKET");
}