#define NO_REPEATS

#include <stdio.h>
#include <stdlib.h>

#include "custom_asserts.h"
#include "binary_tree.h"

int main() {
    Tree<int>    int_tree    = {};
    Tree<double> double_tree = {};
    Tree<char*>  char_tree   = {};
    char* loh = (char*)calloc(5, 1);
    char* ded = (char*)calloc(5, 1);

    TreeInit(&int_tree);
    int_tree.root_node->value = 50;
    PrintTree(int_tree.root_node);
    printf("\n");
    AddNode(int_tree.root_node, 80, LEFT_NODE);//Pizdec
    AddNode(int_tree.root_node, 70, RIGHT_NODE);
    AddNode(int_tree.root_node->right_node, 80, RIGHT_NODE);
    AddNode(int_tree.root_node->right_node, 60, LEFT_NODE);
    AddNode(int_tree.root_node->right_node->left_node, 55, LEFT_NODE);
    AddNode(int_tree.root_node->right_node->right_node, 75, LEFT_NODE);
    PreorderPrintTree_(int_tree.root_node);
    VerifyNodes(int_tree.root_node);
    TreeDtor(&int_tree);

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
    PreorderPrintTree_(double_tree.root_node);
    TreeDtor(&double_tree);

    scanf("%s", loh);
    scanf("%s", ded);
    TreeInit(&char_tree);
    char_tree.root_node->value = loh;
    PrintTree(char_tree.root_node);
    printf("\n");
    AddNode(char_tree.root_node, ded, LEFT_NODE);
    PreorderPrintTree_(char_tree.root_node);
    printf("\n");
    TreeDtor(&char_tree);
    //TODO same pointer in two nodes
}

template <typename T>
TreeErrors TreeInit(Tree<T>* tree) {
    check_expression(tree, NODE_POINTER_IS_NULL);

    tree->error           = NO_TREE_ERRORS;
    tree->root_node       = NULL;
    tree->number_of_kids  = 0;
    T root_node_value     = {};
    CreateNode<T>(&(tree->root_node), root_node_value);
    tree->root_node->tree = tree;

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
    (*node)->left_node      = NULL;
    (*node)->right_node     = NULL;
    (*node)->parent_node    = NULL;
    (*node)->tree           = NULL;

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors LinkNodes(TreeNode<T>* parent_node, TreeNode<T>* child_node, int connection_side) {
    check_expression(parent_node, NODE_POINTER_IS_NULL);
    check_expression(child_node , NODE_POINTER_IS_NULL);

    child_node->parent_node = parent_node;
    child_node->tree        = parent_node->tree;
    //It makes for ability to put in connection_side difference between parent and child values
    //So it's much easier to use binary_tree as sort_tree
    if(connection_side > 0) {
        check_expression(!parent_node->right_node, NODE_ALREADY_TAKEN);
        parent_node->right_node = child_node;
    }
    else if(connection_side < 0) {
        check_expression(!parent_node->left_node, NODE_ALREADY_TAKEN);
        parent_node->left_node  = child_node;
    }
    else {
        color_printf(RED_TEXT, BOLD, "Wrong choice of connection side. It can't be equal to ZERO");

        check_expression(false, NODES_CONNECTION_ERROR);
    }

    parent_node->number_of_kids++;

    return NO_TREE_ERRORS;
}
//TODO
// template <typename T>
// TreeNode<T>* FindRoot(TreeNode<T>* node) {
//     while(node->parent_node) {
//         node = node->parent_node;
//     }
//     return node;
// }

template <typename T>
TreeErrors TreeDtor(Tree<T>* tree) {
    check_expression(tree, TREE_POINTER_IS_NULL);

    NodesDtor(&(tree->root_node));

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors NodesDtor(TreeNode<T>** node) {
    check_expression(node, NODE_POINTER_IS_NULL);

    if((*node)->left_node ) NodesDtor(&((*node)->left_node));
    if((*node)->right_node) NodesDtor(&((*node)->right_node));

    free(*node);
    *node = NULL;

    return NO_TREE_ERRORS;
}

TreeErrors NodesDtor(TreeNode<char*>** node) {
    check_expression(node, NODE_POINTER_IS_NULL);

    ((*node)->tree)

    if((*node)->left_node ) NodesDtor(&((*node)->left_node));
    if((*node)->right_node) NodesDtor(&((*node)->right_node));

    free((*node)->value);
    (*node)->value = NULL;
    free(*node);
    *node          = NULL;

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors VerifyTree(Tree<T>* tree) {
    check_expression(tree,      TREE_POINTER_IS_NULL);

    tree->root_node->tree->number_of_kids
    tree->error = VerifyNodes(tree->root_node);

    return tree->error;
}

template <typename T> //TODO delete check_expressions and ifs(node->left/right_node)
TreeErrors VerifyNodes(TreeNode<T>* node) {
    check_expression(node, NODE_POINTER_IS_NULL);

    int count_kids = 0;

    if(node->left_node) {
        node->error = ParentCheck(node, node->left_node);
        check_expression(!node->error, PARENT_LOST_CHILD);

        node->error = VerifyNodes(node->left_node);
        check_expression(!node->error, node->error);
        count_kids++;
    }
    if(node->right_node) {
        node->error = ParentCheck(node, node->right_node);
        check_expression(!node->error, PARENT_LOST_CHILD);

        node->error = VerifyNodes(node->right_node);
        check_expression(!node->error, node->error);
        count_kids++;
    }

    if(node->number_of_kids > count_kids) {
        node->error = NODE_LOST_KIDS;
    }
    else if(node->number_of_kids < count_kids) {
        node->error = NODE_FOREIGN_KIDS;
    }

    node->error = RepeatsCheck(node);

    // printf("[ %p ] - NODE\t", node);
    // printf("< %d > - ERROR\n", node->error);

    return node->error;
}

template <typename T>
TreeErrors ParentCheck(TreeNode<T>* parent, TreeNode<T>* child) {
    check_expression(parent, NODE_POINTER_IS_NULL);
    check_expression(child,  NODE_POINTER_IS_NULL);

    if(child->parent_node != parent) {
        child->error = PARENT_LOST_CHILD;

        return PARENT_LOST_CHILD;
    }
    if(child->tree != parent->tree) {
        child->error = CHILD_ON_DIFFERENT_TREE;

        return CHILD_ON_DIFFERENT_TREE;
    }

    return NO_TREE_ERRORS;
}

template <typename T>
TreeErrors RepeatsCheck(TreeNode<T>* node) {
    if(node) return NO_TREE_ERRORS;

    T* array_of_values = NULL;



    FillValuesArray(node, array_of_values);

    FindElement();

    free(array_of_values);
}

template <typename T>
TreeErrors FillValuesArray(TreeNode<T>* node, T* array_of_values) {

}

template <typename T>
TreeErrors FindElement(T new_value, T* exist_values) {
    for(int i = 0; i < )
}

//TODO paste verifier everywhere
//RepeatsCheck is very expensive
// template <typename T> //TODO this is the end of verification //TODO *
// TreeErrors RepeatsCheck(TreeNode<T>* original_node, TreeNode<T>* comparing_node, T value) {
//     if(!comparing_node || !original_node) return NO_TREE_ERRORS;
//     //TODO if I change the check_expression call (remove it or replace NODE_POINTER_IS_NULL
//     //     on NO_ERRORS I could call recursive without if's)
//
//     if(comparing_node->value == value) {
//         original_node ->error = REPEATED_NODES_VALUES;
//         comparing_node->error = REPEATED_NODES_VALUES;
//         return REPEATED_NODES_VALUES;
//     }
//
//     RepeatsCheck(original_node, comparing_node->left_node, value);
//     if(original_node->error == NO_TREE_ERRORS) {
//         RepeatsCheck(original_node, comparing_node->right_node, value);
//     }
//
//     if(original_node->error) return original_node->error;
//
//     RepeatsCheck(original_node->left_node, original_node->left_node->left_node, original_node->left_node->value);
//     RepeatsCheck(original_node->left_node, original_node->left_node->right_node, original_node->left_node->value);
//     if(original_node->error == NO_TREE_ERRORS) {
//         RepeatsCheck(original_node->right_node, original_node->right_node->left_node, original_node->right_node->value);
//         RepeatsCheck(original_node->right_node, original_node->right_node->right_node, original_node->right_node->value);
//     }
//
//     return original_node->error;
// }
//TODO Doxy style
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
