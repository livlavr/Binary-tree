#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

static const int LEFT_NODE  = -1;
static const int RIGHT_NODE =  1;

enum TreeErrors {
    NO_TREE_ERRORS         = 0,
    NODE_POINTER_IS_NULL   = 1 << 0,
    NODES_CONNECTION_ERROR = 1 << 1,
    NODE_CALLOC_ERROR      = 1 << 2
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
struct BinaryTree {
    int          number_of_nodes;
    TreeNode<T>* root_node;
    TreeErrors   error;
};

template <typename T>
TreeErrors TreeInit    (BinaryTree<T>* tree         );
template <typename T>
TreeErrors VerifyTree  (BinaryTree<T>* tree         );
template <typename T>
TreeErrors CreateNode  (TreeNode<T>*   node, T value);
template <typename T>
TreeErrors ConnectNodes(TreeNode<T>* parent_node, TreeNode<T>* child_node,
                        int connection_side         );
void PrintTree   (TreeNode<int>*    node    );
void PrintTree   (BinaryTree<double>* node    );
void PrintTree   (BinaryTree<char*>*  node    );
#endif
