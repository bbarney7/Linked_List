//
//  Node.hpp
//  McAfeeTest
//
//  Created by Brennen Barney on 5/17/17.
//  Copyright © 2017 s. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>

class Node{
public:
    Node(Node *, int, Node *);
    Node * next;
    Node * prev;
    int num;
    bool isHead;
    int length;
};

#endif /* Node_hpp */
