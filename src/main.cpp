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
    char* loh = (char*)calloc(30, 1);
    char* ded = (char*)calloc(30, 1);
//     TreeDump(&int_tree);
//
//     TreeInit(&int_tree, 50);
//
//     AddNode(int_tree.root, 55, LEFT_NODE);
//
//     TreeDump(&int_tree);
//
//     AddNode(int_tree.root, 70, RIGHT_NODE);
//
//     AddNode(int_tree.root->right, 80, RIGHT_NODE);
//     AddNode(int_tree.root->right, 60, LEFT_NODE);
//     AddNode(int_tree.root->right->left, 90, LEFT_NODE);
//
//     TreeDump(&int_tree);
//
//     AddNode(int_tree.root->right->right, 75, LEFT_NODE);
//     VerifyNodes(int_tree.root);
//     PreorderPrintTree_(int_tree.root);
//
//     TreeDump(&int_tree);
//
//     TreeDtor(&int_tree);

//     TreeInit(&double_tree);
//     double_tree.root->value = 1.501;
//     PrintTree(double_tree.root);
//     printf("\n");
//     AddNode(double_tree.root, 2.4350, LEFT_NODE);
//     AddNode(double_tree.root, 7.270, RIGHT_NODE);
//     AddNode(double_tree.root->right, 8.70, RIGHT_NODE);
//     AddNode(double_tree.root->right, 0.60, LEFT_NODE);
//     TreeDump(&double_tree);
//
//     AddNode(double_tree.root->right->left, 0.55, LEFT_NODE);
//     AddNode(double_tree.root->right->right, 7.5, LEFT_NODE);
//     PreorderPrintTree_(double_tree.root);
//     TreeDump(&double_tree);
//     TreeDtor(&double_tree);

    scanf("%s", loh);
    // printf("%s", loh);
    scanf("%s", ded);
    TreeInit(&char_tree, loh);
    TreeDump(&char_tree);

    AddNode(char_tree.root, ded, LEFT_NODE);
    PreorderPrintTree_(char_tree.root);

    TreeDump(&char_tree);
    TreeDump(&char_tree);
    TreeDump(&char_tree);
    TreeDtor(&char_tree);
}
