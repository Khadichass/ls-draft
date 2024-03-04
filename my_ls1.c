#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

struct node
{
    char *name;
    int lastAccessTime;
    int lastAccessTime_in_n_seconds;
    struct node *next;
};

int my_strlen(char *str)
{
    int i = 0;
    while (str[i])
    {
        i++;
    }
    return i;
}

int my_strcmp(char *a, char *b)
{
    int len = my_strlen(a), i = 0;
    int len2 = my_strlen(b);
    if (len != len2)
        return 1;
    while (a[i])
    {
        if (a[i] != b[i])
            return 1;
        i++;
    }
    return 0;
}

void add_data_to_end(struct node **head, char *data)
{
    struct node *ptr = malloc(sizeof(struct node));
    ptr->name = strdup(data);
    ptr->next = NULL;
    if (*head == NULL)
    {
        *head = ptr;
    }
    else
    {
        struct node *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = ptr;
    }
}

int counter_for_linked_list(struct node *head)
{
    int count = 0;
    struct node *temp = head;
    while (temp != NULL)
    {
        temp = temp->next;
        count++;
    }
    return count;
}

void my_linked_list_swap(struct node *ptr_1, struct node *ptr_2)
{
    struct node *temp = malloc(sizeof(struct node));
    temp->lastAccessTime = ptr_1->lastAccessTime;
    temp->name = ptr_1->name;
    temp->lastAccessTime_in_n_seconds = ptr_1->lastAccessTime_in_n_seconds;
    ptr_1->lastAccessTime = ptr_2->lastAccessTime;
    ptr_1->name = ptr_2->name;
    ptr_1->lastAccessTime_in_n_seconds = ptr_2->lastAccessTime_in_n_seconds;
    ptr_2->lastAccessTime = temp->lastAccessTime;
    ptr_2->name = temp->name;
    ptr_2->lastAccessTime_in_n_seconds = temp->lastAccessTime_in_n_seconds;
}

void sort_linked_list(struct node *head)
{
    int len_of_list = counter_for_linked_list(head);
    struct node *current = head;
    ;
    struct node *temp = head;
    for (int i = 0; i < len_of_list - 1; i++)
    {
        temp = current;
        for (int j = i + 1; j < len_of_list; j++)
        {
            temp = temp->next;
            if (current->lastAccessTime < temp->lastAccessTime)
            {
                my_linked_list_swap(current, temp);
            }
            else if (current->lastAccessTime == temp->lastAccessTime)
            {
                if (current->lastAccessTime_in_n_seconds < temp->lastAccessTime_in_n_seconds)
                {
                    my_linked_list_swap(current, temp);
                }
            }
        }
        current = current->next;
    }
}

void sort_by_time_last_access(struct node *head)
{
    struct stat stat_file;
    struct node *temp = head;
    while (temp != NULL)
    {
        stat(temp->name, &stat_file);
        temp->lastAccessTime = stat_file.st_mtimespec.tv_sec;
        temp->lastAccessTime_in_n_seconds = stat_file.st_mtimespec.tv_nsec;
        temp = temp->next;
    }
    sort_linked_list(head);
}

struct node *listDirectoryContents(int flag, char *path)
{
    DIR *direc;
    struct dirent *list;
    struct node *head = NULL;
    direc = opendir(path);
    if (direc == NULL)
    {
        return NULL;
    }
    while ((list = readdir(direc)) != NULL)
    {
        if (flag == 0 || flag == 2)
        {
            if (list->d_name[0] != '.')
            {
                add_data_to_end(&head, list->d_name);
            }
        }
        else if (flag == 1 || flag == 3)
        {
            add_data_to_end(&head, list->d_name);
        }
    }
    // head = head->next;
    if (flag == 2 || flag == 3)
    {
        sort_by_time_last_access(head);
    }
    closedir(direc);
    return head;
}

void freeLinkedList(struct node *head)
{
    struct node *current = head;
    struct node *next;
    while (current != NULL)
    {
        next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

int checkFileType(char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
        if (S_ISREG(path_stat.st_mode))
        {
            return -1;
        }
        else if (S_ISDIR(path_stat.st_mode))
        {
            return 1;
        }
        else if (my_strcmp(path, "-a") == 0 || my_strcmp(path, "-t") == 0 || my_strcmp(path, "-at") == 0 || my_strcmp(path, "-ta") == 0)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 2;
    }
}

struct node *my_option_finder(int argc, char **argv, char *path)
{
    struct node *head = NULL;
    for (int i = 1; i < argc; i++)
    {
        if (my_strcmp(argv[i], "-a") == 0)
        {
            head = listDirectoryContents(1, path);
        }
        else if (my_strcmp(argv[i], "-t") == 0)
        {
            head = listDirectoryContents(2, path);
        }
        else if (my_strcmp(argv[i], "-at") == 0 || my_strcmp(argv[i], "-ta") == 0)
        {
            head = listDirectoryContents(3, path);
        }
        else
        {
            head = listDirectoryContents(0, path);
        }
    }
    return head;
}

void mainBrain(int argc, char **argv)
{
    int flag = 0;
    struct node *head = NULL;
    if (argv[1] == NULL)
    {
        head = listDirectoryContents(0, ".");
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            flag = (checkFileType(argv[i]));
            if (flag == 2)
                head = my_option_finder(argc, argv, ".");
            else if (flag == -1)
                printf("%s\n", argv[i]);
        }
        for (int i = 1; i < argc; i++)
        {
            flag = (checkFileType(argv[i]));
            if (flag == 1)
            {
                printf("%s:\n", argv[i]);
                head = my_option_finder(argc, argv, argv[i]);
            }
        }
    }
    struct node *temp = head;
    while (temp != NULL)
    {
        printf("%s\n", temp->name);
        temp = temp->next;
    }
    freeLinkedList(head);
}

int main(int argc, char **argv)
{
    mainBrain(argc, argv);
    return 0;
}