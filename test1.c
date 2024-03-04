#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct ls_listnode
{
    char *file;
    struct ls_listnode *next;
    struct stat info;
} ls_listnode;

// Function to calculate string length
int my_strlen(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

// Function to compare two strings
int my_strcmp(const char *str1, const char *str2)
{
    int i = 0;
    while (str1[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            return 1;
        }
        i++;
    }
    return 0;
}

// Function to swap two elements in linked list
void my_linked_list_swap(ls_listnode *str1, ls_listnode *str2)
{
    char *tempF = str1->file;
    str1->file = str2->file;
    str2->file = tempF;

    struct stat tempT = str1->info;
    str1->info = str2->info;
    str2->info = tempT;
}

// Function to sort linked list by modification time
void sort_linked_list(ls_listnode *head)
{
    int swapped = 1;
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
    }
}

// Function to create a new node and add it to the end of the linked list
void newline_node(ls_listnode **head, const char *file)
{
    ls_listnode *newNode = (ls_listnode *)malloc(sizeof(ls_listnode));
    if (newNode != NULL)
    {
        newNode->file = strdup(file);
        if (newNode->file != NULL)
        {
            newNode->next = NULL;
            if (*head == NULL)
            {
                *head = newNode;
            }
            else
            {
                ls_listnode *temp = *head;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }
    }
}

// Function to print the linked list
void print_linked_list(ls_listnode *head)
{
    while (head != NULL)
    {
        printf("%s\n", head->file);
        head = head->next;
    }
}

// Function to free memory allocated for the linked list
void free_linked_list(ls_listnode *head)
{
    ls_listnode *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp->file);
        free(temp);
    }
}

// Function to read directory contents and populate the linked list
// Function to read directory contents and populate the linked list
void read_directory(ls_listnode **head)
{
    DIR *directory;
    struct dirent *entry;
    struct stat file_stat;
    directory = opendir(".");
    if (directory != NULL)
    {
        while ((entry = readdir(directory)) != NULL)
        {
            if (stat(entry->d_name, &file_stat) == 0)
            {
                newline_node(head, entry->d_name);
                (*head)->info = file_stat;
            }
        }
        closedir(directory);
    }
    else
    {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }
}

void sort_alphabetically(ls_listnode *head)
{
    int swapped;
    ls_listnode *ptr1;
    ls_listnode *lptr = NULL;

    /* Checking for empty list */
    if (head == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->file, ptr1->next->file) > 0)
            {
                my_linked_list_swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Function to sort linked list by modification time
void sort_by_time(ls_listnode *head)
{
    sort_linked_list(head);
}

// Main function
int main(int argc, char *argv[])
{
    ls_listnode *head = NULL;
    read_directory(&head);

    // Check for options
    for (int i = 1; i < argc; i++)
    {
        if (my_strcmp(argv[i], "-t") == 0)
        {
            sort_by_time(head);
        }
        else if (my_strcmp(argv[i], "-a") == 0)
        {
            // Display hidden files
            print_linked_list(head);
        }
        else if (my_strcmp(argv[i], "-at") == 0 || my_strcmp(argv[i], "-ta") == 0)
        {
            // Sort by time
            sort_by_time(head);
            // Display hidden files
            print_linked_list(head);
        }
    }

    print_linked_list(head);
    free_linked_list(head);

    return 0;
}
