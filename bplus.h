#ifndef BPLUS_BPLUS_H
#define BPLUS_BPLUS_H

#endif //BPLUS_BPLUS_H
#include "stdio.h"
#include "stdlib.h"

#define V 2    /* set V */
#define MAXNUM_KEY (V * 2)                  /* max number of key is 2v */
#define MAXNUM_POINTER (MAXNUM_KEY + 1)       /* max number of child is 2v + 1*/

enum NODE_TYPE {

    NODE_TYPE_INTERNAL = 1,    // represent internal node
    NODE_TYPE_LEAF = 2,    // represent leaf node
};

class BTNode {

public:
    BTNode();
    virtual ~BTNode();
    NODE_TYPE getType();
    int getAmount();
    void setAmount(int change);
    virtual int getElement(int index) {return 0;}
    virtual void setElement(int index, int value){}
    virtual BTNode* getPointer(int index) {return NULL;}
    virtual void setPointer(int index, BTNode* pointer) {}
    BTNode* getParent();
    void setParent(BTNode* father);
    BTNode* getNeighbor();
protected:
    void recursiveDelete();
    NODE_TYPE my_type;
    int amount;
    BTNode* parent;
};

class BTInternal : public BTNode  {

public:
    BTInternal();
    virtual ~BTInternal();
    int getElement(int index);
    void setElement(int index, int value);
    BTNode* getPointer(int index);
    void setPointer(int index, BTNode* pointer);
    bool insert(int key, BTNode* node);
    int split(int key, BTInternal* node);
    void remove(int target);
    void merge(BTInternal* node);
protected:
    int all_keys[MAXNUM_KEY];
    BTNode* all_pointers[MAXNUM_POINTER];

};


class BTLeaf : public BTNode {

public:
    BTLeaf();
    virtual ~BTLeaf();
    int getElement(int index);
    void setElement(int index, int value);
    BTNode* getPointer(int i);
    void setPointer(int i, BTNode* pointer);
    bool insert(int key);
    int split(BTLeaf* node);
    BTLeaf* getNext();
    void setNext(BTLeaf* node);
    BTLeaf* getPrevious();
    void setPrevious(BTLeaf* node);
    void remove(int target);
    void merge(BTLeaf* leaf);
protected:
    int all_data[MAXNUM_KEY];
    BTLeaf* nextNode;
    BTLeaf* previousNode;

};

class BTree {

public:
    BTree();
    BTNode* getRoot() {
        return root;
    }
    void setRoot(BTNode* input) {
        root = input;
    }
    bool insert(int target);
    bool search(int target);
    bool remove(int target);
    void printTree();
protected:
    bool insertInternalNode(BTInternal* oldInternal, int key, BTNode* son);
    bool removeInternalNode(BTInternal* targetInternal, int target);
    BTLeaf* searchLeafNode(int target);
    void printNode(BTNode* node);
    void printTreeLeafNode();
    BTNode* root;
    BTLeaf* LeafHead;
    BTLeaf* LeafTail;
};