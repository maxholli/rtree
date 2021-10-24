#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "btree.h"
#include "llist.h"

//todo 
// for btree
//  implement remove
//  implement lookup

int main()
{
    srand(time(NULL));

    // test llist
    llist mylist;
    mylist.print_list();
    // struct lnode* mylist = get_list_head();
    for (int i=0; i<40; i++)
    {
        mylist.insert_item(rand()%100 -49);
    }
    mylist.print_list();
    int search_and_remove = -45;
    std::cout << mylist.search(search_and_remove) << std::endl;
    mylist.remove(search_and_remove);
    mylist.print_list();
    std::cout << mylist.search_by_index(0) << std::endl;


    // test btree
    std::cout << "starting btree stuff" << std::endl;
    
    btree newb(4);
    std::cout << "minimum degree = " << newb.getT() << ' '  << newb.getH() << std::endl;
    
    //fill root with keys
    for (int i=0; i<50; i++)
    {
        newb.insert(rand()%5000);
    }
    
    std::cout << "start depth-first search" << std::endl;
    newb.depth_first_print();

}