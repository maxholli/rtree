#ifndef BTREE_H
#define BTREE_H
#include "llist.h"

#define NUM_CHILDREN 8 // should be an even number
#define DELETE_DEBUG 0 // 0 is off

class btree {
    public:
        btree();
        btree(int min_degree);
        ~btree(); // should teardown whole b-tree
        int getH();
        int getT();
        void insert(int k);
        void depth_first_print();
        void depth_first_5check(int cases);
        void delete_key(int k);
        bool key_exists(int k);
        int keys_total();

    private:
        struct btnode;
        typedef btnode* btnodePtr;
        struct btnode {
            int n; // number of keys stored on this node.
            llist<int> keys; // keys, size n
            bool leaf; 
            btnodePtr* a_children; // children, size n+1
        };
        int find_biggest_key(btnodePtr btp);
        int find_smallest_key(btnodePtr btp);
        void depth_first_print_rec(btnodePtr btp);
        void depth_first_5check_rec(btnodePtr btp);
        void depth_first_ncheck_rec(btnodePtr btp);
        int depth_first_totaln_rec(btnodePtr btp);
        void split_child(btnodePtr x, int i);
        btnodePtr init_btnode();
        void leaf_to_inner(btnodePtr leaf_node);
        void insert_nonfull(btnodePtr x, int k);
        void teardown(btnodePtr btp);
        void delete_key_rec(btnodePtr x, int k);
        bool key_exists_rec(btnodePtr x, int k);
        btnodePtr root;
        int h; // the tree's height.
        int t; // minimum degree of the b-tree
        // every node must have at least t-1 keys (except root)
        //  therefore every internal node has at least t children.
        // every node may have at most 2t-1 keys, and therefore 2t children.
};

#endif