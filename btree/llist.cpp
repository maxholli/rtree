#include <iostream>
#include <stdlib.h>
#include "llist.h"

llist::llist()
{
    head = (lnode*) malloc(sizeof(struct lnode));
    head->prev = nullptr;
    head->key = 0;
    head->next = nullptr;
}

llist::~llist()
{
    teardown(head);
}

void llist::teardown(lnode* head) //for the deconstructor
{
    if (head->next != nullptr) 
    {
        teardown(head->next);
    }
    free(head);
}

void llist::print_list()
{
    print_list_rec(head);
    std::cout << std::endl;
}

void llist::print_list_rec(lnode* head)
{
    if (head->next != nullptr) {
        std::cout << head->next->key << ' ';
        print_list_rec(head->next);
    }
}

int llist::insert_item(int key)
{
    return insert_item_rec(key, head, 0);
}

int llist::insert_item_rec(int key, lnode* head, int index)
{
    lnode* current = head;
    if (current->next == nullptr) //new last node
    {
        lnode *new_node = (lnode*) malloc(sizeof(struct lnode));
        current->next = new_node;
        new_node->prev = current;
        new_node->key = key;
        new_node->next = nullptr;
        return index;
    }
    if (current->next->key >= key) //new node inserted before next node
    {
        lnode *new_node = (lnode*) malloc(sizeof(struct lnode));
        new_node->prev = current;
        new_node->next = current->next;
        current->next->prev = new_node;
        current->next = new_node;

        new_node->key = key;
        return index;
    }
    return insert_item_rec(key, current->next, index+1);
}

int llist::where_to_insert(int key)
{
    return where_to_insert_item_rec(key, head, 0);
}


int llist::where_to_insert_item_rec(int key, lnode* head, int index)
{
    lnode* current = head;
    if (current->next == nullptr) //new last node
    {
        return index;
    }
    if (current->next->key >= key) //new node inserted before next node
    {
        return index;
    }
    return where_to_insert_item_rec(key, current->next, index+1);
}


lnode* llist::search(int key) 
{
    if (head->next == nullptr)
    {
        return nullptr;
    }
    lnode* current = head->next;
    while (current->key != key) {
        if (current->next == nullptr)
        {
            return nullptr;
        }
        current = current->next;
    }
    return current;
}

int llist::search_by_index(int index)
{
    //walk thru the list i steps. return the key. (and pop? would save time)
    //0 indexed.
    lnode* current = head;
    for (int i=0; i<=index; i++)
    {
        if (current->next == nullptr)
        {
            std::cout << "ERROR, list is shorter than the index." << std::endl;
            return -1;
        }
        current = current->next;
    }
    return current->key;

}

void llist::remove(int key)
{
    lnode* to_remove = search(key);
    if (to_remove != nullptr)
    {
        if (to_remove->next == nullptr)
        {
            to_remove->prev->next = nullptr;
        } 
        else
        {
            to_remove->prev->next = to_remove->next;
            to_remove->next->prev = to_remove->prev;
        }
        free(to_remove);
    }
}