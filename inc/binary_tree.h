#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

static const int LEFT_NODE  = -1;
static const int RIGHT_NODE =  1;
static const int CHILD_FREE =  0;

enum TreeErrors {
    NO_TREE_ERRORS          = 0,
    NODE_POINTER_IS_NULL    = 1 << 0,
    TREE_POINTER_IS_NULL    = 1 << 1,
    NODES_CONNECTION_ERROR  = 1 << 2,
    NODE_CALLOC_ERROR       = 1 << 3,
    NODE_ALREADY_TAKEN      = 1 << 4,
    NODE_BAD_POINTER        = 1 << 5,
    NODE_LOST_KIDS          = 1 << 6,
    NODE_FOREIGN_KIDS       = 1 << 7,
    PARENT_LOST_CHILD       = 1 << 8,
    REPEATED_NODES_VALUES   = 1 << 9,
    CHILD_ON_DIFFERENT_TREE = 1 << 10,
    NULL_ARRAY_OF_VALUES    = 1 << 11
};
template <typename T>
struct TreeNode;

template <typename T>
struct Tree {
    TreeNode<T>* root_node;
    int          number_of_kids;//wrapper no_repeat
    TreeErrors   error;//TODO wrapper ndebug
};

template <typename T>
struct TreeNode {
    Tree<T>*     tree;//TODO wrapper no_repeat
    T            value;
    TreeNode<T>* left_node;
    TreeNode<T>* right_node;
    TreeNode<T>* parent_node;
    int          number_of_kids;//Ndebug
    TreeErrors   error;//TODO wrapper ndebug
};

template <typename T>
TreeErrors TreeInit       (Tree<T>*          tree     );
template <typename T>
TreeErrors VerifyTree     (Tree<T>*          tree     );
template <typename T>
TreeErrors VerifyNodes    (TreeNode<T>*      node     );
template <typename T>
TreeErrors ParentCheck    (TreeNode<T>*      node     );
template <typename T>
TreeErrors RepeatsCheck   (TreeNode<T>* original_node, TreeNode<T>* comparing_node);
template <typename T>
TreeErrors FillValuesArray(TreeNode<T>* node, T* array_of_values, int* count_checking_nodes);
template <typename T>
TreeErrors FindRepeats    (T* array_of_values, int count_checking_nodes);

template <typename T>
TreeErrors TreeDtor       (Tree<T>*          tree     );
template <typename T>
TreeErrors NodesDtor      (TreeNode<T>**     node     );
TreeErrors NodesDtor      (TreeNode<char*>** node     );

template <typename T>
TreeErrors AddNode        (TreeNode<T>* node, T value, int connection_side);
template <typename T>
TreeErrors CreateNode     (TreeNode<T>* node, T value );
template <typename T>
TreeErrors LinkNodes      (TreeNode<T>* parent_node, TreeNode<T>* child_node, int connection_side);
template <typename T>
TreeErrors FindRoot       (TreeNode<T>*      node     );

void PrintTree            (TreeNode<int>*    node     );
void PrintTree            (TreeNode<double>* node     );
void PrintTree            (TreeNode<char*>*  node     );

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

#endif
