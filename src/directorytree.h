#include <malloc.h>
#include <stdio.h>
#include "types.h"

typedef struct DirNode {
    char *name;
    uint_64 size;
    uint_64 numFiles;
    uint_64 numSubDirs;
    struct DirNode *parent;
    struct DirNode **subdirectories;
    struct FileNode **files;
} DirNode;

typedef struct FileNode {
    char *name;
    uint_64 size;
    DirNode *parent;
} FileNode;

/**
 * Function Prototypes
 * @func createDirNode: Creates a dir node with a specific parent and a size.
 * @func createFileNode: Creates a file node with a specific parent and a size.
 * @func addSubDir: Adds a sub directory to a dir node.
 * @func addFile: Adds a file to a dir node.
 * @func freeDirNode: Frees a dir node and all of its subdirectories and files.
 * @func freeFileNode: Frees a file node.
 * @func printDirTree: Prints the directory tree.
 */
DirNode *createDirNode(char *name, DirNode *parent, uint_64 size);
FileNode *createFileNode(char *name, DirNode *parent, uint_64 size);
void addSubDir(DirNode *parent, DirNode *subDir);
void addFile(DirNode *parent, FileNode *file);
void freeDirNode(DirNode *node);
void freeFileNode(FileNode *node);
void printDirTree(DirNode *node, int depth);
void printFileNode(FileNode *node, int depth);


/**
 * @brief Creates a directory node, with no children.
 * @param name Directory Name
 * @param parent Directory Parent Node. This may be NULL for root directory
 * @param size Directory Size [OPTIONAL]
 * @return
 */
DirNode *createDirNode(char *name, DirNode *parent, uint_64 size) {
    DirNode *node = (DirNode *)malloc(sizeof(DirNode));
    node->name = name;
    node->size = size ? size : 0;
    node->numFiles = 0;
    node->numSubDirs = 0;
    node->parent = parent;
    node->subdirectories = (DirNode **) malloc(sizeof(DirNode *));
    node->files = (FileNode **) malloc(sizeof(FileNode *));
    return node;
}

/**
 * @brief Creates a file node, with no children.
 * @param name File Name
 * @param parent File's Parent Node.
 * @param size File Size [REQUIRED]
 * @return
 */
FileNode *createFileNode(char *name, DirNode *parent, uint_64 size) {
    FileNode *node = (FileNode *)malloc(sizeof(FileNode));
    node->name = name;
    node->size = size;
    node->parent = parent;
    return node;
}

/**
 * @brief Adds a sub directory to a directory node.
 * @param parent Parent Directory Node
 * @param subDir Sub Directory Node
 * @return
 */
void addSubDir(DirNode *parent, DirNode *subDir) {
    DirNode** temp = (DirNode **)realloc(parent->subdirectories, sizeof(DirNode *) * (parent->numSubDirs + 1));
    if (temp == NULL) {
        free(parent->subdirectories);
        return;
    }
    parent->subdirectories = temp;
    parent->subdirectories[parent->numSubDirs++] = subDir;
}

/**
 * @brief Adds a file to a directory node.
 * @param parent Parent Directory Node
 * @param file File Node
 */
void addFile(DirNode *parent, FileNode *file) {
    FileNode** temp = (FileNode **)realloc(parent->files, sizeof(FileNode *) * (parent->numFiles + 1));
    if (temp == NULL) {
        free(parent->files);
        return;
    }
    parent->files = temp;
    parent->files[parent->numFiles++] = file;
}

/**
 * @brief Frees a directory node and all of its subdirectories and files.
 * @param node Directory Node to be freed.
 */
// NOLINTNEXTLINE - Disable Recursion Check
void freeDirNode(DirNode *node) {
    for (uint_64 i = 0; i < node->numSubDirs; i++) freeDirNode(node->subdirectories[i]);
    for (uint_64 i = 0; i < node->numFiles; i++) freeFileNode(node->files[i]);
    free(node->subdirectories);
    free(node->files);
    free(node);
}

/**
 * @brief Frees a file node.
 * @param node File Node to be freed.
 */
void freeFileNode(FileNode *node) {
    free(node);
}

/**
 * @brief Prints the directory tree.
 * @param node Directory Node
 * @param depth Depth of the directory node
 */
// NOLINTNEXTLINE - Disable Recursion Check
void printDirTree(DirNode *node, int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", node->name);
    for (uint_64 i = 0; i < node->numFiles; i++) printFileNode(node->files[i], depth + 1);
    for (uint_64 i = 0; i < node->numSubDirs; i++) printDirTree(node->subdirectories[i], depth + 1);
}

/**
 * @brief Prints a file node.
 * @param node File Node
 * @param depth Depth of the file node
 */
void printFileNode(FileNode *node, int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", node->name);
}
