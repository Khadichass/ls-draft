#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
typedef struct options
{
    char *name;
    struct options *next;
    struct stat info;
} listnode;

void print_line(listnode *node, int flag)
{
    if (flag == 1)
    {
        while (node)
        {
            printf("%s\n", node->name);
            node = node->next;
        }
    }

    if (flag == 2)
    {
        while (node)
        {
            if (node->name[0] != '.')
            {
                printf("%s\n", node->name);
            }
            node = node->next;
        }
    }
}

void swap(listnode *list1, listnode *list2)
{
    listnode *temp = (listnode *)malloc(sizeof(listnode));
    temp->name = malloc(sizeof(char) * strlen(list1->name));
    temp->info = list1->info;
    temp->name = list1->name;

    list1->name = list2->name;
    list1->info = list2->info;

    list2->info = temp->info;
    list2->name = temp->name;

    free(temp);
}

void new_list(listnode **list, char *name, struct stat info)
{
    listnode *node = (listnode *)malloc(sizeof(listnode));
    listnode *forward = *list;
    node->name = malloc(sizeof(char) * strlen(name));
    node->info = info;
    node->name = name;
    node->next = NULL;

    if (*list == NULL)
        *list = node;

    else
    {
        while (forward->next)
            forward = forward->next;
        forward->next = node;
    }
}

void sort_by_last_mod(listnode **list)
{
    listnode *node1 = *list;
    listnode *node2;

    while (node1)
    {
        node2 = node1->next;
        while (node2)
        {

            if (node1->info.st_mtim.tv_sec == node2->info.st_mtim.tv_sec)
            {
                if (node1->info.st_mtim.tv_nsec <= node2->info.st_mtim.tv_nsec)
                {
                    swap(node1, node2);
                }
            }
            else if (node1->info.st_mtim.tv_sec < node2->info.st_mtim.tv_sec)
            {
                swap(node1, node2);
            }
            node2 = node2->next;
        }
        node1 = node1->next;
    }
    free(node1);
    free(node2);
}

void sort_by_ascii(listnode *list)
{
    listnode *node1 = list;
    listnode *node2;

    while (node1)
    {
        node2 = node1->next;
        while (node2)
        {
            if (strcmp(node1->name, node2->name) > 0)
                swap(node1, node2);
            node2 = node2->next;
        }
        node1 = node1->next;
    }
    free(node1);
    free(node2);
}

listnode *all_dirs()
{
    listnode *list = NULL;
    DIR *directory;
    directory = opendir(".");
    struct dirent *page;

    while ((page = readdir(directory)))
    {
        struct stat info;
        stat(page->d_name, &info);
        new_list(&list, page->d_name, info);
    }
    return list;
    free(list);
}

void files_in_folder(listnode *list, int flag, int num_files, int j, int folders, int count)
{
    listnode *files = NULL;
    DIR *direct;
    direct = opendir(list->name);
    struct dirent *page;

    while ((page = readdir(direct)))
    {
        struct stat info;
        stat(page->d_name, &info);
        new_list(&files, page->d_name, info);
    }
    if (count != 0 || num_files != 0)
        putchar('\n');
    if (num_files > 0 || j || folders > 1)
        printf("%s:\n", list->name);

    if (flag == 1)
    {
        sort_by_last_mod(&files);
        print_line(files, 2);
    }
    else if (flag == 2)
    {
        sort_by_last_mod(&files);
        print_line(files, 1);
    }
    else if (flag == 3)
    {
        sort_by_ascii(files);
        print_line(files, 1);
    }
    else
    {
        sort_by_ascii(files);
        print_line(files, 2);
    }
    free(files);
}

void files_folders(char **arr, int flag)
{
    listnode *files = NULL;
    listnode *folders = NULL;
    int file = 0, folder = 0, j = 0;

    for (int i = 0; arr[i]; i++)
    {
        if (open(arr[i], O_RDONLY) > 0)
        {
            // printf("%s  ", arr[i]);
            struct stat mode;
            stat(arr[i], &mode);
            if (S_ISDIR(mode.st_mode))
            {
                new_list(&folders, arr[i], mode);
                folder++;
            }
            else
            {
                new_list(&files, arr[i], mode);
                file++;
            }
        }

        
        else
        {
            printf("ls: cannot access '%s': No such file or directory\n", arr[i]);
            j++;
        }
    }

    if (file > 0)
    {
        if (flag == 1)
        {
            sort_by_last_mod(&files);
            print_line(files, 2);
        }

        else if (flag == 2)
        {
            sort_by_last_mod(&files);
            print_line(files, 1);
        }
        else if (flag == 3)
        {
            sort_by_ascii(files);
            print_line(files, 1);
        }
        else
        {
            sort_by_ascii(files);
            print_line(files, 2);
        }
    }

    if (folder)
    {
        int count = 0;
        while (folders)
        {
            sort_by_ascii(folders);
            files_in_folder(folders, flag, file, j, folder, count);
            folders = folders->next;
            count++;
        }
    }
    free(folders);
    free(files);
}

void input(int a, int t, int j, int all, char **str, int argc)
{
    if (argc == 1)
    {
        listnode *ls = all_dirs();
        sort_by_ascii(ls);
        print_line(ls, 2);
    }
    else if (a && t == 0 && j == 0 && all == 0)
    {
        listnode *ls = all_dirs();
        sort_by_ascii(ls);
        print_line(ls, 1);
    }
    else if (t && a == 0 && j == 0 && all == 0)
    {
        listnode *ls = all_dirs();
        sort_by_last_mod(&ls);
        print_line(ls, 2);
    }
    else if ((all || (a == 1 && t == 1)) && j == 0)
    {
        listnode *ls = all_dirs();
        sort_by_last_mod(&ls);
        print_line(ls, 1);
    }
    else if (j && t == 0 && a == 0 && all == 0)
    {
        files_folders(str, 0);
    }
    else if (j && t && a == 0 && all == 0)
    {
        files_folders(str, 1);
    }
    else if (j && a && t == 0 && all == 0)
    {
        files_folders(str, 3);
    }
    else if (j && (all || (t == 1 && a == 1)))
    {
        files_folders(str, 2);
    }
}

int main(int argc, char **argv)
{
    char **str = (char **)malloc(sizeof(char *) * argc);
    int a = 0, t = 0, all = 0, j = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            a++;
        }
        else if (strcmp(argv[i], "-t") == 0)
        {
            t++;
        }
        else if (strcmp(argv[i], "-ta") == 0 || strcmp(argv[i], "-at") == 0)
        {
            all++;
        }
        else
        {
            str[j] = malloc(strlen(argv[i]));
            str[j++] = argv[i];
        }
    }
    input(a, t, j, all, str, argc);
    free(str);
}