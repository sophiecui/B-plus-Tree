#include <stdio.h>
#include "bplus.h"

//search
void Test1(BTree* pTree, int data) {

    if (pTree->search(data) ) {
        printf("\nfind data\n");
        return;
    }
    printf("\nfailed\n");
}
//insert
void Test2(BTree* pTree, int data) {
    bool success = pTree->insert(data);
    if (true == success) {
        printf("\nsuccessed!\n");
    } else {
        printf("\nfailed!\n");
    }
}
//delete
void Test3(BTree* pTree, int data) {
    bool success = pTree->remove(data);
    if (true == success)
    {
        printf("\nsuccessed!\n");
    }
    else
    {
        printf("\nfailed!\n");
    }
}
//print
void Test4(BTree* pTree) {
    pTree->printTree();
}
//build from 1 to setNUM
void Test5(BTree* pTree) {
    int i = 1;
    int setNUM = 20;
    while (i <= setNUM) {
        pTree->insert(i);
        i++;
    }
}


int main(int argc, char* argv[])
{
    BTree* tree = new BTree;

    int x = 1;
    int y = 0;

    while (0 != x) {

        printf("\n");
        printf("    *How to use the B+ tree: 1:search 2:insert 3:delete 4:print 5:build 0:exit*\n");
        printf("    command:");
        scanf("%d", &x);
        switch (x) {

            case 1:
                printf("input search num:");
                scanf("%d", &y);
                Test1(tree, y);
                break;
            case 2:
                printf("input insert num:");
                scanf("%d", &y);
                Test2(tree, y);
                break;
            case 3:
                printf("input delete num:");
                scanf("%d", &y);
                Test3(tree, y);
                break;
            case 4:
                Test4(tree);
                break;
            case 5:
                Test5(tree);
                break;
            case 0:
                delete tree;
                return 0;
                break;
            default:
                break;
        }

    }

    //Test5(tree);
    delete tree;
    return 0;
}
