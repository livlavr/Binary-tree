#ifndef TREE_DEFINITIONS_H_
#define TREE_DEFINITIONS_H_

static const int LEFT_SIDE  = -1;
static const int RIGHT_SIDE =  1;
static const int CHILD_FREE =  0;

enum TreeErrors {
    NO_TREE_ERRORS          = 0,
    NODE_POINTER_IS_NULL    = 1 << 0,
    TREE_POINTER_IS_NULL    = 1 << 1,
    NODES_CONNECTION_ERROR  = 1 << 2,
    NODE_CALLOC_ERROR       = 1 << 3,
    NODE_ALREADY_TAKEN      = 1 << 4,
    NODE_BAD_POINTER        = 1 << 5,
    NODE_LOST_CHILDREN      = 1 << 6,
    NODE_UNKNOWN_CHILDREN   = 1 << 7,
    PARENT_LOST_CHILD       = 1 << 8,
    REPEATED_NODES_VALUES   = 1 << 9,
    CHILD_ON_DIFFERENT_TREE = 1 << 10,
    NULL_ARRAY_OF_VALUES    = 1 << 11,
    VALUE_POINTER_IS_NULL   = 1 << 12
};

template <typename T>
struct TreeNode {
    T            value;
    TreeNode<T>* left;
    TreeNode<T>* right;
    TreeNode<T>* parent;
    int          number_of_children;//Ndebug
    TreeErrors   error;//TODO wrapper ndebug
};

template <typename T>
struct Tree {
    TreeNode<T>* root;
    TreeErrors   error;//TODO wrapper ndebug
    char*        dump_svg_file;
    char*        dump_html_file;
};

//FUNCTION DEFINITION------------------------------------------------------------------------
//TODO codestyle
template <typename T>
TreeErrors TreeInit                 (Tree<T>* tree, T root_value);
template <typename T>
TreeErrors VerifyTree               (Tree<T>*            tree   );
template <typename T>
TreeErrors VerifyNodes              (TreeNode<T>*        node   );
template <typename T>
TreeErrors CheckParent              (TreeNode<T>*        node   );
template <typename T>
TreeErrors CheckRepeats             (TreeNode<T>*        parent,
                                     TreeNode<T>*        child  );
template <>
TreeErrors CheckRepeats<char*>      (TreeNode<char*>*  parent,
                                     TreeNode<char*>*  child  );
template <typename T>
TreeNode<T>* FindRoot               (TreeNode<T>*      node   );
template <typename T>
inline TreeErrors FindRepeats       (TreeNode<T>*      parent,
                                     TreeNode<T>*      child  );
template <>
inline TreeErrors FindRepeats<char*>(TreeNode<char*>*  parent,
                                     TreeNode<char*>*  child  );

template <typename T>
TreeErrors        TreeDtor          (Tree<T>*            tree   );
template <typename T>
inline TreeErrors NodesDtor         (TreeNode<T>**       node   );

template <typename T>
TreeErrors AddNode                  (TreeNode<T>*        node, T value, int connection_side);
template <typename T>
inline TreeErrors CreateNode        (TreeNode<T>**       node, T     value                 );
template <typename T>
TreeErrors LinkNodes                (TreeNode<T>* parent_node, TreeNode<T>* child_node,
                                     int connection_side                                   );

template <typename T>
inline void PrintTree               (TreeNode<T>*        node   );
template <>
inline void PrintTree<int>          (TreeNode<int>*      node   );
template <>
inline void PrintTree<double>       (TreeNode<double>*   node   );
template <>
inline void PrintTree<char*>        (TreeNode<char*>*    node   );

inline uint32_t Hash                (char* str);
//-------------------------------------------------------------------------------------------

#endif
