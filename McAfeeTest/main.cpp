//
//  main.cpp
//  McAfeeTest
//
//  Created by Brennen Barney on 5/17/17.
//  Copyright © 2017 s. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Node.hpp"
#include "Linked_List.hpp"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>




using namespace std;


//When I had trouble getting my linked list into shared memory I created these structs to see if they'd be easier to get into shared memory.
typedef struct node_obj {
    struct node_obj * next;
    struct node_obj * prev;
    int num;
} node_obj;

typedef struct Linked_List_object{
    struct node_obj * head;
    int length;
    
}Linked_List_object;

Node * swap(Node* curr, Node* prev_node){
    if(prev_node->prev != NULL){
        prev_node->prev->next = curr;
    }
    if (curr->next != NULL){
        curr->next->prev = prev_node;
    }
    prev_node->next = curr->next;
    curr->prev = prev_node->prev;
    prev_node->prev = curr;
    curr->next = prev_node;
    
    
    return curr;
    
}

void sort_helper(Node * curr, int j){
    int key = curr->num;
    while(j > 0 && key < curr->prev->num){
        curr = swap(curr, curr->prev);
        j--;
    }
}

void sort(int seg_id, int length){
    for (int i = 1; i < length; i ++){
        Node * head = (Node*) shmat(seg_id, (void *)0, 0);
        Node * curr_node = head->next;
        for (int j = 0; j < i; j++){
            curr_node = curr_node->next;
        }
        sort_helper(curr_node,i);
        if (shmdt(head) == -1){
            perror("shmdt");
            exit(1);
        }

    }
}

void delete_duplicates(int seg_id, int length){
    for (int i = 0; i < length-1; i++){
        Node * head = (Node*) shmat(seg_id, (void *)0, 0);
        Node * curr_node = head->next;
        for(int j = 0; j < i; j++){
            curr_node= curr_node->next;
        }
        if (curr_node->next != NULL && curr_node->num == curr_node->next->num){
            if (curr_node->next->next == NULL){
                curr_node->next = NULL;
            }
            else{
                curr_node->next = curr_node->next->next;
                curr_node->next->prev = curr_node;
            }
            length --;
            i--;
        }
        if (shmdt(head) == -1){
            perror("shmdt");
            exit(1);
        }
    }
}

int main(int argc, const char * argv[]) {
    
    
    //Reading in numbers from the command line, placing them in the list and sorting the list
//    cout << "Please enter a list of numbers, each number should be seperated by a space: ";
//    string input;
//    getline(cin, input);
//    istringstream iss(input);
//    Linked_List list = Linked_List();
//    for( int s; iss >> s; ){
//        Node* n = new Node(NULL, s);
//        list.add(n);
//    }
    
//    list.sort();
//    list.delete_duplicates();
//    list.print();
    
    
//    //Forking a process and using mapped memory to change an integer.
//    cout<<endl<<"Example of sharing a variable between two processes."<<endl;
//    int * shared_int = (int*)mmap(0, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
//    *shared_int = 5;
//    cout<<"Origional number = "<<*shared_int<<endl;
//    pid_t pid = fork();
//    if (pid == 0){ //child process
//        *shared_int = 50;
//    }
//    else{ //parent process
//        wait(NULL);
//        cout<<"Child process changed number to "<<*shared_int<<endl;
//    }
    
    //Setting up shared memory segment using shmget
    
    
    key_t key = 9876;
    int seg_id = shmget(key, 2048, IPC_CREAT | 0644);

    cout << "Please enter a list of numbers, each number should be seperated by a space: ";
    string input;
    getline(cin, input);
    istringstream iss(input);
    Node * start = (Node*) shmat(seg_id, (void *)0, 0);
    Node * shared_mem = start;
    bool first = true;
    int length = 0;
    *(shared_mem) = * new Node(NULL, NULL, NULL);
    shared_mem->isHead = true;
    shared_mem++;
    for( int s; iss >> s; ){
        if (first){
            first = false;
            *(shared_mem) = * new Node (NULL,s, (shared_mem-1));
            (shared_mem-1)->next = shared_mem;
            shared_mem++;
            length++;
            
        }
        else{
            *(shared_mem) = * new Node(NULL, s, (shared_mem-1));
            (shared_mem-1)->next = shared_mem;
            shared_mem++;
            length++;
        }
    }
    
    if (shmdt(start) == -1){
        perror("shmdt");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0){
        sort(seg_id, length);
        delete_duplicates(seg_id, length);
        cout<<"Done with Child"<<endl;
    }
    else {
        wait(NULL);
        cout<<"Starting Parent"<<endl;
        Node * shared_mem = (Node*) shmat(seg_id, (void *)0, 0);
        shared_mem = shared_mem->next;
        while(shared_mem != NULL){
            cout<<shared_mem->num<< " ";
            shared_mem = shared_mem->next;
        }
    }


}
