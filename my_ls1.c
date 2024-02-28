#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

struct node 
{
    char* name;
    time_t lastAccessTime;
    struct node* next;
};

int my_strlen(char* str){
    int i = 0;
    while (str[i])
    {
        i++;
    }
    return i;
}

int my_strcmp( char* a, char* b){
    int len = my_strlen(a), i = 0;
    int len2 = my_strlen(b);
    if(len != len2)
        return 1;
    while(a[i]){
        if(a[i] != b[i])
            return 1;
        i++;
    }
    return 0;
}

void add_data_to_end(struct node* head, char* data){
    struct node * ptr = malloc(sizeof(struct node));
    ptr->name = data;
    ptr->next = NULL;
    while(head->next != NULL){
        head = head->next;
    }
    head->next = ptr;
}

int counter_for_linked_list(struct node *head){
    int count = 0;
    while(head != NULL){
        head = head->next;
        count++;
    }
    return count;
}

void my_linked_list_swap(struct node *ptr_1, struct node *ptr_2){
    struct node *temp = malloc(sizeof(struct node));
    temp->lastAccessTime = ptr_1->lastAccessTime;
    temp->name = ptr_1->name;
    ptr_1->lastAccessTime = ptr_2->lastAccessTime;
    ptr_1->name = ptr_2->name;
    ptr_2->lastAccessTime = temp->lastAccessTime;
    ptr_2->name = temp->name;
}

void sort_linked_list(struct node * head){
    int len_of_list = counter_for_linked_list(head);
    struct node *temp = head;
    for(int i = 0; i < len_of_list -1; i++){
        temp = head;
        for(int j = i+1; j < len_of_list; j++){
            temp = temp->next;
            if(head->lastAccessTime < temp->lastAccessTime){
                my_linked_list_swap(head, temp);
            }
        }
        head = head->next;
    }
}

void sort_by_time_last_access(struct node * head){
    struct stat stat_file;
    struct node * temp = head;
    while(temp != NULL)
    {
        stat(temp->name, &stat_file);
        temp->lastAccessTime = stat_file.st_atime;
        // printf("%s : %ld\n", temp->name, temp->lastAccessTime);
        temp = temp->next;
    }
    temp = head;
    sort_linked_list(temp);
}

void listDirectoryContents(struct node* head, int flag, char* path) {
    DIR *direc;
    struct dirent *list;
    direc = opendir(path);
    if (direc == NULL) {
        return;
    }
    while ((list = readdir(direc)) != NULL) {
        if(flag == 0 || flag == 2) 
        {
            if(list->d_name[0] != '.')
            {
                add_data_to_end(head, list->d_name);
            }
        }
        else if (flag == 1 || flag == 3)
        {
           add_data_to_end(head, list->d_name);
        }
    }
    head = head->next;
    if(flag == 2 || flag == 3)
    {
        sort_by_time_last_access(head);
    }
    closedir(direc);
}


int checkFileType(char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == 0) {
        if (S_ISREG(path_stat.st_mode)){
            return -1;
        } else if (S_ISDIR(path_stat.st_mode)) {
            return 1;
        }else if(my_strcmp(path, "-a") == 0 || my_strcmp(path, "-t") == 0 || my_strcmp(path, "-at") == 0 || my_strcmp(path, "-ta") == 0) {
            return 0;
        }
        else{
            return 2;
        }
        
    } else {
        return 2;
    }
}

void my_option_finder(int argc, char** argv, struct node* head, char* path){
     for(int i = 2; i < argc; i++)
            {
                if(my_strcmp(argv[i],"-a") == 0)
                {
                    listDirectoryContents(head, 1, path);
                }
                else if(my_strcmp(argv[i],"-t") == 0)
                {
                    listDirectoryContents(head, 2, path);
                }
                else if(my_strcmp(argv[i],"-at") == 0 || my_strcmp(argv[i],"-ta") == 0)
                {
                    listDirectoryContents(head, 3, path);
                }
                else
                {
                    listDirectoryContents(head, 0, path);
                }
            }
}




void mainBrain(int argc, char** argv){
    int flag;
    if(my_strcmp(argv[1],"my_ls") == 0)
    {
        struct node * head = malloc(sizeof(struct node));
        if(argv[2] == NULL)
        {
            listDirectoryContents(head, 0,".");
        }
        else
        {
           for(int i = 2; i < argc; i++){
                flag = (checkFileType(argv[i]));
                if(flag == 2)
                    my_option_finder(argc, argv, head, ".");
                else if(flag == -1)
                    printf("%s\n", argv[i]);
           }
           for(int i = 2; i < argc; i++){
                flag = (checkFileType(argv[i]));
                if(flag == 1){
                    printf("%s:\n", argv[i]);
                    my_option_finder(argc, argv, head, argv[i]);
                }
           }
        }
        head = head->next;
        while(head != NULL){
            printf("%s  ", head->name);
            head = head->next;
        }
        printf("\n");
    }    
}

int main(int argc, char** argv) {
    mainBrain(argc, argv);
    return 0;
}
