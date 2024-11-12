#include <stdio.h>
#include <stdlib.h>

#include "custom_asserts.h"
#include "binary_tree.h"

int main() {
    Tree<int>    int_tree    = {};
    Tree<double> double_tree = {};
    Tree<char*>  char_tree   = {};
    char* hui = (char*)calloc(5, 1);
    char* ded = (char*)calloc(5, 1);

    TreeInit(&int_tree);
    int_tree.root_node->value = 50;
    PrintTree(int_tree.root_node);
    printf("\n");
    AddNode(int_tree.root_node, 20, LEFT_NODE);
    AddNode(int_tree.root_node, 70, RIGHT_NODE);
    AddNode(int_tree.root_node->right_node, 80, RIGHT_NODE);
    AddNode(int_tree.root_node->right_node, 60, LEFT_NODE);
    AddNode(int_tree.root_node->right_node->left_node, 55, LEFT_NODE);
    AddNode(int_tree.root_node->right_node->right_node, 75, LEFT_NODE);
    PrintTree(int_tree.root_node);
    printf("\n");
    TreeDestruct(&int_tree);

    TreeInit(&double_tree);
    double_tree.root_node->value = 1.501;
    PrintTree(double_tree.root_node);
    printf("\n");
    AddNode(double_tree.root_node, 2.4350, LEFT_NODE);
    AddNode(double_tree.root_node, 7.270, RIGHT_NODE);
    AddNode(double_tree.root_node->right_node, 8.70, RIGHT_NODE);
    AddNode(double_tree.root_node->right_node, 0.60, LEFT_NODE);
    AddNode(double_tree.root_node->right_node->left_node, 0.55, LEFT_NODE);
    AddNode(double_tree.root_node->right_node->right_node, 7.5, LEFT_NODE);
    PrintTree(double_tree.root_node);
    printf("\n");
    TreeDestruct(&double_tree);

    scanf("%s", hui);
    scanf("%s", ded);
    TreeInit(&char_tree);
    char_tree.root_node->value = hui;
    PrintTree(char_tree.root_node);
    printf("\n");
    AddNode(char_tree.root_node, ded, LEFT_NODE);
    PrintTree(char_tree.root_node);
    printf("\n");
    TreeDestruct(&char_tree);
    //TODO same pointer in two nodes
}

template <typename T>
TreeErrors TreeInit(Tree<T>* tree) {
    check_expression(tree, NODE_POINTER_IS_NULL);

    tree->number_of_nodes = 0;
    tree->root_node       = NULL;
    T root_node_value     = {};
    CreateNode<T>(&(tree->root_node), root_node_value);

    return NO_TREE_ERRORS;
}

template <typename T> //TODO up number_of_nodes
TreeErrors AddNode(TreeNode<T>* node, T value, int connection_side) {
    check_expression(node, NODE_POINTER_IS_NULL);

    TreeNode<T>* new_node = NULL;
    CreateNode(&new_node, value);
    LinkNodes(node, new_node, connection_side);

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
TreeErrors LinkNodes(TreeNode<T>* parent_node, TreeNode<T>* child_node, int connection_side) {
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
        color_printf(RED_TEXT, BOLD, "Wrong choice of connection side. It can't be equal to ZERO");

        check_expression(false, NODES_CONNECTION_ERROR);
    }

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors TreeDestruct(Tree<T>* tree) {
    check_expression(tree, TREE_POINTER_IS_NULL);

    NodesDestruct(&(tree->root_node));

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors NodesDestruct(TreeNode<T>** node) {
    check_expression(node, TREE_POINTER_IS_NULL);

    if((*node)->left_node ) NodesDestruct(&((*node)->left_node));
    if((*node)->right_node) NodesDestruct(&((*node)->right_node));

    free(*node);
    *node = NULL;

    return NO_TREE_ERRORS;
}

TreeErrors NodesDestruct(TreeNode<char*>** node) {
    check_expression(node, TREE_POINTER_IS_NULL);

    if((*node)->left_node ) NodesDestruct(&((*node)->left_node));
    if((*node)->right_node) NodesDestruct(&((*node)->right_node));

    free((*node)->value);
    (*node)->value = NULL;
    free(*node);
    *node          = NULL;

    return NO_TREE_ERRORS;
}

template <typename T> //TODO
TreeErrors VerifyTree(Tree<T>* tree) {
    return NO_TREE_ERRORS;
}

//This function takes node as argument to add ability
//   of print not only the full tree but also subtree
void PrintTree(TreeNode<int>* node) {
    check_expression(node, (void)NODE_POINTER_IS_NULL);

    printf("(");
    printf("%d", node->value);
    if(node->left_node ) PrintTree(node->left_node );
    if(node->right_node) PrintTree(node->right_node);
    printf(")");
}

void PrintTree(TreeNode<double>* node) {
    check_expression(node, (void)NODE_POINTER_IS_NULL);

    printf("(");
    printf("%.3lf", node->value);
    if(node->left_node ) PrintTree(node->left_node );
    if(node->right_node) PrintTree(node->right_node);
    printf(")");
}

void PrintTree(TreeNode<char*>* node) {
    check_expression(node, (void)NODE_POINTER_IS_NULL);

    printf("(");
    printf("%s", node->value);
    if(node->left_node ) PrintTree(node->left_node );
    if(node->right_node) PrintTree(node->right_node);
    printf(")");
}
