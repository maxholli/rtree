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

    // // test llist
    // llist<int> mylist;
    // mylist.print_list();
    // std::cout << mylist.length() << std::endl;
    // // struct lnode* mylist = get_list_head();
    // for (int i=0; i<40; i++)
    // {
    //     mylist.insert_item(rand()%100 -49);
    // }
    // std::cout << mylist.length() << std::endl;
    // mylist.print_list();
    // int search_and_remove = -45;
    // std::cout << mylist.search(search_and_remove) << std::endl;
    // mylist.remove(search_and_remove);
    // mylist.print_list();
    // std::cout << mylist.search_by_index(0) << std::endl;


    // test btree
    // std::cout << "starting btree stuff" << std::endl;
    
    btree newb(4);
    // std::cout << "minimum degree = " << newb.getT() << ' '  << newb.getH() << std::endl;
    
    //fill root with keys
    int n_keys = 200;
    int in_array[n_keys];

    int num_times = 1000;
    for (int i=0; i<num_times; i++)
    {

        for (int i=0; i<n_keys; i++)
        {
            int to_insert = rand()%50000000; 
            newb.insert(to_insert);
            // std::cout << "\n\ndepth-first search" << std::endl;
            // newb.depth_first_5check();
            in_array[i] = to_insert;
        }

        for (int i=0; i<(n_keys); i++)
        {
            // std::cout << "================================================================" << std::endl;
            // newb.depth_first_print();
            // std::cout << "Key to delete: " << in_array[i] << std::endl;
            newb.delete_key(in_array[i]);
            newb.depth_first_5check(1);
            // newb.depth_first_5check(2);
            if (newb.keys_total() != n_keys-(i+1))
            {
                std::cout << "ERROR: you got a problem with removing keys" << std::endl;
            }
        }

    }
    newb.insert(29);
    newb.delete_key(55);

    newb.depth_first_print();
    // newb.depth_first_5check(0);


    // for (int i=0; i<n_keys; i++)
    // {
    //     if (!newb.key_exists(in_array[i]))
    //     {
    //         std::cout << "you got a problem, one of those keys isn't in there" << std::endl;
    //     }
    // }
    
    // for (int i=0; i<500*n_keys; i++)
    // {
    //     int to_check = rand()%5000000;
    //     if (newb.key_exists(to_check))
    //     {
    //         std::cout << "you might have a problem, rand supposably exists in your tree " << to_check << std::endl;
    //         std::cout << "start depth-first search" << std::endl;
    //         newb.depth_first_5check();
    //     }
    // }



    // std::cout << "start depth-first search" << std::endl;
    // newb.depth_first_5check();

    // std::cout << "enter integer to search: ";
    // int to_remove;
    // std::cin >> to_remove;

    // std::cout << std::endl;
    // newb.delete_key(to_remove);
    // newb.depth_first_5check();




    // if (newb.key_exists(to_remove))
    // {
    //     std::cout << "Yes, key " << to_remove << "does exist" << std::endl;
    // }
    // else
    // {
    //     std::cout << "No, key " << to_remove << "doesn't exist" << std::endl;
    // }


}