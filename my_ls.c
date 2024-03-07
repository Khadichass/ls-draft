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

// int my_length(ls_listnode *head)
// {
//     ls_listnode* header = head;
//     int count = 0;
//     while (header != NULL)
//     {
//         count++;
//         header = header->next;
//     }

//     return count;
// }

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
    ls_listnode *temp = head;
    while (temp != NULL)
    {
        stat(temp->file, &temp->info);
        temp = temp->next;
    }

    temp = head;
    ls_listnode *temp1 = head;

    while (temp1 != NULL)
    {
        while (temp != NULL)
        {
            if (temp->info.st_mtime > temp->next->info.st_mtime)
            {
                my_linked_list_swap(temp, temp->next);
            }
            temp = temp->next;
        }

        temp1 = temp1->next;
    }
}

void print_linked_list(ls_listnode *head)
{
    while (head != NULL)
    {
        printf("%s\n", head->file);
        head = head->next;
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

void my_ls(ls_listnode **head, int include_hidden)
{
    DIR *direct;
    direct = opendir(".");

    struct dirent *data;

    while ((data = readdir(direct)) != NULL)
    {
       if (include_hidden || data->d_name[0] != '.' || strcmp(data->d_name, ".") == 0 || strcmp(data->d_name, ".."))
       {
         newline_node(head, data->d_name);
       }
       
    }
    print_linked_list(*head);

    closedir(direct);
}

int main(int argc, char **argv)
{
    struct ls_listnode *head = NULL;
    int include_hidden = 0;
    for (int i = 0; i < argc; i++)
    {
        if (my_strcmp(argv[i], "-t") == 0)
        {
            sort_linked_list(head);
        }
        else if(my_strcmp(argv[i], "-a") == 0)
        {
            include_hidden = 1;
        }
    }
    my_ls(&head, include_hidden);
    return 0;
}