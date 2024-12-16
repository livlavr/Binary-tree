#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

#include "custom_asserts.h"

#ifndef TREE_DUMP_H_
#define TREE_DUMP_H_

static const char* HTML_HEADER = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t"
                                  "<meta charset=\"UTF-8\">\n\t<meta name=\"viewport\""
                                  "content=\"width=device-width, initial-scale=1.0\">\n\t"
                                  "<title>Document</title>\n</head>\n<body>\n"
                                  "<style>.line {border-bottom: 10px solid orange;}</style>\n"
                                  "<div style = \"text-align: center;\" class = \"line\">";

template<typename T>
TYPE_OF_ERROR        TreeDump           (Tree<T>*          tree                 );
template<typename T>
TYPE_OF_ERROR        SetDumpFile        (Tree<T>*          tree                 );
inline
TYPE_OF_ERROR        ProcessFilename    (char*             filename             );
template <typename T>
TYPE_OF_ERROR        ProcessTree        (TreeNode<T>*      node, FILE* dot_file );
template <typename T>
inline TYPE_OF_ERROR ProcessNode        (TreeNode<T>*      node, FILE* dot_file );
template <typename T>
inline void          ProcessValue       (TreeNode<T>*  node, FILE* dot_file );
template <typename T>
TYPE_OF_ERROR        OutputToHtml       (Tree<T>*          tree                 );
inline
TYPE_OF_ERROR        PrintHtmlHeader    (FILE*             dump_file            );
template <typename T>
TYPE_OF_ERROR        OpenDump           (Tree<T>*          tree                 );

static const size_t SIZE_OF_BUFFER = 40;

template<typename T>
TYPE_OF_ERROR TreeDump(Tree<T>* tree) {
    check_expression(tree,       POINTER_IS_NULL);
    check_expression(tree->root, POINTER_IS_NULL);

    system("mkdir -p Dump-source");

    FILE* dot_file = fopen("Dump-source/dump.dot", "w");

    warning(dot_file, FILE_OPEN_ERROR);

    //Header of graphviz file
    fprintf(dot_file, "digraph tree{\nsplines=ortho;\nrankdir=HR;\nnodesep=0.4;"
                       "\nnode [shape=record, fontname=\"Arial\"];\n"
                       "edge [style=bold, color=\"black\", weight=10, penwidth=4, "
                       "arrowsize=0.2];\n");

    ProcessTree<T>(tree->root, dot_file);

    fprintf(dot_file, "}\n");

    fclose(dot_file);

    OutputToHtml<T>(tree);

    return SUCCESS;
}

template<typename T>
TYPE_OF_ERROR SetDumpFile(Tree<T>* tree) {
    check_expression(tree, POINTER_IS_NULL);

    char *buffer_svg  = (char*)calloc(SIZE_OF_BUFFER, sizeof(char));
    char *buffer_html = (char*)calloc(SIZE_OF_BUFFER, sizeof(char));

    warning(buffer_svg,  CALLOC_ERROR);
    warning(buffer_html, CALLOC_ERROR);

    time_t my_time          = time(NULL);
    char*  time             = ctime(&my_time);
    time[strlen(time) - 1]  = '\0';
    size_t time_char_length = strlen(time) - 1;
    const char *folder_name = "./Tree-dumps/";

    system("mkdir -p Tree-dumps");

    strcpy (buffer_svg,   folder_name                                );
    strcpy (buffer_html,  folder_name                                );
    strncpy(buffer_svg  + strlen(folder_name), time, time_char_length);
    strncpy(buffer_html + strlen(folder_name), time, time_char_length);
    strcat (buffer_svg,   ".svg"                                     );
    strcat (buffer_html,  ".html"                                    );

    ProcessFilename(buffer_svg );
    ProcessFilename(buffer_html);

    tree->dump_svg_file  = buffer_svg;
    tree->dump_html_file = buffer_html;

    return SUCCESS;
}

inline
TYPE_OF_ERROR ProcessFilename(char* filename) {
    check_expression(filename, POINTER_IS_NULL);

    char* filename_ptr  = filename;
    filename_ptr        = strchr(filename_ptr, ' ');
    while(filename_ptr != NULL)
    {
        *filename_ptr   = '_';
        filename_ptr    = strchr(filename_ptr, ' ');
    }

    return SUCCESS;
}

template <typename T>
TYPE_OF_ERROR ProcessTree(TreeNode<T>* node, FILE* dot_file) {
    check_expression(dot_file, POINTER_IS_NULL);
    if(!node) return SUCCESS;

    ProcessNode<T>(node, dot_file);

    ProcessTree(node->left,  dot_file);
    ProcessTree(node->right, dot_file);

    if(node->left ) fprintf(dot_file, "P%p:<l> -> P%p\n", node, node->left );
    if(node->right) fprintf(dot_file, "P%p:<r> -> P%p\n", node, node->right);

    return SUCCESS;
}

template <typename T>
inline TYPE_OF_ERROR ProcessNode(TreeNode<T>* node, FILE* dot_file) {
    check_expression(dot_file, POINTER_IS_NULL);
    check_expression(node,      POINTER_IS_NULL);

    fprintf(dot_file, "P%p [style = \"filled, rounded\", fillcolor=\"yellow:magenta\" gradientangle=270,"
           "label=\" {Node = [ %p ] | Parent = [ %p ] | Error = %d | Number of kids = %d | ",
            node, node, node->parent, node->error, node->number_of_children);

    ProcessValue<T>(node, dot_file);

    fprintf(dot_file, " | { <l> LEFT = [ %p ] | <r> RIGHT = [ %p ]}}\" ];\n", node->left, node->right);

    return SUCCESS;
}

inline void ProcessValue(TreeNode<int>* node, FILE* dot_file) {
    fprintf(dot_file, "VALUE = %d", node->value);
}

inline void ProcessValue(TreeNode<double>* node, FILE* dot_file) {
    fprintf(dot_file, "VALUE = %3.lf", node->value);
}

inline void ProcessValue(TreeNode<char*>* node, FILE* dot_file) {
    fprintf(dot_file, "VALUE = %s", node->value);
}

template <typename T>
TYPE_OF_ERROR OutputToHtml(Tree<T>* tree) {
    FILE* svg_file  = fopen(tree->dump_svg_file,  "w");
    FILE* html_file = fopen(tree->dump_html_file, "a");

    warning(svg_file,  FILE_OPEN_ERROR);
    warning(html_file, FILE_OPEN_ERROR);

    PrintHtmlHeader(html_file);

    char system_buffer[600] = "";

    snprintf(system_buffer, 600, "echo \'%s\' >> %s; "
             "dot -Tsvg Dump-source/dump.dot >> %s; echo \'</div><br></body>\n\' >> %s",
             HTML_HEADER, tree->dump_html_file, tree->dump_html_file, tree->dump_html_file);

    system(system_buffer);

    fclose(svg_file );
    fclose(html_file);

    return SUCCESS;
}

inline TYPE_OF_ERROR PrintHtmlHeader(FILE* dump_file) {
    check_expression(dump_file, POINTER_IS_NULL);

    fprintf(dump_file, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<meta charset=\"UTF-8\">"
                       "\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                       "\n\t<title>Document</title>\n</head>\n<body>\n");

    return SUCCESS;
}

template <typename T>
TYPE_OF_ERROR OpenDump(Tree<T>* tree) {

    char system_buffer[600] = "";
    snprintf(system_buffer, 300, "open %s", tree->dump_html_file);
    system(system_buffer);

    return SUCCESS;
}

#endif
