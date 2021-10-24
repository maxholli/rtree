#ifndef BTREE_H
#define BTREE_H
#include "llist.h"

#define NUM_CHILDREN 8 // should be an even number

struct btnode {
    int n; // number of keys stored on this node.
    llist keys; // keys, size n
    bool leaf; 
    btnode** a_children; // children, size n+1
};

class btree {
    public:
        btree();
        btree(int min_degree);
        ~btree(); // should teardown whole b-tree
        int getH();
        int getT();
        void insert(int k);
        void depth_first_print();

    private:
        void depth_first_print_rec(btnode* btp);
        void split_child(btnode* x, int i);
        btnode* init_btnode();
        void leaf_to_inner(btnode* leaf_node);
        void insert_nonfull(btnode* x, int k);
        void teardown(btnode* btp);
        btnode* root;
        int h; // the tree's height.
        int t; // minimum degree of the b-tree
        // every node must have at least t-1 keys (except root)
        //  therefore every internal node has at least t children.
        // every node may have at most 2t-1 keys, and therefore 2t children.
};

#endif