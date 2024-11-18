#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "custom_asserts.h"

#ifndef TREE_DUMP_H_
#define TREE_DUMP_H_

template<typename T>
TYPE_OF_ERROR TreeDump(Tree<T>* tree);
template<typename T>
TYPE_OF_ERROR SetDumpFile(Tree<T>* tree);
inline TYPE_OF_ERROR ProcessFilename(char* filename);
template <typename T>
TYPE_OF_ERROR DumpPrintTree(TreeNode<T>* node, FILE* dump_file);
template <typename T>
TYPE_OF_ERROR DumpPrintNode(TreeNode<T>* node, FILE* dump_file);
template <>
TYPE_OF_ERROR DumpPrintNode<int>(TreeNode<int>* node, FILE* dump_file);
template <>
TYPE_OF_ERROR DumpPrintNode<double>(TreeNode<double>* node, FILE* dump_file);
template <>
TYPE_OF_ERROR DumpPrintNode<char*>(TreeNode<char*>* node, FILE* dump_file);

static const size_t SIZE_OF_BUFFER = 40;

template<typename T>
TYPE_OF_ERROR TreeDump(Tree<T>* tree) {
    check_expression(tree, POINTER_IS_NULL);
    check_expression(tree->root, POINTER_IS_NULL);

    system("mkdir -p Dump-source");

    FILE* dump_file = fopen("Dump-source/dump.dot", "w");

    warning(dump_file, FILE_OPEN_ERROR);

    //Header of graphviz file
    fprintf(dump_file, "digraph tree{\nsplines=ortho;\nrankdir=HR;\nnodesep=0.4;"
                       "\nnode [shape=record, fontname=\"Arial\"];\n"
                       "edge [style=bold, color=\"#5f5dbd:black;0.001\", weight=10, penwidth=4.5, "
                       "arrowsize=0.3];\n");

    DumpPrintTree(tree->root, dump_file);

    fprintf(dump_file, "}\n");

    fclose(dump_file);

    char system_duffer[100] = "";
    strcat(system_duffer, "dot -Tpng Dump-source/dump.dot -o ");
    strcat(system_duffer, tree->dump_file);

    system(system_duffer);

    strcpy(system_duffer, "");
    strcat(system_duffer, "open ");
    strcat(system_duffer, tree->dump_file);

    system(system_duffer);

    return SUCCESS;
}

template<typename T>
TYPE_OF_ERROR SetDumpFile(Tree<T>* tree)
{
    check_expression(tree, POINTER_IS_NULL);

    char *buffer = (char*)calloc(SIZE_OF_BUFFER, sizeof(char));

    warning(buffer, CALLOC_ERROR);

    time_t my_time          = time(NULL);
    char*  time             = ctime(&my_time);
    time[strlen(time) - 1]  = '\0';
    size_t time_char_length = strlen(time) - 1;
    const char *folder_name = "Tree-dumps/";

    system("mkdir -p Tree-dumps");

    strcpy(buffer, folder_name);
    strncpy(buffer + strlen(folder_name), time, time_char_length);
    strcat(buffer, ".png");

    ProcessFilename(buffer);

    tree->dump_file = buffer;

    return SUCCESS;
}

inline
TYPE_OF_ERROR ProcessFilename(char* filename) {
    check_expression(filename, POINTER_IS_NULL);

    char* filename_ptr = filename;
    filename_ptr = strchr(filename_ptr, ' ');
    while(filename_ptr != NULL)
    {
        *filename_ptr = '_';
        filename_ptr  = strchr(filename_ptr, ' ');
    }

    return SUCCESS;
}

template <typename T>
TYPE_OF_ERROR DumpPrintTree(TreeNode<T>* node, FILE* dump_file) {
    check_expression(dump_file, POINTER_IS_NULL);
    if(!node) return SUCCESS;

    DumpPrintNode<T>(node, dump_file);

    DumpPrintTree(node->left,  dump_file);
    DumpPrintTree(node->right, dump_file);

    if(node->left ) fprintf(dump_file, "P%p -> P%p\n", node, node->left);
    if(node->right) fprintf(dump_file, "P%p -> P%p\n", node, node->right);

    return SUCCESS;
}

template <typename T>
TYPE_OF_ERROR DumpPrintNode(TreeNode<T>* node, FILE* dump_file) {
    check_expression(dump_file, POINTER_IS_NULL);
    check_expression(node, POINTER_IS_NULL);

    fprintf(dump_file, "P%p [style = \"filled, rounded\", fillcolor=\"#a4fbe3\","
          "label=\" {Value = Undefined Type | Number of kids = %d | Left = %p | Right = %p | Node = %p | Parent = %p | Error = %d}\" ];\n",
           node, node->number_of_kids, node->left, node->right, node, node->parent, node->error);

    return SUCCESS;
}

template <>
TYPE_OF_ERROR DumpPrintNode<int>(TreeNode<int>* node, FILE* dump_file) {
    check_expression(dump_file, POINTER_IS_NULL);
    check_expression(node, POINTER_IS_NULL);

    fprintf(dump_file, "P%p [style = \"filled, rounded\", fillcolor=\"#a4fbe3\","
          "label=\" { Value = %d | Number of kids = %d | Node = %p | Parent = %p | Error = %d | {Left = %p | Right = %p}}\" ];\n",
           node, node->value, node->number_of_kids, node, node->parent, node->error, node->left, node->right);

    return SUCCESS;
}

template <>
TYPE_OF_ERROR DumpPrintNode<double>(TreeNode<double>* node, FILE* dump_file) {
    check_expression(dump_file, POINTER_IS_NULL);
    check_expression(node, POINTER_IS_NULL);

    fprintf(dump_file, "P%p [style = \"filled, rounded\", fillcolor=\"#a4fbe3\","
          "label=\" {Value = %.3lf | Number of kids = %d | Left = %p | Right = %p | Node = %p | Parent = %p | Error = %d}\" ];\n",
           node, node->value, node->number_of_kids, node->left, node->right, node, node->parent, node->error);

    return SUCCESS;
}

template <>
TYPE_OF_ERROR DumpPrintNode<char*>(TreeNode<char*>* node, FILE* dump_file) {
    check_expression(dump_file, POINTER_IS_NULL);
    check_expression(node, POINTER_IS_NULL);

    fprintf(dump_file, "P%p [style = \"filled, rounded\", fillcolor=\"#a4fbe3\","
          "label=\" {Value = %s | Hash = %lu | Number of kids = %d | Left = %p | Right = %p | Node = %p | Parent = %p | Error = %d}\" ];\n",
           node, node->value, node->number_of_kids, node->left, node->right, node, node->parent, node->error);

    return SUCCESS;
}

#endif
