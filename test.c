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


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>


struct Node {
    char* file;
    struct Node* next;
};

void newline_node(struct Node** head, char* file) {

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode != NULL) {
        newNode->file = (char*)malloc(strlen(file) + 1);
        if (newNode->file != NULL) {
            strcpy(newNode->file, file);
            newNode->next = NULL;

            if (*head == NULL) {
                
                *head = newNode;
            } else {
                struct Node* temp = *head;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        } 
    } 
}



void ls_1() {
    DIR* directory;
    struct dirent* entry;
    struct Node* head = NULL;

    directory = opendir(".");
    
    while ((entry = readdir(directory)) != NULL) {
        newline_node(&head, entry->d_name);
    }

    closedir(directory);

    struct Node* current = head;
    while (current != NULL) {
        printf("%s\n", current->file);
        current = current->next;
    }
}

int main() {
    ls_1();
    return 0;
}
