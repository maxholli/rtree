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
        int insert_item(T value);
        int where_to_insert(T value);
        T search_by_index(int index); //returns the key's value
        void remove(T value);
        // void remove_last();
        int contains(T value);
        void print_list(); // maybe replace with
        int length();
        //friend wostream& operator<< <T>(wostream& os, llist<T>& ll)

    private:
        // struct lnode;
        // typedef lnode* lnode*;
        struct lnode {
            lnode* prev;
            T value;
            lnode* next;
        };
        int length_rec(lnode* head);
        void print_list_rec(lnode* head);
        int insert_item_rec(T value, lnode* head, int index);
        int where_to_insert_item_rec(T value, lnode* head, int index);
        int contains_rec(T value, lnode* head, int index);
        void teardown(lnode* head); //for the deconstructor
        lnode* search(T value);
        lnode* head;
        lnode* tail;
};

template <class T>
llist<T>::llist()
{
    head = (lnode*) malloc(sizeof(struct lnode));
    head->prev = NULL;
    //head->value = 0;
    head->next = NULL;
    tail = head; 
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
    if (head->next != NULL) 
    {
        std::cout << head->next->value << ' ';
        print_list_rec(head->next);
    }
}

template <class T>
int llist<T>::length()
{
    return length_rec(head);
}

template <class T>
int llist<T>::length_rec(lnode* head)
{
    if (head->next != NULL) 
    {
        return length_rec(head->next) + 1;
    } 
    else 
    {
        return 0;
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
        tail = new_node;
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
int llist<T>::contains(T value)
{
    return contains_rec(value, head, 0);
}

template <class T>
int llist<T>::contains_rec(T value, lnode* head, int index)
{
    lnode* current = head;
    if (current->next == NULL) //new last node
    {
        return -1;
    }
    if (current->next->value == value) //new node inserted before next node
    {
        return index;
    }
    return contains_rec(value, current->next, index+1);
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
        if (to_remove->next == NULL) // removing last node in list
        {
            to_remove->prev->next = NULL;
            tail = to_remove->prev;
        } 
        else
        {
            to_remove->prev->next = to_remove->next;
            to_remove->next->prev = to_remove->prev;
        }
        free(to_remove);
    }
}

// template <class T>
// void llist<T>::remove_last()
// {
//     lnode* to_remove = tail;
//     to_remove->prev->next = NULL;
//     tail = to_remove->prev;
//     free(to_remove);
// }

#endif