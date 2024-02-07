#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "numformat.h"
#include "directorytree.h"

/**
 * Recursively list all files and folders in a directory
 * @param sDir char* - The directory to list
 * @return 0 on success, 1 on failure
 */
// NOLINTNEXTLINE - Disable Recursion warning for this function
bool ListDirectoryContents(const char *sDir) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    char sPath[2048];

    // INFO: Specify file mask. *.* = every file. You can specify a specific file name or use *.*
    sprintf(sPath, "%s\\*.*", sDir);

    // INFO: Return early if invalid path
    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path not found: [%s]\n", sDir);
        return false;
    }

    do {
        // INFO: Skip . and .. folders
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            // INFO: Get full file path using the passed in [sDir] and the file/foldername we just found
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            // INFO: Recurse only if entity is a Folder
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
                printf("Directory: %s\n", sPath);
                ListDirectoryContents(sPath);
            }
            else printf("File: %s\n", sPath);
        }
    } while (FindNextFile(hFind, &fdFile)); // Find the next file.

    FindClose(hFind); // CLEANUP: Close the Find handle

    return true;
}

/**
 * Recursively get the size of a directory
 * @param sDir char* - The directory to list
 * @param numFiles uint_64* - The number of files in the directory
 * @param numDirs uint_64* - The number of directories in the directory
 * @return Size of the directory in bytes
 */
// NOLINTNEXTLINE - Disable Recursion warning for this function
uint_64 ListDirectorySize(const char *sDir, uint_64 *numFiles, uint_64 *numDirs) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    char sPath[2048];

    uint_64 currSize = 0, fileSize;

    // INFO: Specify file mask. *.* = every file. You can specify a specific file name or use *.*
    sprintf(sPath, "%s\\*.*", sDir);

    // INFO: Return early if invalid path
    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
//        printf("Path not found: [%s]\n", sDir);
        return currSize;
    }

    do {
        // INFO: Skip . and .. folders
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            // INFO: Get full file path using the passed in [sDir] and the file/foldername we just found
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            // INFO: Recurse only if entity is a Folder
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
                (*numDirs)++;
                currSize += ListDirectorySize(sPath, numFiles, numDirs);
                if (currSize < 1024) printf("Directory: %s\t%lldB\n", sPath, currSize);
                else if (currSize < 1024 * 1024) printf("Directory: %s\t%lldK\n", sPath, currSize / 1024);
                else if (currSize < 1024 * 1024 * 1024) printf("Directory: %s\t%lldM\n", sPath, currSize / (1024 * 1024));
                else printf("Directory: %s\t%lldG\n", sPath, currSize / (1024 * 1024 * 1024));
            } else {
                fileSize = (uint_64) fdFile.nFileSizeHigh << 32 | fdFile.nFileSizeLow;
                currSize += fileSize;
                if (++*numFiles % 100000 == 0) printf("Checked: %lld Files\r", *numFiles);
//                printf("File: %s\t%lld\n", sPath, fileSize);
            }
        }
    } while (FindNextFile(hFind, &fdFile)); // Find the next file.

    FindClose(hFind); // CLEANUP: Close the Find handle

    return currSize;
}

DirNode *GetDirectoryTree(const char *sDir, DirNode *parent) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    char sPath[2048];

    uint_64 currSize = 0, fileSize;

    // INFO: Specify file mask. *.* = every file. You can specify a specific file name or use *.*
    sprintf(sPath, "%s\\*.*", sDir);

    // INFO: Return early if invalid path
    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
        return NULL;

    DirNode *root = createDirNode((char *) sDir, parent, 0);
    do {
        // INFO: Skip . and .. folders
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            // INFO: Get full file path using the passed in [sDir] and the file/foldername we just found
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            // INFO: Recurse only if entity is a Folder
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
                DirNode *child = GetDirectoryTree(sPath, root);
//                addSubDir(root, child)
                currSize += ListDirectorySize(sPath, numFiles, numDirs);
//                if (currSize < 1024) printf("Directory: %s\t%lldB\n", sPath, currSize);
//                else if (currSize < 1024 * 1024) printf("Directory: %s\t%lldK\n", sPath, currSize / 1024);
//                else if (currSize < 1024 * 1024 * 1024) printf("Directory: %s\t%lldM\n", sPath, currSize / (1024 * 1024));
//                else printf("Directory: %s\t%lldG\n", sPath, currSize / (1024 * 1024 * 1024));
            } else {
                fileSize = (uint_64) fdFile.nFileSizeHigh << 32 | fdFile.nFileSizeLow;
                currSize += fileSize;
//                if (++*numFiles % 100000 == 0) printf("Checked: %lld Files\r", *numFiles);
//                printf("File: %s\t%lld\n", sPath, fileSize);
            }
        }
    } while (FindNextFile(hFind, &fdFile)); // Find the next file.

    FindClose(hFind); // CLEANUP: Close the Find handle

    return root;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }
    char * dir = argv[1];
    uint_64 numFiles = 0, numDirs = 0;
//    bool check = ListDirectoryContents(dir);
    clock_t begin = clock();
    uint_64 bytes = ListDirectorySize(dir, &numFiles, &numDirs);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    char *size = formatNumber(bytes);
    if (bytes < 1024) printf("Size: %s\n", size);
    else if (bytes < 1048576) printf("Size: %.2lfK \t (%s Bytes)\n", (double) bytes / 1024.0, size);
    else if (bytes < 1073741824) printf("Size: %.2lfM \t (%s Bytes)\n", (double) bytes / 1048576.0, size);
    else printf("Size: %.2lfG \t (%s Bytes)\n", (double) bytes / 1073741824.0, size);
    if (!bytes) return printf("Error: %s\n", dir) % 1 + 1;
    printf("Files: %lld\n", numFiles);
    printf("Directories: %lld\n", numDirs);
    printf("Time: %fs\n", time_spent);
    free(size);
    return 0;
}
