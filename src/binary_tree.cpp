#include <stdio.h>
#include <stdlib.h>

#include "custom_asserts.h"
#include "binary_tree.h"

int main() {
    BinaryTree<int>    int_tree;
    BinaryTree<double> double_tree;
    BinaryTree<char*>  char_tree;
    TreeInit(&int_tree);
    PrintTree(int_tree.root_node);
}

template <typename T>
TreeErrors TreeInit(BinaryTree<T>* tree) {
    check_expression(tree, NODE_POINTER_IS_NULL);

    tree->number_of_nodes = 0;
    tree->root_node       = NULL;
    T root_node_value     = {};
    CreateNode<T>(&(tree->root_node), root_node_value);

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors CreateNode(TreeNode<T>** node, T value) {
    check_expression(node, NODE_POINTER_IS_NULL);

    *node = (TreeNode<T>*)calloc(1, sizeof(TreeNode<T>));
    warning(node, NODE_CALLOC_ERROR);

    (*node)->value       = value;
    (*node)->left_node   = NULL;
    (*node)->right_node  = NULL;
    (*node)->parent_node = NULL;

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors ConnectNodes(TreeNode<T>* parent_node, TreeNode<T>* child_node, int connection_side) {
    check_expression(parent_node, NODE_POINTER_IS_NULL);
    check_expression(child_node , NODE_POINTER_IS_NULL);

    child_node->parent_node     = parent_node;

    if(connection_side > 0) {
        parent_node->right_node = child_node;
    }
    else if(connection_side < 0) {
        parent_node->left_node  = child_node;
    }
    else {
        // color_printf(RED_TEXT, BOLD, "Wrong choice of connection side. It can't be equal to ZERO");

        check_expression(false, NODES_CONNECTION_ERROR);
    }

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors VerifyTree(BinaryTree<T>* tree) {
    return NO_TREE_ERRORS;
}

//This function takes node as argument to add ability
//   of print not only the full tree but also subtree
void PrintTree(TreeNode<int>* node) {
    if(!node) return;

    printf("{");
    printf("%d", node->value);
    if(node->left_node)  PrintTree(node->left_node);
    if(node->right_node) PrintTree(node->right_node);
    printf("}");
}

void PrintTree(TreeNode<double>* node) {
    if(!node) return;

    printf("{");
    printf("%lf", node->value);
    if(node->left_node)  PrintTree(node->left_node);
    if(node->right_node) PrintTree(node->right_node);
    printf("}");
}

void PrintTree(TreeNode<char*>* node) {
    if(!node) return;

    printf("{");
    printf("%s", node->value);
    if(node->left_node)  PrintTree(node->left_node);
    if(node->right_node) PrintTree(node->right_node);
    printf("}");
}
