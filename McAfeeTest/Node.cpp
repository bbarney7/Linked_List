//
//  Node.cpp
//  McAfeeTest
//
//  Created by Brennen Barney on 5/17/17.
//  Copyright © 2017 s. All rights reserved.
//

#include "Node.hpp"


Node::Node(Node * nextNode, int data, Node * previous ){
    next = nextNode;
    num = data;
    prev = previous;
}

