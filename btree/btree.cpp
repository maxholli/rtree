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
    // std::cout << "getting to teardown" << std::endl;
    teardown(root);
}

void btree::teardown(btnodePtr btp)
{
    if (btp->leaf)
    {
        delete btp;
    }
    else
    {
        for (int i=0,z=2*t; i<z; i++)
        {
            if (btp->a_children[i] != NULL)
            {
                teardown(btp->a_children[i]);
            }
        }
        free(btp->a_children);
        delete btp;
    }
}

btree::btnodePtr btree::init_btnode()
{
    // btnodePtr new_node = (btnodePtr) malloc(sizeof(struct btnode));
    btnodePtr new_node = new btnode;
    new_node->n = 0;
    new_node->leaf = true;
    return new_node;
}

void btree::leaf_to_inner(btnodePtr leaf_node)
{
    leaf_node->a_children = (btnodePtr*) malloc(sizeof(struct btnode) * 2*t);
    for (int i=0,z=2*t; i<z; i++)
    {
        leaf_node->a_children[i] = NULL;
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

void btree::depth_first_print_rec(btnodePtr btp)
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
        for (int i=0,z=2*t; i<z; i++)
        {
            std::cout << std::hex << (long int) btp->a_children[i] % 1048576 << std::dec << ' ';
        }
        std::cout << '\n' << std::endl;
        for (int i=0,z=2*t; i<z; i++)
        {
            if (btp->a_children[i] != NULL)
            {
                depth_first_print_rec(btp->a_children[i]);
            }
        }
    }
}

void btree::depth_first_5check(int cases)
{
    if (cases == 0)
    {
        depth_first_5check_rec(root);
    }
    else if (cases == 1)
    {
        depth_first_ncheck_rec(root);
    }
    else
    {
        std::cout << "total keys = " << depth_first_totaln_rec(root) << std::endl;
    }
}

int btree::keys_total()
{
    return depth_first_totaln_rec(root);
}


void btree::depth_first_5check_rec(btnodePtr btp)
{
    std::cout << "ID: " << std::hex << (long int) btp % 1048576 << std::dec << std::endl;
    if (btp->leaf)
    {
        btp->keys.print_list();
        std::cout << "LEAF no children. n=" << btp->n << "\n" << std::endl;

        if (btp != root)
        {
            if (!(btp->n >= t-1 && btp->n < 2*t))
            {
                std::cout << "ERROR: this leaf breaks rule 5! \n" << std::endl;
            }
        }
    }
    else
    {
        btp->keys.print_list();
        for (int i=0,z=2*t; i<z; i++)
        {
            std::cout << std::hex << (long int) btp->a_children[i] % 1048576 << std::dec << ' ';
        }
        std::cout << " n=" << btp->n;
        std::cout << '\n' << std::endl;
        if (btp != root)
        {
            if (!(btp->n >= t-1 && btp->n < 2*t))
            {
                std::cout << "ERROR: this internal breaks rule 5! \n" << std::endl;
            }
        }
        for (int i=0,z=2*t; i<z; i++)
        {
            if (btp->a_children[i] != NULL)
            {
                depth_first_5check_rec(btp->a_children[i]);
            }
        }
    }
}

void btree::depth_first_ncheck_rec(btnodePtr btp)
{
    if (btp->leaf)
    {
        if (btp->keys.length() != btp->n)
        {
            {
                std::cout << "ERROR: leaf's n is off from length \n" << std::endl;
            }
        } 
        if (btp != root)
        {
            if (!(btp->n >= t-1 && btp->n < 2*t))
            {
                std::cout << "ERROR: this leaf breaks rule 5! \n" << std::endl;
            }
        }
    }
    else
    {
        if (btp->keys.length() != btp->n)
        {
            {
                std::cout << "ERROR: internal's n is off from length \n" << std::endl;
            }
        } 
        if (btp != root)
        {
            if (!(btp->n >= t-1 && btp->n < 2*t))
            {
                std::cout << "ERROR: this internal breaks rule 5! \n" << std::endl;
            }
        }
        for (int i=0,z=2*t; i<z; i++)
        {
            if (btp->a_children[i] != NULL)
            {
                depth_first_ncheck_rec(btp->a_children[i]);
            }
        }
    }
}

int btree::depth_first_totaln_rec(btnodePtr btp)
{
    if (btp->leaf)
    {
        return btp->n;
    }
    else
    {
        int sub_total = 0;
        for (int i=0,z=2*t; i<z; i++)
        {
            if (btp->a_children[i] != NULL)
            {
                sub_total = sub_total + depth_first_totaln_rec(btp->a_children[i]);
            }
        }
        return sub_total + btp->n;
    }
}


void btree::split_child(btnodePtr x, int i)
{
    btnodePtr z = init_btnode(); // new node z
    btnodePtr y = x->a_children[i]; // find x's i-th child, stick it in y 
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
            y->a_children[j+t] = NULL;
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
    btnodePtr r = root;
    if (r->n == 2*t-1)
    {
        btnodePtr s = init_btnode();
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

void btree::insert_nonfull(btnodePtr x, int k)
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


bool btree::key_exists(int k)
{
    return key_exists_rec(root, k);
}

bool btree::key_exists_rec(btnodePtr x, int k)
{
    //std::cout << "ID: " << std::hex << (long int) x % 1048576 << std::dec << std::endl;
    int key_index = x->keys.contains(k); //returns -1 if k is not in keys
    if (key_index != -1) 
    {
        // std::cout << "Yes, key " << k << "does exist" << std::endl;
        return true;
    }
    else
    {
        if (!x->leaf)
        {
            key_exists_rec(x->a_children[x->keys.where_to_insert(k)], k);
            // std::cout << "sorry key " << k << " doesn't exist" << std::endl;
        }
        else
        {
            return false;
        }
    }
}

int btree::find_biggest_key(btnodePtr btp)
{
    if (btp->leaf)
    {
        return btp->keys.search_by_index(btp->n-1);
    }
    else
    {
        return find_biggest_key(btp->a_children[btp->n]);
    }
}

int btree::find_smallest_key(btnodePtr btp)
{
    if (btp->leaf)
    {
        return btp->keys.search_by_index(0);
    }
    else
    {
        return find_smallest_key(btp->a_children[0]);
    }
}

void btree::delete_key(int k)
{
    delete_key_rec(root, k);

    // check if root has no keys, get rid of it (and should shorten the tree by one)
    if (root->n == 0 && !root->leaf) 
    {
        btnodePtr old_root = root;
        root = root->a_children[0];
        free(old_root->a_children);
        delete old_root;
    }
}

// TODO update n for all of these
void btree::delete_key_rec(btnodePtr x, int k) // TODO int k should be template T
{
    if (DELETE_DEBUG) std::cout << "ID: " << std::hex << (long int) x % 1048576 << std::dec << std::endl;
    int key_index = x->keys.contains(k); //returns -1 if k is not in keys
    if (key_index != -1) // x contains the key
    {
        if (DELETE_DEBUG) std::cout << "node contains key" << std::endl;
        if (x->leaf)
        {
            x->keys.remove(k);
            x->n = x->n - 1;
        }
        else // x is an internal node
        {
            if (x->a_children[key_index]->n >= t) // examine predecesor child y  // SEEN IT
            {
                // take last thing in the list of a_children[key_index]->keys as k'
                // int k_prime = x->a_children[key_index]->keys.search_by_index(x->a_children[key_index]->n - 1); // TODO k_prime should be template T
                int k_prime = find_biggest_key(x->a_children[key_index]);
                delete_key_rec(x->a_children[key_index], k_prime);
                x->keys.remove(k);
                x->keys.insert_item(k_prime);
                // no change to n (k is replaced by k_prime)
            }
            else if (x->a_children[key_index+1]->n >= t) // examine following child z
            {
                // take the first thing in the list of a_children[key_index+1]->keys as k'
                //int k_prime = x->a_children[key_index+1]->keys.search_by_index(0); // TODO k_prime should be template T
                int k_prime = find_smallest_key(x->a_children[key_index+1]);
                delete_key_rec(x->a_children[key_index+1], k_prime);
                x->keys.remove(k);
                x->keys.insert_item(k_prime);
                // no change to n (k is replaced by k_prime)
            }
            else
            {
                btnodePtr y = x->a_children[key_index];
                btnodePtr z = x->a_children[key_index+1];
                // merge (k and all of z) into y
                if (y->leaf && z->leaf) // both are leaves
                {
                    // only need to merge keys
                    for (int i=0; i < z->n; i++)
                    {
                        int key_to_move = z->keys.search_by_index(i); // TODO int key_to_move should be template
                        y->keys.insert_item(key_to_move);
                        y->n = y->n + 1;
                    }
                    y->keys.insert_item(k);
                    y->n = y->n + 1;
                }
                else if (!y->leaf && !z->leaf) // both are internal
                {
                    // merge children (done before keys because y->n is used)
                    for (int i=0; i <= z->n; i++)
                    {
                        y->a_children[y->n+1+i] = z->a_children[i];
                    }
                    // merge keys 
                    for (int i=0; i < z->n; i++)
                    {
                        int key_to_move = z->keys.search_by_index(i); // TODO int key_to_move should be template
                        y->keys.insert_item(key_to_move);
                        y->n = y->n + 1;
                    }
                    y->keys.insert_item(k);
                    y->n = y->n + 1;
                }
                else
                {
                    std::cout << "ERROR, leaf imbalence, y is " << y->leaf << " and z is " << z->leaf << std::endl;
                    // if one is leaf and the other isn't, then make the merged y become an internal node, and merge keys, with the children i think you ... i dunno
                }

                //  so that x looses k and the pointer to z. and y now contains 2t-1 keys
                x->keys.remove(k);
                x->n = x->n - 1;
                for (int i=key_index+2; i<2*t; i++)
                {
                    x->a_children[i-1] = x->a_children[i];
                }
                x->a_children[2*t-1] = NULL;
                //  free z 
                if (!z->leaf) //internal node
                {
                    free(z->a_children);
                }
                delete z;
                // and recursively delete k from y 
                delete_key_rec(y, k);
            }
        }
    }
    else // x doesn't contain the key
    {
        if (DELETE_DEBUG) std::cout << "node doesn't contain key" << std::endl;
        if (x->leaf)
        {
            std::cout << "ERROR, the key you specified doesn't exist in this tree" << std::endl;
        }
        else
        {
            int child_index = x->keys.where_to_insert(k);
            btnodePtr x_subt = x->a_children[child_index];
            
            if (x->a_children[child_index]->n < t)
            {
                if (DELETE_DEBUG) std::cout << "x_subt " << std::hex << (long int) x_subt % 1048576 << std::dec << " has too few keys, got do some shuffling" << std::endl;
                // execute 3a and 3b as necessary
                // 3a
                bool did_3a_happen = false;
                bool check_right = false;
                bool check_left = false;
                if (DELETE_DEBUG) std::cout << "child_index=" << child_index << " and x->n=" << x->n << std::endl;
                if (child_index == 0)
                {
                    check_right = true;
                    if (x_subt->leaf != x->a_children[child_index+1]->leaf)
                    {
                        std::cout << "ERROR 3a1" << std::endl; //problem that one is a leaf and the other is an internal
                    }
                }
                else if (child_index == x->n)
                {
                    if (DELETE_DEBUG) std::cout << "got into only check left subtree" << std::endl;
                    check_left = true;
                    if (x_subt->leaf != x->a_children[child_index-1]->leaf)
                    {
                        std::cout << "ERROR 3a2" << std::endl; //problem that one is a leaf and the other is an internal
                    }
                }
                else 
                {
                    check_right = true;
                    check_left = true;
                    if (!((x_subt->leaf == x->a_children[child_index-1]->leaf) && (x_subt->leaf == x->a_children[child_index+1]->leaf)))
                    {
                        std::cout << "ERROR 3a3" << std::endl; //problem that some are leaves and others are internal
                    }
                }
                if (DELETE_DEBUG) std::cout << "getting past the checks with check_right=" << check_right << " and check_left=" << check_left << std::endl;
                if (check_right) 
                {
                    btnodePtr y = x->a_children[child_index+1];
                    if (y->n >= t)
                    {
                        if (DELETE_DEBUG) std::cout << "do move from right to [child_index]" << std::endl;
                        // do move from right to [child_index]
                        // identify key to move down from x
                        int key_to_move_down = x->keys.search_by_index(child_index);
                        // x_subt gets key from x and first child from y
                        x_subt->keys.insert_item(key_to_move_down);
                        x_subt->n = x_subt->n + 1; // done first on purpose
                        if (!x_subt->leaf)
                        {
                            x_subt->a_children[x_subt->n] = y->a_children[0];
                        }
                        // x loses key_to_move_down and adds key_to_move_up
                        x->keys.remove(key_to_move_down);
                        int key_to_move_up = y->keys.search_by_index(0);
                        x->keys.insert_item(key_to_move_up);
                        // y loses key_to_move_up and first child
                        if (!x_subt->leaf)
                        {
                            for (int i=0; i<(2*t)-1; i++)
                            {
                                y->a_children[i] = y->a_children[i+1];
                            }
                            y->a_children[(2*t)-1] = NULL;
                        }
                        y->keys.remove(key_to_move_up);
                        y->n = y->n - 1;
                        // skip check_left
                        check_left = false;
                        did_3a_happen = true;
                    }
                }
                if (check_left)
                {
                    btnodePtr w = x->a_children[child_index-1];
                    if (w->n >= t)
                    {
                        if (DELETE_DEBUG) std::cout << "do move from left to [child_index]" << std::endl;
                        // do move from left to [child_index]
                        // identify key to move down from x
                        int key_to_move_down = x->keys.search_by_index(child_index-1);
                        // x_subt gets key from x and last child from w
                        x_subt->keys.insert_item(key_to_move_down);
                        if (!x_subt->leaf)
                        {
                            for (int i=(2*t)-1; i >= 0; i--)
                            {
                                x_subt->a_children[i+1] = x_subt->a_children[i];
                            }
                            x_subt->a_children[0] = w->a_children[w->n];
                        }
                        x_subt->n = x_subt->n + 1;
                        // x loses key_to_move_down and adds key_to_move_up
                        x->keys.remove(key_to_move_down);
                        int key_to_move_up = w->keys.search_by_index(w->n-1);
                        x->keys.insert_item(key_to_move_up);
                        // w loses key_to_move_up and last child
                        w->keys.remove(key_to_move_up);
                        if (!x_subt->leaf)
                        {
                            w->a_children[w->n] = NULL;
                        }
                        w->n = w->n - 1; // this is done last because lots of things depend on this length
                        did_3a_happen = true;
                    }   
                }
                if (!did_3a_happen)
                {
                    if (DELETE_DEBUG) std::cout << "3a didn't happen, so we're in 3b" << std::endl;
                    // 3b
                    if (child_index == 0) // merge must be from the right
                    {
                        if (DELETE_DEBUG) std::cout << "merge must be from the right" << std::endl; // SEEN IT
                        btnodePtr y = x->a_children[child_index+1]; // y is first merged into x_subt, then y is thrown out.
                        // bring children from y over to x_subt
                        if (!x_subt->leaf)
                        {
                            for (int i = 0; i <= y->n; i++)
                            {
                                x_subt->a_children[x_subt->n+1 + i] = y->a_children[i];
                            }
                        }
                        // bring keys from y over to x_subt
                        for (int i = 0; i < y->n; i++)
                        {
                            int key_to_move = y->keys.search_by_index(0);
                            x_subt->keys.insert_item(key_to_move);
                            y->keys.remove(key_to_move);
                            x_subt->n = x_subt->n + 1; 
                        }
                        // add x's key_to_move_down to x_subt
                        int key_to_move_down = x->keys.search_by_index(child_index);
                        x_subt->keys.insert_item(key_to_move_down);
                        x_subt->n = x_subt->n + 1;
                        // free y->a_children and delete y 
                        if (!x_subt->leaf)
                        {
                            free(y->a_children);
                        }
                        delete y;
                        // from x and shift childern over 
                        for (int i=child_index+1; i<(2*t)-1; i++)
                        {
                            x->a_children[i] = x->a_children[i+1];
                        }
                        x->a_children[(2*t)-1] = NULL;
                        // remove key from x
                        x->keys.remove(key_to_move_down);
                        x->n = x->n - 1; 
                    }
                    else // merge will be from the left into x_subt
                    {
                        if (DELETE_DEBUG) std::cout << "merge will be from the left into x_subt" << std::endl; // SEEN IT
                        btnodePtr w = x->a_children[child_index-1]; // x_subt is first merged into w, then x_subt is thrown out.
                        // bring children to w over from x_subt
                        if (!x_subt->leaf)
                        {
                            for (int i = 0; i <= x_subt->n; i++)
                            {
                                w->a_children[w->n+1 + i] = x_subt->a_children[i];
                            }
                        }
                        // bring keys from x_subt over to w
                        for (int i = 0; i < x_subt->n; i++)
                        {
                            int key_to_move = x_subt->keys.search_by_index(0);
                            w->keys.insert_item(key_to_move);
                            x_subt->keys.remove(key_to_move);
                            w->n = w->n + 1; 
                        }
                        // add x's key_to_move_down to w
                        int key_to_move_down = x->keys.search_by_index(child_index-1);
                        w->keys.insert_item(key_to_move_down);
                        w->n = w->n + 1;
                        // free x_subt->a_children and delete x_subt 
                        if (!x_subt->leaf)
                        {
                            free(x_subt->a_children);
                        }
                        delete x_subt;
                        // from x and shift childern over 
                        for (int i=child_index; i<(2*t)-1; i++)
                        {
                            x->a_children[i] = x->a_children[i+1];
                        }
                        x->a_children[(2*t)-1] = NULL;
                        // remove key from x
                        x->keys.remove(key_to_move_down);
                        x->n = x->n - 1; 
                    }
                    // do root check here

                }
            }
            // recurse on the appropriate child
            child_index = x->keys.where_to_insert(k);
            delete_key_rec(x->a_children[child_index], k);
        }
    }
}