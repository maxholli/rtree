#ifndef LLIST_H
#define LLIST_H

#include <iostream>
#include <stdlib.h>

// doubley linked list
// insertions are sorted. Thus maintaines increasing order over all values in the list
// works with int, float, double


template <class T>
class llist {
    public:
        //standard operations
        llist();
        ~llist();

        //list specific operations
        int insert_item(T x);
        int where_to_insert(T x);
        T search_by_index(int index); //returns the key's value
        void remove(T x);
        void print_list(); // maybe replace with
        //friend wostream& operator<< <T>(wostream& os, llist<T>& ll)

    private:
        // struct lnode;
        // typedef lnode* lnode*;
        struct lnode {
            lnode* prev;
            T value;
            lnode* next;
        };
        void print_list_rec(lnode* head);
        int insert_item_rec(T x, lnode* head, int index);
        int where_to_insert_item_rec(T x, lnode* head, int index);
        void teardown(lnode* head); //for the deconstructor
        lnode* search(T x);
        lnode* head;        
};

template <class T>
llist<T>::llist()
{
    head = (lnode*) malloc(sizeof(struct lnode));
    head->prev = NULL;
    //head->value = 0;
    head->next = NULL;
}

template <class T>
llist<T>::~llist()
{
    teardown(head);
}

template <class T>
void llist<T>::teardown(lnode* head) //for the deconstructor
{
    if (head->next != NULL) 
    {
        teardown(head->next);
    }
    free(head);
}

template <class T>
void llist<T>::print_list()
{
    print_list_rec(head);
    std::cout << std::endl;
}

template <class T>
void llist<T>::print_list_rec(lnode* head)
{
    if (head->next != NULL) {
        std::cout << head->next->value << ' ';
        print_list_rec(head->next);
    }
}

template <class T>
int llist<T>::insert_item(T value)
{
    return insert_item_rec(value, head, 0);
}

template <class T>
int llist<T>::insert_item_rec(T value, lnode* head, int index)
{
    lnode* current = head;
    if (current->next == NULL) //new last node
    {
        lnode* new_node = (lnode*) malloc(sizeof(struct lnode));
        current->next = new_node;
        new_node->prev = current;
        new_node->value = value;
        new_node->next = NULL;
        return index;
    }
    if (current->next->value >= value) //new node inserted before next node
    {
        lnode* new_node = (lnode*) malloc(sizeof(struct lnode));
        new_node->prev = current;
        new_node->next = current->next;
        current->next->prev = new_node;
        current->next = new_node;

        new_node->value = value;
        return index;
    }
    return insert_item_rec(value, current->next, index+1);
}

template <class T>
int llist<T>::where_to_insert(T value)
{
    return where_to_insert_item_rec(value, head, 0);
}

template <class T>
int llist<T>::where_to_insert_item_rec(T value, lnode* head, int index)
{
    lnode* current = head;
    if (current->next == NULL) //new last node
    {
        return index;
    }
    if (current->next->value >= value) //new node inserted before next node
    {
        return index;
    }
    return where_to_insert_item_rec(value, current->next, index+1);
}

template <class T>
typename llist<T>::lnode* llist<T>::search(T value) 
{
    if (head->next == NULL)
    {
        return NULL;
    }
    lnode* current = head->next;
    while (current->value != value) {
        if (current->next == NULL)
        {
            return NULL;
        }
        current = current->next;
    }
    return current;
}

template <class T>
T llist<T>::search_by_index(int index)
{
    //walk thru the list i steps. return the value. (and pop? would save time)
    //0 indexed.
    lnode* current = head;
    for (int i=0; i<=index; i++)
    {
        if (current->next == NULL)
        {
            std::cout << "ERROR, list is shorter than the index." << std::endl;
            return (T) -1;
        }
        current = current->next;
    }
    return current->value;

}

template <class T>
void llist<T>::remove(T value)
{
    lnode* to_remove = search(value);
    if (to_remove != NULL)
    {
        if (to_remove->next == NULL)
        {
            to_remove->prev->next = NULL;
        } 
        else
        {
            to_remove->prev->next = to_remove->next;
            to_remove->next->prev = to_remove->prev;
        }
        free(to_remove);
    }
}


#endif