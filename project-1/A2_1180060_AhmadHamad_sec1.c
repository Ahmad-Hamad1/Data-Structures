#include <stdio.h>
#include <stdlib.h>
#include<string.h>
struct node
{
    char arr[16];
    struct node *next;
}*first=NULL,*first2=NULL;   // two global pointers the first one for the sorted list and the second to save the unsorted one
void load_menu()
{
    printf("---------------------Welcome----------------------------\n\n");
    int x;
    int flag=1;
    do
    {
        printf("Program Menu:\n*********************************************\n");
        printf("1- To read data from file please type       1\n");  // main menu to give the user the option to choose what option he want to perform
        printf("2- To sort the data please type             2\n");
        printf("3- To print the unsorted data please type   3\n");
        printf("4- To print the sorted data please type     4\n");
        printf("5- To quit from program please type         5\n");
        scanf("%d",&x);
        printf("\n");
        switch(x)
        {
        case 1 :
            read();
            break;
        case 2:
            radix_sort();
            break;
        case 3:
            print(1);
            break;
        case 4:
            print(0);
            break;
        case 5:
            printf("See you soon");
            flag=0;
            free_();
            break;
        default :
            printf("Invalid entry please try again\n");
            break;
        }
        printf("\n\n");
    }
    while(flag);

}
void read()  // function to read the data from the specified file
{
    FILE *ptr=fopen("strings.txt","r");
    if(!ptr) // checking if the file is not found
    {
        printf("file not found");
        return;
    }
    char temp[16];
    int status=fscanf(ptr,"%s",temp);
    if(!status)  // checking if the file is empty
    {
        printf("Empty file");
        return;
    }
    while(status!=EOF)  // loop that will iterate through the file line by line
    {
        if(first->next==NULL)  // checking if the list is empty and crating the first node of it if it is empty
        {
            first2->next=(struct node *)malloc(sizeof(struct node)); // for the sorted one
            first->next=(struct node *)malloc(sizeof(struct node));  // for the not sorted one
            strcpy(first->next->arr,temp);
            strcpy(first2->next->arr,temp);
            first->next->next=0;
            first2->next->next=0;
        }
        else  // if the list is not empty we need to add the node to the last of the list
        {
            struct node *t=(struct node *)malloc(sizeof(struct node));     //creating the two nodes
            struct node *t2=(struct node *)malloc(sizeof(struct node));
            struct node *p=first->next;
            struct node *p2=first2->next;
            strcpy(t->arr,temp);  // copying data to he new nodes
            strcpy(t2->arr,temp);
            while(p->next!=0)  // a loop that will iterate through the last node of the list
            {
                p=p->next;
                p2=p2->next;
            }
            p->next=t;
            t->next=0;
            p2->next=t2;      // assigning the new nodes by connecting it with the next of the old last node and pointing its next to null
            t2->next=0;
        }
        status=fscanf(ptr,"%s",temp);
    }
    printf("Read successfully \n");
    fclose(ptr);//closing the file
}
int count()  // a simple function to count the number of nodes (strings) in the list
{
    struct node *p=first->next;
    int s=0;
    while(p)
    {
        s++;
        p=p->next;
    }
    return s;
}
int longest()  // a function that will return the length of the longest word in the list
{
    int max_=0;
    struct node *p=first->next;
    while(p)
    {
        if(strlen(p->arr)>max_)
            max_=strlen(p->arr);
        p=p->next;
    }
    return max_;
}
void radix_sort()  // the radix sort function
{
    struct node *p=first->next;
    if(!p)   // checking if the list is empty to not perform any operation
    {
        printf("Empty list ! \n");
        return;
    }
    char  temp [count()][16];  // array of strings that will be used to store the strings temporarily before returning them to the list
    int n=1,index;
    for(int i=longest()-1; i>-1; i--)  // a loop that will iterate through the length of the longest string in the list
    {
        struct node *poi[63]= {0}; // array of pointers that will be used to store each letter words by skipping the 0 index for the words short words
        p=first->next;
        while(p)  // a loop that will iterate through all words in the list
        {
            struct node *t=(struct node *)malloc(sizeof(struct node));
            strcpy(t->arr,p->arr);
            int g=strlen(p->arr);
            if(i>=g) // if the length of the word is less than i then it should e added to the zero index
            {
                index=0;
            }
            else   // otherwise it should be added to its last letter index ( 1 for 1 11 for A 12 for a 13 for B.....etc)
            {
                if(p->arr[i]<65)
                    index=p->arr[i]-'0'+1;
                else if(p->arr[i]<97)
                    index=11+((p->arr[i]-'A')*2);
                else
                    index=12+((p->arr[i]-'a')*2);

            }
            if(!poi[index])  // checking if this letter has any previous strings or not and creating it if not
            {
                poi[index]=(struct node *)malloc(sizeof(struct node));
                poi[index]->next=t;
                t->next=0;
            }
            else // otherwise the list is added to the end of the letter list
            {
                struct node *r=poi[index]->next;
                while(r->next!=0)
                    r=r->next;
                r->next=t;
                t->next=0;
            }
            p=p->next;
        }
        int t=0;
        for(int j=0; j<63; j++)  // transferring all the stored words from there letters to a temp array
        {
            if(!poi[j])
                continue;
            else
            {
                struct node *m=poi[j]->next;
                while(m)
                {
                    strcpy(temp[t++],m->arr);
                    m=m->next;
                }
            }
        }
        struct node *d=first->next;
        int q=0;
        while(d)  // returning back the strings to the list with the new order
        {
            strcpy(d->arr,temp[q++]);
            d=d->next;
        }

    }
    printf("Sorted successfully \n"); // a message to the user to show that the sort operation done successfully
}
void print(int flag)  // a function to print the list that will take parameter to decide which list to print(the original or the sorted one)
{
    struct node *p;
    if(flag==0) // printing the sorted one
    {
        printf("The sorted list: ");
        p=first->next;
    }
    else // printing the non sorted one
    {
        printf("The unsorted list: ");
        p=first2->next;
    }
    if(!p)
    {
        printf("Empty list ! \n");
        return;
    }
    while(p)  // loop that will iterate through the previously specified list
    {
        printf("%s ",p->arr);
        p=p->next;
    }
    printf("\n");
}
void free_()  // a function that will remove all nodes when closing the program
{
    struct node *p=first->next;
    if(!p)  // checking if the list already has nodes or not
    {
        printf("The list is already empty !\n");
        return;
    }
    else
    {
        struct node *t=first2->next;  // iterating through all nodes and removing them one by one
        struct node *del;
        while(p)
        {
            del=p;
            p=p->next;
            free(del);
            del=t;
            t=t->next;
            free(del);
        }

    }
}
int main()
{
    first=(struct node*)malloc(sizeof(struct node));  // creating the head of each list
    first->next=0;
    first2=(struct node*)malloc(sizeof(struct node));
    first2->next=0;
    load_menu();
}
