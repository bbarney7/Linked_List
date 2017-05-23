//
//  Linked_List.hpp
//  McAfeeTest
//
//  Created by Brennen Barney on 5/17/17.
//  Copyright © 2017 s. All rights reserved.
//

#ifndef Linked_List_hpp
#define Linked_List_hpp

#include <stdio.h>
#include "Node.hpp"


class Linked_List {
private:
    int length;
    Node * head;
   
public:
    Linked_List();
    void add(Node *); //adds a Node to the beginning of the list
    int get_length(); //returns the length of the list
    void print(); //prints all the numbers in the list
    void sort(); //sorts and removes duplicates
    Node * swap(Node *, Node *); //swaps two nodes - used in sorting
    void sort_helper(Node*, int);
    void delete_duplicates();
    Node * getNode(int);
};

#endif /* Linked_List_hpp */
