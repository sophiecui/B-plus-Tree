#include "bplus.h"
#include <iostream>
#include <queue>
using namespace std;


BTree :: BTree() {

    root = NULL;
    LeafHead = NULL;
    LeafTail = NULL;
}

bool BTree :: search(int target) {

    BTLeaf* traverseNode = searchLeafNode(target);
    if (traverseNode == NULL) {
        return false;
    }
    for (int i = 0; i < traverseNode->getAmount(); i++) {
        if (target == traverseNode->getElement(i)) {
            return true;
        }
    }
    return false;
}

BTLeaf* BTree :: searchLeafNode(int target) {

    BTNode* traverseNode = getRoot();
    while (traverseNode != NULL) {

        if (traverseNode->getType() == NODE_TYPE_LEAF) {
            return (BTLeaf*)traverseNode;
        }
        int i = 0;
        while ( i < traverseNode->getAmount() && target >= traverseNode->getElement(i)) {
            i++;
        }
        traverseNode = traverseNode->getPointer(i);
    }
    return (BTLeaf*)traverseNode;
}

bool BTree :: insertInternalNode(BTInternal* oldInternal, int key, BTNode* son) {

    /*still have space to insert*/
    if ( oldInternal->getAmount() < MAXNUM_KEY) {
        return oldInternal->insert(key, son);
    }
    /*does not have space, split a new internal node out*/
    BTInternal* newInternal = new BTInternal;
    int newKey = oldInternal->split(key, newInternal);
    if (oldInternal->getAmount() < newInternal->getAmount()) {
        oldInternal->insert(key, son);
    } else if (oldInternal->getAmount() > newInternal->getAmount()) {
        newInternal->insert(key, son);
    } else {
        newInternal->insert(0, son);
    }

    BTInternal* parent = (BTInternal*)(oldInternal->getParent());

    /*leaf node is also the root node*/
    if (parent == NULL) {
        parent = new BTInternal;   								/*new root*/
        parent->setPointer(0, oldInternal);                		// first one set to old target leaf node
        parent->setElement(0, newKey);                           	// set key value
        parent->setPointer(1, newInternal);                   	// second one point to new leaf node
        oldInternal->setParent(parent);                       	// set parent root value
        newInternal->setParent(parent);
        parent->setAmount(1);
        setRoot(parent);                                      	//set new root
        return true;
    }
    /*parent is not the root node, do recursively*/
    return insertInternalNode(parent, newKey, newInternal);

}

bool BTree :: insert(int target) {

    if (search(target)){
        return false;  /*element is already in the tree*/
    }
    BTLeaf* targetLeafNode = searchLeafNode(target);

    /*case #1 leaf node has space, either the tree is empty*/
    if (targetLeafNode == NULL) {
        targetLeafNode = new BTLeaf;
        LeafHead = targetLeafNode;
        LeafTail = targetLeafNode;
        setRoot(targetLeafNode);
    }
    if (targetLeafNode->getAmount() < MAXNUM_KEY) {
        return targetLeafNode->insert(target);
    }

    /*case #2 leaf node doest not have space, but parent still has space*/
    BTLeaf* newLeafNode = new BTLeaf;
    int key = targetLeafNode->split(newLeafNode);
    if (target < key) {
        /*insert into old leaf node*/
        targetLeafNode->insert(target);
    } else {
        /*insert into new leaf node*/
        newLeafNode->insert(target);
    }
    BTLeaf* oldNext = targetLeafNode->getNext();
    targetLeafNode->setNext(newLeafNode);
    newLeafNode->setNext(oldNext);
    newLeafNode->setPrevious(targetLeafNode);
    if (oldNext == NULL) {
        LeafTail = newLeafNode;
    } else {
        oldNext->setPrevious(newLeafNode);
    }

    BTInternal* Oldparent = (BTInternal*)(targetLeafNode->getParent());

    /*leaf node is also the root node*/
    if (Oldparent == NULL) {
        BTNode* newInternal = new BTInternal;   /*new root*/
        newInternal->setPointer(0, targetLeafNode);                // first one set to old target leaf node
        newInternal->setElement(0, key);                           // set key value
        newInternal->setPointer(1, newLeafNode);                   // second one point to new leaf node
        newLeafNode->setParent(newInternal);                       // set parent root value
        targetLeafNode->setParent(newInternal);
        newInternal->setAmount(1);
        setRoot(newInternal);                                      //set new root
        return true;
    }
    /*parent is not the root node, do recursively*/
    return insertInternalNode(Oldparent, key, newLeafNode);
}

bool BTree :: removeInternalNode(BTInternal* targetInternal, int target) {
    /*remove target key in this internal node*/
    targetInternal->remove(target);
    BTInternal* parent = (BTInternal*) targetInternal->getParent();
    if (parent == NULL) {
        /*This node is the root node*/
        if (targetInternal->getAmount() == 0) {
            setRoot(targetInternal->getPointer(0));
            root->setParent(NULL);
            delete targetInternal;
        }
        return true;
    }
    /*This node is not the root node*/
    /*case#1 After deletion, the amount of targetInternal is still more than V*/
    /*just update parent's key value*/
    if (targetInternal->getAmount() >= V) {
        /*check if we need to update the key value in the parent*/
        for (int i = 0; i < parent->getAmount(); i++) {
            if (parent->getElement(i) == target) {
                /*update the key into the first element in its child*/
                parent->setElement(i, targetInternal->getElement(0));
                break;
            }
        }
        return true;
    }
    /*case#2 after deletion, there are not enough nodes in this target node*/
    /*case#2.1 borrow node from its neighbor */
    BTInternal* neighbor = (BTInternal*)targetInternal->getNeighbor();
    bool isLeftNeighbor = false;
    if (neighbor->getElement(0) < targetInternal->getElement(0)) {
        /*neighbor is on its left*/
        isLeftNeighbor = true;
    }

    if (neighbor->getAmount() > V) {

        if (isLeftNeighbor) {
            /*neighbor is on its left*/
            /*make the space for the first element*/
            for (int i = targetInternal->getAmount(); i > 0; i--) {
                targetInternal->setElement(i, targetInternal->getElement(i - 1));
            }
            for (int i = targetInternal->getAmount() + 1; i > 0; i--) {
                targetInternal->setPointer(i, targetInternal->getPointer(i - 1));
            }
            targetInternal->setElement(0, targetInternal->getPointer(0)->getElement(0));
            targetInternal->setPointer(0, neighbor->getPointer(neighbor->getAmount()));
            targetInternal->getPointer(0)->setParent(targetInternal);
            targetInternal->setAmount(targetInternal->getAmount() + 1);
            neighbor->setElement(neighbor->getAmount() - 1, NULL);
            neighbor->setPointer(neighbor->getAmount(), NULL);
            neighbor->setAmount(neighbor->getAmount() - 1);
            /*update key in parent*/
            for (int i = 0; i <= parent->getAmount(); i++) {
                if (parent->getPointer(i) == targetInternal && i > 0) {
                    parent->setElement(i - 1, targetInternal->getPointer(0)->getElement(0));
                }
            }

        } else {
            /*neighbor is on its right*/
            targetInternal->setElement(targetInternal->getAmount(), neighbor->getPointer(0)->getElement(0));
            targetInternal->setPointer(targetInternal->getAmount() + 1, neighbor->getPointer(0));
            targetInternal->getPointer(targetInternal->getAmount() + 1)->setParent(targetInternal);
            /*move element int neighbot*/
            for (int i = 0; i < neighbor->getAmount() - 1; i++) {
                neighbor->setElement(i, neighbor->getElement(i + 1));
            }
            for (int i = 0; i < neighbor->getAmount(); i++) {
                neighbor->setPointer(i, neighbor->getPointer(i + 1));
            }
            neighbor->setElement(neighbor->getAmount() - 1, NULL);
            neighbor->setPointer(neighbor->getAmount(), NULL);
            targetInternal->setAmount(targetInternal->getAmount() + 1);
            neighbor->setAmount(targetInternal->getAmount() - 1);
            /*update key in parent for both target and neighbor*/
            for (int i = 0; i <= parent->getAmount(); i++) {

                if (parent->getPointer(i) == targetInternal && i > 1) {
                    parent->setElement(i - 1, targetInternal->getElement(0));
                }
                if (parent->getPointer(i) == neighbor && i > 1) {
                    parent->setElement(i - 1, neighbor->getElement(0));
                }
            }
        }

        return true;
    }
    /*case#2.2 neighbor does not have enough elements, need to merge two node*/
    int newKey = NULL;
    if (isLeftNeighbor) {
        neighbor->merge(targetInternal);
        newKey = targetInternal->getElement(0);
        delete targetInternal;
    } else {
        targetInternal->merge(neighbor);
        newKey = neighbor->getElement(0);
        delete neighbor;

    }
    return removeInternalNode(parent, newKey);
}
bool BTree :: remove(int target) {

    if (!search(target)) {
        return false; /*does not find target element*/
    }
    /*find the target leaf node*/
    /*then delete target from the leaf node*/
    /*find target leaf node's parent*/
    BTLeaf* targetLeafNode = searchLeafNode(target);
    BTInternal* parent = (BTInternal*)targetLeafNode->getParent();
    targetLeafNode->remove(target);
    if (parent == NULL) {
        /*leaf node is also the root node*/
        if (targetLeafNode->getAmount() == 0) {
            setRoot(NULL);
            LeafHead = NULL;
            LeafTail = NULL;
        }
        return true;
    }
    /*leaf is not the root, it has parent*/
    /*case#1 the amount in leaf node is still more or equal to V*/
    if (targetLeafNode->getAmount() >= V) {
        /*check if we need to update the key value in the parent*/
        for (int i = 0; i < parent->getAmount(); i++) {
            if (parent->getElement(i) == target) {
                /*update the key into the first element in its child*/
                parent->setElement(i, targetLeafNode->getElement(0));
                break;
            }
        }
        return true;
    }
    /*case#2 if leaf node does not have enough element */
    /*case#2.1 borrow elements from its neighor*/
    /*first get element from its left, then try to get its neighbor from right*/
    BTLeaf* neighbor = (BTLeaf*)targetLeafNode->getNeighbor();
    bool isLeftNeighbor = false;
    if (neighbor->getElement(0) < target) {
        isLeftNeighbor = true;
    }
    if (neighbor->getAmount() > V) {

        if (isLeftNeighbor) {
            /*neighbor want to borrow from is on left*/
            /*borrow the last one from the left neigbor*/
            int movedData = neighbor->getElement(neighbor->getAmount() - 1);
            targetLeafNode->insert(movedData);
            neighbor->remove(movedData);
            /*update the key value in the parent, targetNode's index key need to changed*/
            for (int i = 0; i <= parent->getAmount(); i++) {
                if (i > 0 && parent->getPointer(i) == targetLeafNode) {
                    parent->setElement(i - 1, targetLeafNode->getElement(0));
                }
            }
        } else {
            /*neighbor want to borrow from is on the right*/
            /*borrow the first one from the right neighbor*/
            int movedData = neighbor->getElement(0);
            targetLeafNode->insert(movedData);
            neighbor->remove(movedData);
            /*need to updateneighbor's parent index key value*/
            for (int i = 0; i <= parent->getAmount(); i++) {

                if (i > 0 && parent->getPointer(i) == targetLeafNode) {
                    parent->setElement(i - 1, targetLeafNode->getElement(0));
                }
                if (i > 0 && parent->getPointer(i) == neighbor) {
                    parent->setElement(i - 1, neighbor->getElement(0));
                }
            }

        }
        return true;
    }
    /*case#2.2 if its neighor does not have enough elements, need to do the merge()*/
    int newKey = NULL;
    if (isLeftNeighbor) {
        /*merge with its left neighbor, leave the neighbor*/
        neighbor->merge(targetLeafNode);
        newKey = targetLeafNode->getElement(0);

        BTLeaf* oldNext = targetLeafNode->getNext();
        neighbor->setNext(oldNext);
        if (oldNext == NULL) {
            /*targetLeafNode is the tail node*/
            LeafTail = neighbor;
        } else {
            oldNext->setPrevious(neighbor);
        }
        delete targetLeafNode;

    } else {
        /*merge with its right neighbor, leave the oldNode, delete the neighbor*/
        targetLeafNode->merge(neighbor);
        newKey = neighbor->getElement(0);
        BTLeaf* oldNext = neighbor->getNext();
        targetLeafNode->setNext(oldNext);
        if (oldNext == NULL) {
            /*targetLeafNode is the tail node*/
            LeafTail = targetLeafNode;
        } else {
            oldNext->setPrevious(targetLeafNode);
        }
        delete neighbor;
    }

    return removeInternalNode(parent, newKey);
}

void BTree :: printTreeLeafNode() {

    BTLeaf* traverseNode = LeafHead;
    while (traverseNode != NULL) {

        for(int i = 0; i < traverseNode->getAmount(); i++) {
            printf("%3d ", traverseNode->getElement(i));
        }
        printf("\n");
        traverseNode = traverseNode->getNext();
    }
}

void BTree :: printNode(BTNode* node) {

    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->getAmount(); i ++) {
        printf("%d  ", node->getElement(i));
    }

}

void BTree :: printTree() {

    int index = 1;
    queue<BTNode*> treequeue;
    BTNode* traverseNode = getRoot();
    if (traverseNode != NULL) {
        treequeue.push(traverseNode);
    }

    while (!treequeue.empty()) {

        int size = treequeue.size();
        printf("the %d level has node as below: \n", index);
        for (int i = 0; i < size; i++) {
            BTNode* cur = treequeue.front();
            treequeue.pop();
            for (int j = 0; j <= cur->getAmount(); j++) {
                if (cur->getPointer(j) != NULL) {
                    treequeue.push(cur->getPointer(j));
                }
            }
            printf("the %d node is: ", i + 1);
            printNode(cur);
            printf(" || ");
        }
        printf("\n");
        index++;
    }

}
