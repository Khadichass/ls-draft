#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

typedef struct Node
{
    char* file;
    struct Node* next;
    time_t t_last_option;
}ls_listnode;

int my_strlen(char* str_arr)
{
    int length = 0;

    while (str_arr[length] != '\0')
    {
      length++;  
    }

    return length;
}

int my_strcmp(char* str1, char*str2)
{
    int i = 0;
    while (str1[i] != '\0')
    {
        if(str1[i] != str2[i])
        {
            return 1;
        }      
        i++;
    }

    return 0;
}

int my_length(ls_listnode* head)
{
  ls_listnode* header = head;
  int count = 0;
  while (header != NULL)
  {
    count++;
    header = header->next;
  }

  return count;
}

void newline_node(ls_listnode** head, char* file) {

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

void my_ls(int argc, char** argv)
{
    DIR* direct;
    direct = opendir(".");
    ls_listnode* head = NULL;

    // if(direct == NULL)
    // {
    //     return 1;
    // }
    
    struct dirent* data;

    while ((data = readdir(direct)) != NULL) {
        newline_node(&head, data->d_name);
    }

    ls_listnode* current = head;

    while (current != NULL)
    {
        printf("%s\n", current->file);
        current = current->next;
    }

    closedir(direct);
}

int main(int argc, char** argv)
{
    my_ls(argc, argv);
}