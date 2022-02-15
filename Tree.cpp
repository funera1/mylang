#include "include.cpp"

class Tree {
    typedef struct node {
        int val;
        struct node* left;
        struct node* right;
    } node_t;
    
    typedef struct tree{
        node_t* root;
    } tree_t;

    node_t* insertNode
}