#define NO_REPEATS

#include <stdio.h>
#include <stdlib.h>

#include "custom_asserts.h"
#include "tree.h"
#include "tree_dump.h"

int main() {
    Tree<int>    int_tree    = {};
    Tree<double> double_tree = {};
    Tree<char*>  char_tree   = {};
    // char* loh = (char*)calloc(5, 1);
    // char* ded = (char*)calloc(5, 1);

    TreeInit(&int_tree);
    int_tree.root->value = 50;
    AddNode(int_tree.root, 55, LEFT_NODE);
    AddNode(int_tree.root, 70, RIGHT_NODE);
    AddNode(int_tree.root->right, 80, RIGHT_NODE);
    AddNode(int_tree.root->right, 60, LEFT_NODE);
    AddNode(int_tree.root->right->left, 90, LEFT_NODE);
    AddNode(int_tree.root->right->right, 75, LEFT_NODE);
    VerifyNodes(int_tree.root);
    PreorderPrintTree_(int_tree.root);
    TreeDump(&int_tree);
    TreeDtor(&int_tree);
    system("code");

//     TreeInit(&double_tree);
//     double_tree.root->value = 1.501;
//     PrintTree(double_tree.root);
//     printf("\n");
//     AddNode(double_tree.root, 2.4350, LEFT_NODE);
//     AddNode(double_tree.root, 7.270, RIGHT_NODE);
//     AddNode(double_tree.root->right, 8.70, RIGHT_NODE);
//     AddNode(double_tree.root->right, 0.60, LEFT_NODE);
//     AddNode(double_tree.root->right->left, 0.55, LEFT_NODE);
//     AddNode(double_tree.root->right->right, 7.5, LEFT_NODE);
//     PreorderPrintTree_(double_tree.root);
//     TreeDtor(&double_tree);
//
//     scanf("%s", loh);
//     scanf("%s", ded);
//     TreeInit(&char_tree);
//     char_tree.root->value = loh;
//     PrintTree(char_tree.root);
//     printf("\n");
//     AddNode(char_tree.root, ded, LEFT_NODE);
//     PreorderPrintTree_(char_tree.root);
//     printf("\n");
//     TreeDtor(&char_tree);
    //TODO same pointer in two nodes
}
