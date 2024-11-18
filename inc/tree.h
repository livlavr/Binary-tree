#include <stdlib.h>
#include <string.h>

#include "custom_asserts.h"
#include "tree_definitions.h"
#include "tree_dump.h"

#ifndef TREE_H_
#define TREE_H_

//FUNCTION DECLARATION-----------------------------------------------------------------------

template <typename T>
TreeErrors TreeInit(Tree<T>* tree) {
    check_expression(tree, NODE_POINTER_IS_NULL);

    tree->error      = NO_TREE_ERRORS;
    tree->root       = NULL;
    tree->dump_file  = NULL;
    T root_value     = {};
    CreateNode<T>(&(tree->root), root_value);
    SetDumpFile(tree);

    return NO_TREE_ERRORS;
}

template <typename T>
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

    (*node)->value          = value;
    (*node)->error          = NO_TREE_ERRORS;
    (*node)->number_of_kids = CHILD_FREE;
    (*node)->left           = NULL;
    (*node)->right          = NULL;
    (*node)->parent         = NULL;

    return NO_TREE_ERRORS;
}

template <>
TreeErrors CreateNode<char*>(TreeNode<char*>** node, char* value) {
    check_expression(node, NODE_POINTER_IS_NULL);

    *node = (TreeNode<char*>*)calloc(1, sizeof(TreeNode<char*>));
    warning(node, NODE_CALLOC_ERROR);

    (*node)->value          = value;
    (*node)->error          = NO_TREE_ERRORS;
    (*node)->number_of_kids = CHILD_FREE;
    (*node)->left           = NULL;
    (*node)->right          = NULL;
    (*node)->parent         = NULL;
    (*node)->hash           = Hash((*node)->value);

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors LinkNodes(TreeNode<T>* parent, TreeNode<T>* child, int connection_side) {
    check_expression(parent, NODE_POINTER_IS_NULL);
    check_expression(child , NODE_POINTER_IS_NULL);
    check_expression(CheckRepeats(parent, child) == NO_TREE_ERRORS, REPEATED_NODES_VALUES);

    child->parent = parent;
    //It makes for ability to put in connection_side difference between parent and child values
    //So it's much easier to use binary_tree as sort_tree
    if(connection_side > 0) {
        check_expression(!parent->right, NODE_ALREADY_TAKEN);

        parent->right = child;
    }
    else if(connection_side < 0) {
        check_expression(!parent->left, NODE_ALREADY_TAKEN);

        parent->left  = child;
    }
    else {
        color_printf(RED_TEXT, BOLD, "Wrong choice of connection side. It can't be equal to ZERO");

        check_expression(false, NODES_CONNECTION_ERROR);
    }

    parent->number_of_kids++;

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors TreeDtor(Tree<T>* tree) {
    check_expression(tree, TREE_POINTER_IS_NULL);

    NodesDtor(&(tree->root));

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors NodesDtor(TreeNode<T>** node) {
    check_expression(node, NODE_POINTER_IS_NULL);

    if((*node)->left ) NodesDtor(&((*node)->left));
    if((*node)->right) NodesDtor(&((*node)->right));

    free(*node);
    *node = NULL;

    return NO_TREE_ERRORS;
}

template<>
TreeErrors NodesDtor<char*>(TreeNode<char*>** node) {
    check_expression(node, NODE_POINTER_IS_NULL);

    if((*node)->left ) NodesDtor(&((*node)->left));
    if((*node)->right) NodesDtor(&((*node)->right));

    free((*node)->value);
    (*node)->value = NULL;
    free(*node);
    *node          = NULL;

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors VerifyTree(Tree<T>* tree) {
    check_expression(tree, TREE_POINTER_IS_NULL);

    tree->error = VerifyNodes(tree->root);

    return tree->error;
}

template <typename T> //TODO delete check_expressions and ifs(node->left/right)
TreeErrors VerifyNodes(TreeNode<T>* node) {
    check_expression(node, NODE_POINTER_IS_NULL);

    int count_kids = 0;

    if(node->left) {
        node->error = CheckParent(node, node->left);
        check_expression(!node->error, PARENT_LOST_CHILD);

        node->error = VerifyNodes(node->left);
        check_expression(!node->error, node->error);
        count_kids++;
    }
    if(node->right) {
        node->error = CheckParent(node, node->right);
        check_expression(!node->error, PARENT_LOST_CHILD);

        node->error = VerifyNodes(node->right);
        check_expression(!node->error, node->error);
        count_kids++;
    }

    if(node->number_of_kids > count_kids) {
        node->error = NODE_LOST_KIDS;
    }
    else if(node->number_of_kids < count_kids) {
        node->error = NODE_FOREIGN_KIDS;
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

template <typename T>
TreeErrors CheckRepeats(TreeNode<T>* parent, TreeNode<T>* child) {
    check_expression(parent, NODE_POINTER_IS_NULL);
    check_expression(child,  NODE_POINTER_IS_NULL);

    TreeNode<T>* root = FindRoot(parent);

    child->error      = FindRepeats(root, child);

    return child->error;
}

template <>
TreeErrors CheckRepeats<char*>(TreeNode<char*>* parent, TreeNode<char*>* child) {
    check_expression(parent, NODE_POINTER_IS_NULL);
    check_expression(child,  NODE_POINTER_IS_NULL);

    TreeNode<char*>* root = FindRoot(parent);

    child->error          = FindRepeats(root, child);

    return child->error;
}

template <typename T>
TreeNode<T>* FindRoot(TreeNode<T>* node) {
    while(node->parent) {
        node = node->parent;
    }
    return node;
}

template <typename T>
TreeErrors FindRepeats(TreeNode<T>* parent, TreeNode<T>* child) {
    if(!parent) return NO_TREE_ERRORS;

    if(parent->value == child->value) {
        return REPEATED_NODES_VALUES;
    }

    parent->error     = FindRepeats(parent->left, child);
    if(!parent->error) {
        parent->error = FindRepeats(parent->right, child);
    }

    return parent->error;
}

template <>
TreeErrors FindRepeats<char*>(TreeNode<char*>* parent, TreeNode<char*>* child) {
    if(!parent) return NO_TREE_ERRORS;

    if(parent->value == child->value) {
        return REPEATED_NODES_VALUES;
    }
    if(parent->hash == child->hash) {
        if(strcmp(parent->value, child->value) == 0) {
            color_printf(RED_TEXT, BOLD, "Value : \"%s\" already in BinaryTree.\n", child->value);
            return REPEATED_NODES_VALUES;
        }
    }

    parent->error = FindRepeats(parent->left, child);
    if(!parent->error) {
        parent->error = FindRepeats(parent->right, child);
    }

    return parent->error;
}



//TODO Doxy style
//This function takes node as argument to add ability
//   of print not only the full tree but also subtree
template<typename T>
void PrintTree(TreeNode<T>* node) {
    printf("()");
}

template<>
void PrintTree<int>(TreeNode<int>* node) {
    check_expression(node, (void)NODE_POINTER_IS_NULL);

    printf("(");
    printf("%d", node->value);
    if(node->left ) PrintTree(node->left );
    if(node->right) PrintTree(node->right);
    printf(")");
}

template<>
void PrintTree<double>(TreeNode<double>* node) {
    check_expression(node, (void)NODE_POINTER_IS_NULL);

    printf("(");
    printf("%.3lf", node->value);
    if(node->left ) PrintTree(node->left );
    if(node->right) PrintTree(node->right);
    printf(")");
}

template<>
void PrintTree<char *>(TreeNode<char*>* node) {
    check_expression(node, (void)NODE_POINTER_IS_NULL);

    printf("(");
    printf("%s", node->value);
    if(node->left ) PrintTree(node->left );
    if(node->right) PrintTree(node->right);
    printf(")");
}

uint32_t Hash(char* str) {
    if(!str) return 0;

    uint32_t hash = 5381;
    int c = 0;

    while(c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

//-------------------------------------------------------------------------------------------
//DEFINES------------------------------------------------------------------------------------

//PREORDER_PRINT
#define PreorderPrintTree_(node_ptr) \
    PrintTree(node_ptr);             \
    printf("\n")

//NO_REPEATS_MACROS
#ifdef NO_REPEATS
    #define NoRepeats_(argument) argument
#else
    #define NoRepeats_(argument) ;
#endif

//-------------------------------------------------------------------------------------------

#endif

