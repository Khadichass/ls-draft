#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

typedef struct ls_listnode
{
    char *file;
    struct ls_listnode *next;
    struct stat info;
} ls_listnode;

int my_strlen(char *str_arr)
{
    int length = 0;
    while (str_arr[length] != '\0')
    {
        length++;
    }
    return length;
}

int my_length(ls_listnode *head)
{
    ls_listnode *header = head;
    int count = 0;

    while (header != NULL)
    {
        count++;
        header = header->next;
    }

    return count;
}

int my_strcmp(char *str1, char *str2)
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

void newline_node(ls_listnode **head, char *file)
{
    ls_listnode *newNode = (ls_listnode *)malloc(sizeof(ls_listnode));
    if (newNode != NULL)
    {
        newNode->file = (char *)malloc(strlen(file) + 1);
        if (newNode->file != NULL)
        {
            strcpy(newNode->file, file);
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

// void my_option(ls_listnode *head)
// {
//     int flag = 0;
//     while (my_length(head))
//     {
//         if (my_strcmp(head->file, "-t") == 0)
//         {
//             sort_linked_list(head);
//             flag = 1;
//         }
//         head = head->next;
//     }
// }

void print_linked_list (ls_listnode* head)
{
    DIR *direct;
    direct = opendir(".");
    head = NULL;

    struct dirent *data;

    while ((data = readdir(direct)) != NULL)
    {
      
        newline_node(&head, data->d_name);
        
    }

    sort_linked_list(head);
    // my_option(head);
    ls_listnode *current = head;

    while (current != NULL)
    {
        printf("%s\n", current->file);
        current = current->next;
    }

    closedir(direct);
}

void my_ls(int argc, char **argv)
{
    struct ls_listnode* head = NULL;
    
    for (int i = 0; i < argc; i++)
    {
        if (my_strcmp(argv[i], "-t") == 0)
        {
            sort_linked_list(head);
        }
        
    }
    print_linked_list(head);
}

int main(int argc, char **argv)
{
    my_ls(argc, argv);
    return 0;
}
