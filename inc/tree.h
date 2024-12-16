#include <stdlib.h>
#include <string.h>

#include "custom_asserts.h"
#include "tree_definitions.h"
#include "tree_dump.h"

#ifndef TREE_H_
#define TREE_H_

//FUNCTION DECLARATION-----------------------------------------------------------------------

template <typename T>
TreeErrors TreeInit(Tree<T>* tree, const T* root_value) {
    check_expression(tree, NODE_POINTER_IS_NULL);

    tree->error           = NO_TREE_ERRORS;
    tree->root            = NULL;
    tree->dump_svg_file   = NULL;
    tree->dump_html_file  = NULL;
    CreateNode<T>(&(tree->root), root_value);
    SetDumpFile(tree);

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors AddNode(TreeNode<T>* node, const T* value, int connection_side) {
    check_expression(node, NODE_POINTER_IS_NULL);

    TreeNode<T>* new_node = NULL;
    CreateNode(&new_node, value);
    LinkNodes(node, new_node, connection_side);

    return NO_TREE_ERRORS;
}

template <typename T>
inline TreeErrors CreateNode(TreeNode<T>** node, const T* value) {
    check_expression(node, NODE_POINTER_IS_NULL);

    *node = (TreeNode<T>*)calloc(1, sizeof(TreeNode<T>));
    warning(node, NODE_CALLOC_ERROR);

    (*node)->value  = *value;
    (*node)->left   = NULL;
    (*node)->right  = NULL;
    (*node)->parent = NULL;
    (*node)->error  = NO_TREE_ERRORS;
    (*node)->number_of_children = CHILD_FREE;

    return NO_TREE_ERRORS;
}

// TODO struct HashedString {}

template <typename T>
TreeErrors LinkNodes(TreeNode<T>* parent, TreeNode<T>* child, int connection_side) {
    check_expression(parent, NODE_POINTER_IS_NULL);
    check_expression(child,  NODE_POINTER_IS_NULL);

    child->parent = parent;

    if(connection_side > 0) {
        check_expression(!parent->right, NODE_ALREADY_TAKEN);

        parent->right = child;
    }
    else if(connection_side < 0) {
        check_expression(!parent->left, NODE_ALREADY_TAKEN);

        parent->left  = child;
    }
    else {
        if(!parent->left) {
            parent->left = child;
        }
        else if(!parent->right) {
            parent->right = child;
        }
    }

    parent->number_of_children = 0;
    if(parent->left) {
        parent->number_of_children++;
    }
    if(parent->right) {
        parent->number_of_children++;
    }

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors TreeDtor(Tree<T>* tree) {
    check_expression(tree, TREE_POINTER_IS_NULL);

    DestroySubtree(&(tree->root));

    free(tree->dump_svg_file);
    tree->dump_svg_file = NULL;

    free(tree->dump_html_file);
    tree->dump_html_file = NULL;

    return NO_TREE_ERRORS;
}

template <typename T>
inline TreeErrors DestroySingleNode(TreeNode<T>** node) {
    check_expression(*node, NODE_POINTER_IS_NULL);

    free(*node);
    *node = NULL;

    return NO_TREE_ERRORS;
}

template <typename T>
inline TreeErrors DestroySubtree(TreeNode<T>** node) {
    check_expression(node, NODE_POINTER_IS_NULL);
    if(!(*node)) return NO_TREE_ERRORS;

    if((*node)->left ) DestroySubtree(&((*node)->left));
    if((*node)->right) DestroySubtree(&((*node)->right));

    DestroySingleNode(node);

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors VerifyTree(Tree<T>* tree) {
    check_expression(tree, TREE_POINTER_IS_NULL);

    tree->error = VerifyNodes(tree->root);

    return tree->error;
}

template <typename T>
TreeErrors VerifyNodes(TreeNode<T>* node) {
    check_expression(node, NODE_POINTER_IS_NULL);

    int count_children = 0;

    if(node->left) {
        node->error |= CheckParent(node, node->left);
        check_expression(!node->error, PARENT_LOST_CHILD);

        node->error |= VerifyNodes(node->left);
        check_expression(!node->error, node->error);
        count_children++;
    }
    if(node->right) {
        node->error |= CheckParent(node, node->right);
        check_expression(!node->error, PARENT_LOST_CHILD);

        node->error |= VerifyNodes(node->right);
        check_expression(!node->error, node->error);
        count_children++;
    }

    if(node->number_of_children > count_children) {
        node->error |= NODE_LOST_CHILDREN;
    }
    else if(node->number_of_children < count_children) {
        node->error |= NODE_UNKNOWN_CHILDREN;
    }

    return node->error;
}

template <typename T>
TreeErrors CheckParent(TreeNode<T>* parent, TreeNode<T>* child) {
    check_expression(parent, NODE_POINTER_IS_NULL);
    check_expression(child,  NODE_POINTER_IS_NULL);

    if(child->parent != parent) {
        child->error = PARENT_LOST_CHILD;

        return PARENT_LOST_CHILD;
    }

    return NO_TREE_ERRORS;
}

template<typename T>
inline void PrintTree(const TreeNode<T>* node) {
    printf("(");

    PrintNode<T>(stdout, node);

    if(node->left ) PrintTree(node->left );
    if(node->right) PrintTree(node->right);
    printf(")");
}

template<typename T>
inline void PrintNode(FILE *stream, const TreeNode<T> *node) {
    printf("()");

}

template<>
inline void PrintNode<int>(FILE *stream, const TreeNode<int> *node) {
    printf("%d", node->value);
}

template<>
inline void PrintNode<double>(FILE *stream, const TreeNode<double> *node) {
    printf("%.3lf", node->value);
}

#endif
