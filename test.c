// #include <stdio.h>

// void my_swap(int *a, int *b, int *c) {
//     int temp = *a;
//     *a = *c;
//     *c = *b;
//     *b = temp;
// }

// int main() {
//     int x = 5;
//     int y = 10;
//     int z = 15;

//     printf("%d %d %d\n", x, y, z);

//     my_swap(&x, &y, &z);

//     printf("%d %d %d\n", x, y, z);




//     return 0;
// }


// #include <stdio.h>
// #include <stdlib.h>
// #include <dirent.h>
// #include <string.h>


// struct Node {
//     char* file;
//     struct Node* next;
// };

// void newline_node(struct Node** head, char* file) {

//     struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
//     if (newNode != NULL) {
//         newNode->file = (char*)malloc(strlen(file) + 1);
//         if (newNode->file != NULL) {
//             strcpy(newNode->file, file);
//             newNode->next = NULL;

//             if (*head == NULL) {
                
//                 *head = newNode;
//             } else {
//                 struct Node* temp = *head;
//                 while (temp->next != NULL) {
//                     temp = temp->next;
//                 }
//                 temp->next = newNode;
//             }
//         } 
//     } 
// }



// void ls_1() {
//     DIR* directory;
//     struct dirent* entry;
//     struct Node* head = NULL;

//     directory = opendir(".");
    
//     while ((entry = readdir(directory)) != NULL) {
//         newline_node(&head, entry->d_name);
//     }

//     closedir(directory);

//     struct Node* current = head;
//     while (current != NULL) {
//         printf("%s\n", current->file);
//         current = current->next;
//     }
// }

// int main() {
//     ls_1();
//     return 0;
// }



#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef struct ls_listnode {
    char *file;
    struct ls_listnode *next;
    struct stat info;
} ls_listnode;

void my_linked_list_swap(ls_listnode *str1, ls_listnode *str2)
{
    char *tempF = str1->file;
    str1->file = str2->file;
    str2->file = tempF;

    struct stat tempT = str1->info;
    str1->info = str2->info;
    str2->info = tempT;
}

void sort_linked_list(ls_listnode *head)
{
    int swapped = 1;
    ls_listnode *last = NULL; 
    while (swapped)
    {
        swapped = 0;
        ls_listnode *current = head;
        ls_listnode *nextNode = head->next;
        while (nextNode != NULL)
        {
            if (current->info.st_mtime <= nextNode->info.st_mtime)
            {
                my_linked_list_swap(current, nextNode);
                swapped = 1; 
            }
            current = nextNode;
            nextNode = nextNode->next;
        }
        last = current;
    }
}


ls_listnode* create_ls_node(char *file, struct stat info) {
    ls_listnode *newNode = (ls_listnode *)malloc(sizeof(ls_listnode));
    if (newNode != NULL) {
        newNode->file = file;
        newNode->next = NULL;
        newNode->info = info;
    }
    return newNode;
}


int main() {
    ls_listnode *head = create_ls_node("file1.txt", (struct stat){.st_mtime = 100});
    head->next = create_ls_node("file2.txt", (struct stat){.st_mtime = 200});
    head->next->next = create_ls_node("file3.txt", (struct stat){.st_mtime = 50});
    head->next->next->next = create_ls_node("file4.txt", (struct stat){.st_mtime = 150});
    
    ls_listnode *current = head;
    while (current != NULL) {
        printf("%s - %ld\n", current->file, current->info.st_mtime);
        current = current->next;
    }
    sort_linked_list(head);
    
    current = head;
    while (current != NULL) {
        printf("%s - %ld\n", current->file, current->info.st_mtime);
        current = current->next;
    }    
    return 0;
}
