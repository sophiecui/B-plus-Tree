#include <stdio.h>
#include <iostream>
#include "bplus.h"
using namespace std;

#define SINGLE_TEST

//search
void testSearch(BTree* tree, int data) {

    if (tree->search(data)) {
        printf("\nsearch %d successfully\n", data);
    } else {
        printf("\nsearch %d FAILED!\n", data);
    }
}
//insert
void testAdd(BTree* tree, int data) {
    
    if (tree->insert(data)) {
        printf("\ninsert %d successfully\n", data);
    } else {
        printf("\ninsert %d FAILED!\n", data);
    }
}
//delete
void testDelete(BTree* tree, int data) {
    
    if (tree->remove(data)) {
        printf("\ndelete %d successfully\n", data);
    } else {
        printf("\ndelete %d FAILED!\n", data);
    }
}
//print
void printTree(BTree* tree) {
    tree->printTree();
}
//build from 1 to setNUM
void buildTree(BTree* tree) {
    int i = 1;
    int setNUM = 13;
    while (i <= setNUM) {
        tree->insert(i);
        i++;
    }
}




int main(int argc, char* argv[]) {
    
    BTree* tree = new BTree;
    #ifdef SINGLE_TEST
    int keys[] = {97, 1, 93, 91, 90, 22, 64, 52, 48, 47, 13, 34, 27, 21, 20, 14, 8, 7, 98};
    int length = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < length; i++) {
        testAdd(tree, keys[i]);
    }
    for (int i = 0; i < length; i++) {
        testDelete(tree, keys[i]);
        printTree(tree);
    }
    #endif
    delete tree;
    return 0;
}
