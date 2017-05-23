//
//  Linked_List.cpp
//  McAfeeTest
//
//  Created by Brennen Barney on 5/17/17.
//  Copyright © 2017 s. All rights reserved.
//

#include "Linked_List.hpp"
#include <string>
#include <sstream>
#include <iostream>

Linked_List::Linked_List(){
    head = new Node(NULL, NULL, NULL);
    length = 0;
    
}

int Linked_List::get_length(){
    return length;
}

//Adds a node to the beginning of the list
void Linked_List::add(Node * newNode){
    
    newNode->next = head;
    newNode->prev = NULL;
    head -> prev = newNode;
    head = newNode;
    length++;

}


void Linked_List::print(){
    std::ostringstream output;
    Node * p = head;
    for (int i = 0; i < length; i++){
        output << p->num;
        output << " ";
        p = p->next;
    }
    std::cout << output.str() << std::endl;
}

void Linked_List::sort(){
    for (int i = 1; i < length; i ++){
        Node * currentNode = head;
        for(int j = 0; j < i; j++){
           currentNode = currentNode->next; 
        }
        
        sort_helper(currentNode, i);
    }
}

void Linked_List::sort_helper(Node* curr, int j){
    int key = curr->num;
    while(j > 0 && key < curr->prev->num){
        curr = swap(curr, curr->prev);
        j--;
        if (j == 0){
            head = curr;
        }
    }
}


Node * Linked_List::swap(Node* curr, Node* prevNode){
    if(prevNode->prev != NULL){
        prevNode->prev->next = curr;
    }
    curr->next->prev = prevNode;
    prevNode->next = curr->next;
    curr->prev = prevNode->prev;
    prevNode->prev = curr;
    curr->next = prevNode;
    return curr;
}

Node * Linked_List::getNode(int index){
    Node * p = head;
    for (int i = 0; i < length; i ++){
        if ( i == index ){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void Linked_List::delete_duplicates(){
    Node * curr = head;
    for (int i = 0; i < length-1; i++){
        curr = head;
        for(int j = 0; j < i; j++){
            curr = curr->next;
        }
        if (curr->next != NULL && curr->num == curr->next->num){
            Node * p = curr->next;
            curr->next = curr->next->next;
            curr->next->prev = curr;
            length --;
            i--;
            delete p;
            p = NULL;
            
        }
        
    }
}



