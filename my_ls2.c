#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

struct ListNode {
    char* filename;
    time_t lastAccessTime;
    struct ListNode* next;
};

int myStrlen(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

int myStrcmp(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] == str2[i]) {
        if (str1[i] == '\0')
            return 0;
        i++;
    }
    return (str1[i] - str2[i]);
}

void insertNode(struct ListNode** headRef, char* filename) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->filename = strdup(filename); // Using strdup to allocate memory for the filename
    newNode->next = NULL;

    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }

    struct ListNode* current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

void swapNodes(struct ListNode* a, struct ListNode* b) {
    char* tempFilename = a->filename;
    a->filename = b->filename;
    b->filename = tempFilename;

    time_t tempTime = a->lastAccessTime;
    a->lastAccessTime = b->lastAccessTime;
    b->lastAccessTime = tempTime;
}

void sortByLastAccessTime(struct ListNode* head) {
    struct stat fileInfo;
    struct ListNode* current = head;

    while (current != NULL) {
        stat(current->filename, &fileInfo);
        current->lastAccessTime = fileInfo.st_atime;
        current = current->next;
    }

    struct ListNode* ptr1 = head;
    while (ptr1 != NULL) {
        struct ListNode* ptr2 = ptr1->next;
        while (ptr2 != NULL) {
            if (ptr1->lastAccessTime < ptr2->lastAccessTime) {
                swapNodes(ptr1, ptr2);
            }
            ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
}

void listDirectoryContents(const char* directory, struct ListNode** fileList, int showHidden) {
    DIR* dir = opendir(directory);
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (showHidden || (entry->d_name[0] != '.')) {
            insertNode(fileList, entry->d_name);
        }
    }

    closedir(dir);
}

void printFileList(struct ListNode* head) {
    while (head != NULL) {
        printf("%s  ", head->filename);
        head = head->next;
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-a] [-t] <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int showHidden = 0;
    int sortByTime = 0;
    int directoryIndex = 1;

    // Process command-line options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "-at") == 0 || strcmp(argv[i], "-ta") == 0) {
            showHidden = 1;
        }
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-at") == 0 || strcmp(argv[i], "-ta") == 0) {
            sortByTime = 1;
        }
        if (strcmp(argv[i], "-a") != 0 && strcmp(argv[i], "-t") != 0 && strcmp(argv[i], "-at") != 0 && strcmp(argv[i], "-ta") != 0) {
            directoryIndex = i; // Get the index of the directory argument
        }
    }

    struct ListNode* fileList = NULL;
    listDirectoryContents(argv[directoryIndex], &fileList, showHidden);

    if (fileList == NULL) {
        printf("No files found in the directory.\n");
        return EXIT_SUCCESS;
    }

    if (sortByTime) {
        sortByLastAccessTime(fileList);
    }

    printFileList(fileList);

    // Free memory
    struct ListNode* current = fileList;
    while (current != NULL) {
        struct ListNode* temp = current;
        current = current->next;
        free(temp->filename);
        free(temp);
    }

    return EXIT_SUCCESS;
}
