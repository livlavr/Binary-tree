#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

template <typename T>
struct TreeNode {
    T         value;
    TreeNode* left_node;
    TreeNode* right_node;
    TreeNode* parent_node;
}

struct BinaryTree {
    int       number_of_nodes;
    TreeNode* root_node;
}



#endif
