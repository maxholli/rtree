#ifndef LLIST_H
#define LLIST_H

struct lnode {
    lnode *prev;
    int key;
    lnode *next;
};

class llist {
    public:
        llist();
        ~llist();
        void print_list();
        int insert_item(int key);
        int where_to_insert(int key);
        lnode* search(int key);
        int search_by_index(int index); //returns the key's value
        void remove(int key);

    private:
        void print_list_rec(lnode* head);
        int insert_item_rec(int key, lnode* head, int index);
        int where_to_insert_item_rec(int key, lnode* head, int index);
        void teardown(lnode* head); //for the deconstructor
        lnode* head;
        
};

#endif