#include <stdio.h>
#include <stdlib.h>

#include "custom_asserts.h"
#include "binary_tree.h"

int main() {

}

template <typename T>
TreeErrors TreeInit(BinaryTree<T>* tree) {
    check_expression(tree, POINTER_IS_NULL);

    tree->number_of_nodes = 0;
    tree->root_node       = NULL;
    T root_node_value     = {};
    CreateNode<T>(*(tree->root_node), root_node_value);

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors CreateNode(TreeNode<T>* node, T value) {
    check_expression(node, POINTER_IS_NULL);

    node = (TreeNode<T>*)calloc(1, sizeof(TreeNode<T>));
    warning(node, CALLOC_ERROR);

    node->value       = value;
    node->left_node   = NULL;
    node->right_node  = NULL;
    node->parent_node = NULL;

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors ConnectNodes(TreeNode<T>* parent_node, TreeNode<T>* child_node, int connection_side) {
    check_expression(parent_node, POINTER_IS_NULL);
    check_expression(child_node , POINTER_IS_NULL);

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

