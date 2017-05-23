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
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

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
    

    key_t key = 9876;
    int seg_id = shmget(key, 1024, IPC_CREAT | 0644);

    
    Node * start = (Node*) shmat(seg_id, (void *)0, 0);
    Node * shared_mem = start;
    bool first = true;
    int length = 0;
    *(shared_mem) = * new Node(NULL, NULL, NULL);
    shared_mem->isHead = true;
    shared_mem++;
    bool correctInput = false;
    while (!correctInput){
        cout << "Please enter a list of integers, each integer should be seperated by a space: ";
        string input;
        getline(cin, input);
        istringstream iss(input);
        int counter = 0;
        for( int s; iss >> s; ){
            counter++;
            if (counter > 115){
                cout<<"Error! Too many numbers entered!"<<endl;
                return 1;
            }
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
        correctInput = true;
        if(!iss.eof()){
            cout <<"You must enter in a list of integers seperated by a space. ";;
            correctInput = false;
            cin.clear();
            iss.str("");
            input = "";
            counter = 0;
        }
        else{
            correctInput = true;
        }
        
    }

    
    if (shmdt(start) == -1){
        perror("shmdt");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0){ //child process
        sort(seg_id, length);
        delete_duplicates(seg_id, length);
    }
    else {
        wait(NULL); // waiting for child process
        Node * shared_mem = (Node*) shmat(seg_id, (void *)0, 0);
        shared_mem = shared_mem->next;
        while(shared_mem != NULL){
            cout<<shared_mem->num<< " ";
            shared_mem = shared_mem->next;
        }
        string in;
        cout<<endl<<"Is the list correct? (enter y or n) ";
        getline(cin, in);
        while (in != "y" && in != "Y" && in != "n" && in != "N"){
            cout<<endl<<"Please enter y or n ";
            in = "";
            getline(cin, in);
            
        }
        if (in == "y" || in == "Y"){
            cout<<"Perfect!"<<endl;
            return 0;
        }
        else{
            cout<<"Sorry I must have made a mistake."<<endl;
            return 0;
        }

        
    }


}
