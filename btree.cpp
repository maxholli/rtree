#include "btree.h"
#include <stdlib.h>
#include <iostream>

btree::btree()
{
    root = init_btnode();
    t = NUM_CHILDREN / 2;
    h = 0; // root alone is 0 height
}

btree::btree(int min_degree)
{
    root = init_btnode();
    t = min_degree;
    h = 0; // root alone is 0 height
}

btree::~btree()
{
    teardown(root);
}

void btree::teardown(btnode* btp)
{
    if (btp->leaf)
    {
        delete btp;
    }
    else
    {
        for (int i=0; i<8; i++)
        {
            if (btp->a_children[i] != nullptr)
            {
                teardown(btp->a_children[i]);
            }
        }
        free(btp->a_children);
        delete btp;
    }
}

btnode* btree::init_btnode()
{
    //btnode* new_node = (btnode*) malloc(sizeof(struct btnode));
    btnode* new_node = new btnode;
    new_node->n = 0;
    new_node->leaf = true;
    return new_node;
}

void btree::leaf_to_inner(btnode* leaf_node)
{
    leaf_node->a_children = (btnode**) malloc(sizeof(struct btnode) * 2*t);
    for (int i=0; i<(2*t); i++)
    {
        leaf_node->a_children[i] = nullptr;
    }
}

int btree::getH()
{
    return h;
}
int btree::getT()
{
    return t;
}

void btree::depth_first_print()
{
    depth_first_print_rec(root);
}

void btree::depth_first_print_rec(btnode* btp)
{
    std::cout << "ID: " << std::hex << (long int) btp % 1048576 << std::dec << std::endl;
    if (btp->leaf)
    {
        btp->keys.print_list();
        std::cout << "LEAF no children.\n" << std::endl;
    }
    else
    {
        btp->keys.print_list();
        for (int i=0; i<8; i++)
        {
            std::cout << std::hex << (long int) btp->a_children[i] % 1048576 << std::dec << ' ';
        }
        std::cout << '\n' << std::endl;
        for (int i=0; i<8; i++)
        {
            if (btp->a_children[i] != nullptr)
            {
                depth_first_print_rec(btp->a_children[i]);
            }
        }
    }
}

void btree::split_child(btnode* x, int i)
{
    btnode* z = init_btnode(); // new node z
    btnode* y = x->a_children[i]; // find x's i-th child, stick it in y 
    z->leaf = y->leaf;
    if (z->leaf == false)
    {
        leaf_to_inner(z);
    }
    z->n = t-1;

    // key S in cormen book
    int key_to_x = y->keys.search_by_index(t-1);

    // move second half of y's keys to z.
    int keys_to_move[t-1];
    for (int j=0; j < t-1; j++) 
    {
        keys_to_move[j] = y->keys.search_by_index(j+t);
    }
    for (int j=0; j < t-1; j++)
    {
        z->keys.insert_item(keys_to_move[j]);
        y->keys.remove(keys_to_move[j]);
    }

    // if not a leaf node, move second half of y's children to z.
    if (!y->leaf)
    {
        for (int j=0; j<t; j++)
        {
            z->a_children[j] = y->a_children[j+t];
            y->a_children[j+t] = nullptr;
        }
    }

    y->n = t-1;

    //shift x's above-i children up one slot, to make room for the new child
    for (int j=x->n; j > i; j--)
    {
        x->a_children[j+1] = x->a_children[j];
    } 
    x->a_children[i+1] = z;

    //no shifting needed for keys (because it's a linked list)
    // just insert new key, and remove old from y
    x->keys.insert_item(key_to_x);
    y->keys.remove(key_to_x);

    x->n = x->n + 1;

}

void btree::insert(int k)
{
    btnode* r = root;
    if (r->n == 2*t-1)
    {
        btnode* s = init_btnode();
        root = s;
        s->leaf = false;
        leaf_to_inner(s);
        s->n = 0;
        s->a_children[0] = r;
        split_child(s, 0);
        insert_nonfull(s, k);
    }
    else
    {
        insert_nonfull(r, k);
    }
}

void btree::insert_nonfull(btnode* x, int k)
{
    int i = x->n;
    if (x->leaf)
    {
        x->keys.insert_item(k);
        x->n = x->n+1;
    }
    else
    {
        i = x->keys.where_to_insert(k);
        
        //in_i = in_i + 1;
        if (x->a_children[i]->n == 2*t-1) 
        {
            split_child(x, i);
            if (k > x->keys.search_by_index(i))
            {
                i = i + 1;
            }
        }
        insert_nonfull(x->a_children[i], k);
    }
}