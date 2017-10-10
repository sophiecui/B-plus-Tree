
#include "stdio.h"
#define NDEBUG
#include <assert.h>
#include "bplus.h"

BTNode :: BTNode() {

    my_type = NODE_TYPE_LEAF;
    amount = 0;
    parent = NULL;
}

BTNode :: ~BTNode(){

    recursiveDelete();

}

void BTNode :: recursiveDelete() {

    for (int i = 0; i <= amount; i++) {
        BTNode * node = getPointer(i);
        if (node != NULL) {
            node->recursiveDelete();
        }
        delete node;
    }

}

NODE_TYPE BTNode :: getType() {

    return my_type;
}

int BTNode :: getAmount() {

    return amount;
}

void BTNode :: setAmount(int change) {

    amount = change;
}

BTNode* BTNode :: getParent() {

    return parent;
}

void BTNode :: setParent(BTNode* father) {

    parent = father;
}

BTNode* BTNode :: getNeighbor() {

    BTNode* parent = this->getParent();
    BTNode* neighbor = NULL;
    for (int i = 0; i <= parent->getAmount(); i++) {
        if (parent->getPointer(i) ==  this) {
            if (i == 0) { /*this is the first one child of the parent*/
                return parent->getPointer(i + 1);
            } else if (i == parent->getAmount()){ /*this is the last child of the parent*/
                return parent->getPointer(i - 1);
            } else if (parent->getPointer(i - 1)->getAmount() > V){
                return parent->getPointer(i - 1);
            } else {
                return parent->getPointer(i + 1);
            }
        }
    }
    return neighbor;
}

BTInternal :: BTInternal() {

    my_type = NODE_TYPE_INTERNAL;

    for (int i = 0; i < MAXNUM_KEY; i++) {
        all_keys[i] = 0;
    }
    for (int i = 0; i < MAXNUM_KEY; i++) {
        all_pointers[i] = NULL;
    }
}

BTInternal :: ~BTInternal() {

    for (int i = 0; i < MAXNUM_POINTER; i++) {
        all_pointers[i] = NULL;
    }

}

int BTInternal :: getElement(int index) {

    assert(index < 0 && "index is smaller that 0");
    assert(index >= MAXNUM_KEY && "index is bigger than supposed");
    if (index < 0 || index >= MAXNUM_KEY) {
        return NULL;
    }
    return all_keys[index];

}
void BTInternal :: setElement(int index, int value) {

    assert(index < 0 && "index is smaller that 0");
    assert(index >= MAXNUM_KEY && "index is bigger than supposed");

    if (index < 0 || index >= MAXNUM_KEY) {
        return ;
    }
    all_keys[index] = value;

}

BTNode* BTInternal :: getPointer(int index) {

    assert(index < 0 && "index is smaller that 0");
    assert(index >= MAXNUM_POINTER && "index is bigger than supposed");

    if ( index >= 0 && index <= amount) {
        return all_pointers[index];
    }
    return NULL;
}

void BTInternal :: setPointer(int index, BTNode* pointer) {

    assert(index < 0 && "index is smaller that 0");
    assert(index >= MAXNUM_POINTER && "index is bigger than supposed");
    if ( index >= 0 && index < MAXNUM_POINTER) {
        all_pointers[index] = pointer;
    }
}
bool BTInternal :: insert(int key, BTNode* node) {

    assert(amount >= MAXNUM_KEY && "Internal Node is full");
    if (amount >= MAXNUM_KEY) {
        return false;
    }

    int j = 0;
    int i = 0;
    // find where to insert, insert at index == i
    while ( i < amount && key > all_keys[i]) {
        i++;
    }
    // move keys in the part after i
    for (j = amount; j > i; j--) {
        all_keys[j] = all_keys[j - 1];
    }
    // move pointers in the part after i
    for (j = amount + 1; j > i + 1; j--) {
        all_pointers[j] = all_pointers[j - 1];
    }
    // update current position
    all_keys[i] = key;
    all_pointers[i + 1] = node;
    node->setParent(this);
    amount++;
    return true;
}
int BTInternal :: split(int key, BTInternal* node) {

    int i = 0, j = 0;
    // key is between V and V + 1; After insert, two node have the same number of keys
    if ((key > this->getElement(V - 1)) && (key < this->getElement(V))) {
        for (i = V; i < MAXNUM_KEY; i++) {

            node->setElement(j, this->getElement(i));
            this->setElement(i, NULL);
            j++;
        }
        j = 1;
        for (i = V + 1; i < MAXNUM_POINTER; i++) {
            this->getPointer(i)->setParent(node);
            node->setPointer(j, this->getPointer(i));
            this->setPointer(i, NULL);
            j++;
        }
        this->setAmount(V);
        node->setAmount(V);
        return key;
    }

    // Key is belong to the first part or the second part of the old Internal Node
    int position = 0;
    if (key < this->getElement(V - 1)) {
        position = V - 1;
    } else {
        position = V;
    }
    int newKey = this->getElement(position);

    // move keys from positon + 1 to end
    j = 0;
    for (i = position + 1; i < MAXNUM_KEY; i++) {
        node->setElement(j, this->getElement(i));
        this->setElement(i, NULL);
        j++;
    }
    // move pointers from positon + 1 to end
    j = 0;
    for (i = position + 1; i < MAXNUM_POINTER; i++) {

        this->getPointer(i)->setParent(node);
        node->setPointer(j, this->getPointer(i));
        this->setPointer(i, NULL);
        j++;
    }

    this->setElement(position, NULL);
    this->setAmount(position);
    node->setAmount(MAXNUM_KEY - position - 1);
    return newKey;

}

void BTInternal :: remove(int target) {

    int i = 0;
    for (i = 0; i < amount; i++) {
        if (target  >= all_keys[i]) {
            continue;
        } else {
            break;
        }
    }
    int j = 0;
    for (j = i - 1; j < amount - 1; j++) {
        all_keys[j] = all_keys[j + 1];
    }
    all_keys[j] = NULL;
    for (j = i; j < amount; j++) {
        all_pointers[j] = all_pointers[j + 1];
    }
    all_pointers[j] = NULL;
    amount--;
    return;
}

void BTInternal :: merge(BTInternal* node) {

    assert(amount + node->getAmount() + 1 > MAXNUM_KEY && "amount is more than supposed");
    /*node's first element is as the new key in the current node*/
    all_keys[amount] = node->getPointer(0)->getElement(0);
    amount++;
    all_pointers[amount] = node->getPointer(0);
    this->getPointer(amount)->setParent(this);
    for (int i = 0; i < node->getAmount(); i++) {
        all_keys[amount] = node->getElement(i);
        amount++;
        all_pointers[amount] = node->getPointer(i + 1);
        this->getPointer(amount)->setParent(this);
    }
    return;

}

BTLeaf :: BTLeaf() {

    previousNode = NULL;
    nextNode = NULL;
    my_type = NODE_TYPE_LEAF;
    for (int i = 0; i < MAXNUM_KEY; i++) {
        all_data[i] = NULL;
    }

}

BTLeaf :: ~BTLeaf() {

}


int BTLeaf :: getElement(int index) {

    assert(index < 0 && "index is smaller that 0");
    assert(index >= amount && "index is bigger than supposed");
    return all_data[index];
}
void BTLeaf :: setElement(int index, int value) {

    assert(index < 0 && "index is smaller that 0");
    assert(index >= amount && "index is bigger than supposed");
    all_data[index] = value;
}
/*meanless for leaf node*/
BTNode* BTLeaf :: getPointer(int index) {

    return NULL;
}
/*meanless for leaf node*/
void BTLeaf :: setPointer(int index, BTNode* pointer) {

    return;
}

bool BTLeaf :: insert(int key) {

    assert(amount >= MAXNUM_KEY && "Leaf Node is full");
    if (amount >= MAXNUM_KEY) {
        return false;
    }
    int i = 0;
    int j = 0;
    // find where to insert, insert at index == i
    while ( i < amount && key > all_data[i]) {
        i++;
    }
    // move keys in the part after i
    for (j = amount; j > i; j--) {
        all_data[j] = all_data[j - 1];
    }
    // update current position
    all_data[i] = key;
    amount++;
    return true;
}
int BTLeaf :: split(BTLeaf* newLeafNode) {

    int index = amount / 2;
    int j = 0;
    for (int i = index; i < amount; i++) {
        newLeafNode->setElement(j, this->getElement(i));
        this->setElement(i, NULL);
        j++;
    }
    this->setAmount(amount - j);
    newLeafNode->setAmount(j);
    return newLeafNode->getElement(0);
}

void BTLeaf :: remove(int target) {

    int i = 0;
    while (i < amount && all_data[i] != target) {
        i++;
    }
    /*i is the position to remove*/
    int j = i;
    while (j < amount - 1) {
        all_data[j] = all_data[j + 1];
        j++;
    }
    all_data[j] = NULL;
    amount = amount - 1;
}
void BTLeaf :: merge(BTLeaf* leaf) {

    assert(amount + leaf->getAmount() >= MAXNUM_KEY && "SUM OF AMOUNT is more than supposed");

    for(int i = 0; i < leaf->getAmount(); i++) {
        this->insert(leaf->getElement(i));
    }

}


BTLeaf* BTLeaf :: getNext() {

    return nextNode;
}
void BTLeaf :: setNext(BTLeaf* node) {

    nextNode = node;
}
BTLeaf* BTLeaf :: getPrevious() {

    return previousNode;
}
void BTLeaf :: setPrevious(BTLeaf* node) {

    previousNode = node;
}




