#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct ls_listnode
{
    char *file;
    struct ls_listnode *next;
    struct stat info;
} ls_listnode;

typedef struct u_input
{
    int a;
    int t;
    int length;
    char **all_file;
    ls_listnode *files;
    ls_listnode *directs;

} input;

void print_linked_list(ls_listnode *head);
void free_linked_list(ls_listnode **head);

int my_strlen(char *string_arr)
{
    int length = 0;
    while (string_arr[length])
    {
        length++;
    }
    return length;
}

char *my_strcat(char *main_str, char *str)
{
    int len_main = my_strlen(main_str);
    int index = len_main;

    for (int i = 0; str[i] != '\0'; i++)
    {
        main_str[index] = str[i];
        index++;
    }

    return main_str;
}

char *my_strcpy(char *destination, char *source)
{
    int destination_len = my_strlen(destination);
    int source_len = my_strlen(source);
    if (destination_len < source_len)
    {
        char *temp = calloc((source_len + destination_len + 1), sizeof(char));
        my_strcat(temp, destination);
        my_strcat(temp, source);
        for (int i = 0; i <= source_len + destination_len; i++)
        {
            destination[i] = temp[i];
        }
    }
    else
    {
        for (int i = 0; source[i] != '\0'; i++)
        {
            destination[i] = source[i];
        }
    }
    return destination;
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

void fill_null(char *str)
{
    for (int i = 0; i < my_strlen(str); i++)
    {
        str[i] = '\0';
    }
}

void my_strCpy(char *dest, char *source)
{
    if (my_strlen(dest) < my_strlen(source))
    {
        dest = realloc(dest, (my_strlen(source) + 1) * sizeof(char));
    }
    fill_null(dest);

    for (int i = 0; i < my_strlen(source); i++)
    {
        dest[i] = source[i];
    }
}

void my_linked_list_swap(ls_listnode *str1, ls_listnode *str2)
{
    ls_listnode *temp = (ls_listnode *)calloc(sizeof(ls_listnode), 1);
    temp->file = calloc(sizeof(char), my_strlen(str1->file) + 1);
    strcpy(temp->file, str1->file);
    temp->info = str1->info;

    strcpy(str1->file, str2->file);
    strcpy(str2->file, temp->file);

    str1->info = str2->info;
    str2->info = temp->info;

    free(temp->file);
    free(temp);
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
        temp = head;
        while (temp->next != NULL)
        {
            if (temp->info.st_mtime < temp->next->info.st_mtime)
            {
                my_linked_list_swap(temp, temp->next);
            }
            temp = temp->next;
        }
        temp1 = temp1->next;
    }
    // free_linked_list(&temp);
    // free_linked_list(&temp1);
}

void sort_by_ascii(ls_listnode *head)
{
    ls_listnode *temp = head;

    while (temp != NULL)
    {
        stat(temp->file, &temp->info);
        temp = temp->next;
    }

    ls_listnode *temp2 = head;
    ls_listnode *temp1 = head;

    while (temp1 != NULL)
    {
        temp2 = head;

        while (temp2->next != NULL)
        {
            if (my_strcmp(temp2->file, temp2->next->file) > 0)
            {
                my_linked_list_swap(temp2, temp2->next);
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }

    // free_linked_list(&temp1);
}

void free_linked_list(ls_listnode **head)
{
    ls_listnode *curr = *head;
    ls_listnode *temp = curr;
    while (curr != NULL)
    {
        curr = curr->next;
        free(temp->file);
        free(temp);
        temp = curr;
    }
    *head = NULL;
}

void print_linked_list(ls_listnode *head)
{
    ls_listnode *copy = head;
    while (copy != NULL)
    {
        printf("%s\n", copy->file);
        copy = copy->next;
    }
    // free_linked_list(&copy);
}

void newline_node(ls_listnode **head, char *file)
{
    ls_listnode *newNode = (ls_listnode *)malloc(sizeof(ls_listnode));
    if (newNode != NULL)
    {
        newNode->file = (char *)calloc(my_strlen(file) + 1, sizeof(char));
        if (newNode->file != NULL)
        {
            my_strcpy(newNode->file, file);
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

void my_ls(ls_listnode **head, int include_dot, char *dir_name)
{

    DIR *direct;
    direct = opendir(dir_name);

    struct dirent *data;

    while ((data = readdir(direct)) != NULL)
    {
        if (include_dot || data->d_name[0] != '.')
        {
            newline_node(head, data->d_name);
        }
    }
    closedir(direct);
    // free(data);
}

int list_counter(ls_listnode *head)
{
    ls_listnode *temp = head;
    int length = 0;
    while (temp)
    {
        length++;
        temp = temp->next;
    }

    return length;
}

void control(input *us)
{
    if (us->length == 1)
    {
        ls_listnode *all = NULL;
        my_ls(&all, us->a, ".");
        sort_by_ascii(all);
        print_linked_list(all);
        free_linked_list(&all);
    }
    if (us->files)
    {
        if (us->t)
        {
            sort_linked_list(us->files);
        }
        else
        {
            sort_by_ascii(us->files);
        }
        print_linked_list(us->files);
    }
    if (us->directs != NULL)
    {
        ls_listnode *temp = us->directs;
        sort_by_ascii(temp);
        ls_listnode *inside = NULL;
        int list_length = list_counter(temp);
        while (temp)
        {
            inside = NULL;
            if (us->a)
            {
                my_ls(&inside, us->a, temp->file);
                sort_by_ascii(inside);
            }
            else if (us->t)
            {
                my_ls(&inside, us->a, temp->file);
                sort_linked_list(inside);
            }
            else
            {
                my_ls(&inside, us->a, temp->file);
                sort_by_ascii(inside);
            }

            if (list_length != 1)
            {
                printf("\n");
                sort_by_ascii(us->directs);
                printf("%s:\n", temp->file);
            }

            // if (inside == NULL)
            // {
            //     return;
            // }
            print_linked_list(inside);
            temp = temp->next;
        }
    }
    else
    {
        ls_listnode *all = NULL;
        if (us->a && us->t)
        {
            my_ls(&all, us->a, ".");
            sort_linked_list(all);
        }
        else if (us->a)
        {
            my_ls(&all, us->a, ".");
            sort_by_ascii(all);
        }
        else if (us->t)
        {
            my_ls(&all, us->a, ".");
            sort_linked_list(all);
        }
        print_linked_list(all);
    }
}

int main(int argc, char **argv)
{
    input us_inp;
    us_inp.a = 0;
    us_inp.t = 0;
    us_inp.length = argc;
    us_inp.all_file = argv;
    us_inp.directs = NULL;
    us_inp.files = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (my_strcmp(argv[i], "-a") == 0)
        {
            us_inp.a = 1;
        }
        else if (my_strcmp(argv[i], "-t") == 0)
        {
            us_inp.t = 1;
        }
        else if ((my_strcmp(argv[i], "-at") && my_strcmp(argv[i], "-ta")) == 0)
        {
            us_inp.t = 1;
            us_inp.a = 1;
        }
        else
        {
            if (open(argv[i], O_RDONLY) <= 0)
            {
                printf("ls: cannot access '%s': No such file or directory\n", argv[i]);
            }
            else
            {
                struct stat mode;
                stat(argv[i], &mode);
                if (S_ISREG(mode.st_mode))
                {
                    newline_node(&us_inp.files, argv[i]);
                }
                else
                {
                    newline_node(&us_inp.directs, argv[i]);
                }
            }
        }
    }
    control(&us_inp);
    return 0;
}
