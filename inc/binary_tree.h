#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

static const int LEFT_NODE  = -1;
static const int RIGHT_NODE =  1;

enum TreeErrors {
    NO_TREE_ERRORS         = 0,
    NODE_POINTER_IS_NULL   = 1 << 0,
    TREE_POINTER_IS_NULL   = 1 << 1,
    NODES_CONNECTION_ERROR = 1 << 2,
    NODE_CALLOC_ERROR      = 1 << 3
};

template <typename T>
struct TreeNode {
    T            value;
    TreeNode<T>* left_node;
    TreeNode<T>* right_node;
    TreeNode<T>* parent_node;
    TreeErrors   error;
};

template <typename T>
struct Tree {
    int          number_of_nodes;
    TreeNode<T>* root_node;
    TreeErrors   error;
};

template <typename T>
TreeErrors TreeInit     (Tree<T>*      tree    );
template <typename T>
TreeErrors VerifyTree   (Tree<T>*      tree    );
template <typename T>
TreeErrors TreeDestruct (Tree<T>*      tree    );
template <typename T>
TreeErrors NodesDestruct(TreeNode<T>** node    );
TreeErrors NodesDestruct(TreeNode<char*>** node);

template <typename T>
TreeErrors AddNode      (TreeNode<T>* node, T value, int connection_side);
template <typename T>
TreeErrors CreateNode   (TreeNode<T>* node, T value);
template <typename T>
TreeErrors LinkNodes    (TreeNode<T>* parent_node, TreeNode<T>* child_node, int connection_side);

void PrintTree          (TreeNode<int>*    node);
void PrintTree          (TreeNode<double>* node);
void PrintTree          (TreeNode<char*>*  node);

#endif
