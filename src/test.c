#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    struct node
    {
        char data[100];
        struct node* next;
    };
    
    struct node* head = NULL;
    
    struct node* getnode(char *v)
    {
        struct node* newnode = (struct node*)malloc(sizeof(struct node));
        strcpy(newnode->data,v);
        newnode->next = NULL;
        return newnode;
    };
    
    void insert(char *v)
    {
        struct node* newnode = getnode(v);
        newnode->next = head;
        head = newnode;
    }
    
    void print()
    {
        struct node* temp = head;
        //struct node* end = NULL;
        int c = 1;
        while(temp->next != NULL)
        {
            temp = temp->next;
            c++;
        }
        temp = head;
        char x[100];
        for(int i=0;i<c;i++)
        {
            temp = head;
            while(temp->next != NULL)
            {
                if(strcmp(temp->data,temp->next->data) > 0)
                {
                    strcpy(x,temp->data);
                    strcpy(temp->data,temp->next->data);
                    strcpy(temp->next->data,x);
                }
                temp = temp->next;
            }
            //end = temp;
        }
        temp = head;
        while(temp != NULL)
        {
            printf("%s\t",temp->data);
            temp = temp->next;
        }
        printf("\n\n The Length of the list : %d",c);
    }
    
    int main()
    {
        printf("Hello world!\n");
        char v[100];
        while(1)
        {
            printf("enter a string : ");scanf("%s",v);
            if(v[0] == 'Q' || v[0] == 'q')
            {
                break;
            }
            else
            {
                insert(v);
            }
        }
        print();
        return 0;
    }
    
