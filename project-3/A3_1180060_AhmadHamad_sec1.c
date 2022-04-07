#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node
{
    char book_name[20];
    char author_name[20];
    char publish_date[20];
    char publisher_address[20];
    char category[20];
    int height;
    struct node* left, *right;
}*root=NULL;
struct node* insert(struct node* root,struct node* t);
void in_order_to_file(FILE *f, struct node *root);
void search_book_name(struct node *root,char arr[20]);
void search_authour_name(struct node *root,char arr[20]);
void in_order_to_authour(struct node *root,char arr[20],int* flag,int* x);
struct node* delete(struct node *root,char key[20]);                            // declaration of all used functions in the project
struct node* in_order_pre(struct node *p);
void user_read();
void update(char arr[20],struct node*root);
void in_order_fiction(struct node*root);
void in_order_non_fiction(struct node*root);
void print_sroted();
void file_read();
void file_print();
void menu(){
    int f = 1;
    printf("--------------welcome---------------------\n\n");
    while (f)
    {
        printf("-------------program menu-----------------\n\n");
        printf("TO read books from the file please type                  1\n");
        printf("TO insert new book to the library                        2\n");
        printf("TO search for a book in the library please type          3\n");
        printf("TO update or delete a book from the library please type  4\n");   // main menu function
        printf("TO list books by category please type                    5\n");
        printf("To print the height of the tree please type              6\n");
        printf("TO save the data to the file please type                 7\n");
        printf("TO quit from the program please type                     8\n");
        int x; //scanning a variable to decide what operation the user want to perform
        scanf("%d", &x);
        switch (x)    // invoking the suitable function for each operation
        {
            case 1:
                file_read();
                break;
            case 2:
                user_read();
                break;
            case 3:
                printf("To search using book name please type 1 or 2 to search using author name: ");
                char arr[20];
                int state;
                scanf("%d",&state);
                if(state==1)
                {
                    printf("Please enter the book name : ");
                    getchar();
                    gets(arr);
                    strtok(arr,"\n");
                    search_book_name(root,arr);
                }
                else if(state==2){
                    printf("Please enter the author name : ");
                    getchar();
                    gets(arr);
                    strtok(arr,"\n");
                    search_authour_name(root,arr);
                } else
                    printf("Invalid entry\n\n");

                break;
            case 4:
                printf("To delete a book please type 1 or 2 update an existing one: ");
                char arr1[20];
                int state1;
                scanf("%d",&state1);
                if(state1==1)
                {
                    printf("Please enter the book name : ");
                    getchar();
                    gets(arr1);
                    strtok(arr1,"\n");
                    delete(root,arr1);
                }
                else if(state1==2){
                    printf("Please enter the book name : ");
                    getchar();
                    gets(arr1);
                    strtok(arr1,"\n");
                    update(arr1,root);
                } else
                    printf("Invalid entry\n\n");
                break;
            case 5 :
               print_sroted();
                break;
            case 6:
                printf("The height of the tree is: %d\n",height_node(root)-1);
                break;
            case 7:
                file_print();
                break;
            case 8:
                f=0;
                break;
            default:
                printf("invalid entry ! \n");
                break;

        }
    }
}
void file_read(){  // a function to read the data from the file
    char arr[100];
    char temp[30];
    FILE *f=fopen("books.txt","r");
    if(!f){
        printf("file not found !\n\n");
        return;
    }
    while(fgets(arr,100,f)){  // reading the file line by line and splitting it using the "|"
        strtok(arr,"\n");
        struct node *t=(struct node*)malloc(sizeof(struct node));  // creating the new nod
        char *c=strtok(arr,"|");
        strcpy(t->book_name,c);
        c=strtok(NULL,"|");
        strcpy(t->author_name,c);
        c=strtok(NULL,"|");       // copying the read data to the new nod
        strcpy(t->publish_date,c);
        c=strtok(NULL,"|");
        strcpy(t->publisher_address,c);
        c=strtok(NULL,"|");
        strcpy(t->category,c);
        root=insert(root,t);  // sending the new nod to the insert function to add it to the tree
    }
    fclose(f);// closing the file
    printf("Read successfully\n\n");
}
void file_print(){  //  a function to print the data to the file again(it will call another recursive function to travel through the tree)
    FILE *f=fopen("books.txt","w");
    in_order_to_file(f,root);
    printf("Saved to the file successfully\n\n");
    fclose(f);

}
int height_node (struct node *p){  // a function to calculate the height of a nod
    int left_height=0,right_height=0; // initializing the heights with zero
    if(!p)  // returning 0 if the tree is empty
        return 0;
    if(p->left)  // calculating the height of the left subtree if exists
        left_height=p->left->height;
    if(p->right)  // calculating the height of the right subtree if exists
        right_height=p->right->height;
    if(left_height>right_height)
        return left_height+1;
    else                // returning the maximum height between the left and the right subtree +1
        return right_height+1;


}
int balance_factor(struct node *p){  // a function to calculate the balance factor for a nod
    int left_height,right_height;
    if(p->left)
        left_height=p->left->height;
    else
        left_height=0;                 // calculating the height of the left and right subtree if exists and returning the balance factor
                                      // where balance factor = height of the left subtree- height of the right subtree
    if(p->right)
        right_height=p->right->height;
    else
        right_height=0;
    return left_height-right_height;

}
struct node * LLrotation(struct node *p){  // a function to perform the ll rotation ( single left rotation)
    struct node *p_left= p->left;
    struct node *p_left_right=p->left->right;
    p_left->right=p;           // swapping the nodes to perform the ll rotation and get a right balance factor
    p->left=p_left_right;
    p->height=height_node(p);  // calculating the new balance factor for the swapped nodes
    p_left->height=height_node(p_left);
    if(root==p)
        root=p_left;
    return p_left; // returning the new swapped subtree
}
struct node * LRrotation(struct node *p){  // a function to perform the left right rotation
    struct node *p_left=p->left;
    struct node *p_left_right=p->left->right;
    p->left=p_left_right->right;
    p_left->right=p_left_right->left;
    p_left_right->right=p;
    p_left_right->left=p_left;                        // swapping the nodes to perform the ll rotation and get a right balance factor
    p_left->height=height_node(p_left);   // calculating the new balance factor for the swapped nodes
    p->height=height_node(p);  // calculating the new balance factor for the swapped nodes
    p_left_right->height=height_node(p_left_right); // calculating the new balance factor for the swapped nodes
    if(root==p)  // checking if the root of the sub tree is the same as the hall tree nod and moving the root to its new position if so
        root=p_left_right;
    return p_left_right;  // returning the new swapped subtree
}
struct node * RRrotation(struct node *p){
    struct node* p_right=p->right;
    struct node*p_right_left=p->right->left;
    p_right->left=p;
    p->right=p_right_left;
    p->height=height_node(p);      // calculating the new balance factor for the swapped nodes
    p_right->height=height_node(p_right);  // calculating the new balance factor for the swapped nodes
    if(p==root) // checking if the root of the sub tree is the same as the hall tree nod and moving the root to its new position if so
        root=p_right;
    return p_right;  // returning the new swapped subtree
}
struct node* RLrotation(struct node* p){  // a function that performs the right left rotation
    struct node *p_right=p->right;
    struct node*p_right_left=p->right->left;
    p->right=p_right_left->left;
    p_right->left=p_right_left->right;
    p_right_left->right=p_right;
    p_right_left->left=p;
    p_right->height=height_node(p_right);
    p->height=height_node(p);
    p_right_left->height=height_node(p_right_left);
    if(p==root) // checking if the root of the sub tree is the same as the hall tree nod and moving the root to its new position if so
        root=p_right_left;
    return  p_right_left;  // returning the new swapped subtree
}
struct node * insert (struct node*root,struct node* t){  // a function to insert new nod to the tree
    if(root==NULL){   // checking if th tree is empty and assigning the new nod as a root if so
        t->height=1;
        t->left=t->right=NULL;
        return t;
    }
    if(strcmp(t->book_name,root->book_name)==1)   // checking if the key if the new nod is greater than the current nod and moving to the right sub tree if so
        root->right=insert(root->right,t->book_name);
    else if(strcmp(t->book_name,root->book_name)==-1) // checking if the key if the new nod is smaller than the current nod and moving to the left sub tree if so
        root->left=insert(root->left,t->book_name);
    root->height=height_node(root);   // updating the height of each nod after every operation
    if(balance_factor(root)==-2 && balance_factor(root->right)==1)
        return RLrotation(root);
    else if(balance_factor(root)==-2 && balance_factor(root->right)==-1)
        return RRrotation(root);                                                  // cehcking the balance factor for all nodes and perform the suitable rotation if needed
    else if(balance_factor(root)==2 && balance_factor(root->left)==1)
        return LLrotation(root);
    else if(balance_factor(root)==2 && balance_factor(root->left)==-1)
        return LRrotation(root);
    return root;
}

void in_order_to_file(FILE *f, struct node *root){ // a function to iterate over the tree in in order to print it to the file
    if(root){
        in_order_to_file(f,root->left);
        fprintf(f,"%s|%s|%s|%s|%s\n",root->book_name,root->author_name,root->publish_date,root->publisher_address,root->category);
        in_order_to_file(f,root->right);
    }
}
void search_book_name(struct node *root,char arr[20]){  // a function to search for a book using its name
if(!root){  // if the traverse;ling nod become null then the book is not found
    printf("The book wasn't found in the library \n");
    return;
}
    if(strcmp(root->book_name,arr)==0){  // checking the current nod has the same book and giving message if so
        printf("THE book was found successfully \n Iformation : \n");
        printf("Book's name : %s\n",root->book_name);
        printf("Author's name : %s\n",root->author_name);
        printf("Publishing date : %s\n",root->publish_date);
        printf("Publisher address : %s\n",root->publisher_address);
        printf("Book's category : %s\n",root->category);
        printf("\n");
        return;
    }
    else if(strcmp(root->book_name,arr)==1) // moving the left or right subtree depending on the name of the book we are searching for and the value of the current nod
        search_book_name(root->left,arr);
    else
        search_book_name(root->right,arr);
    }
void search_authour_name(struct node *root,char arr[20]){ //  a function to search for a book ( or books) by their author name
    int flag=0,x=0;   // two flags used to print the output in a more beautiful way
    in_order_to_authour(root,arr,&flag,&x); // calling an in order function to iterate over all the tree and check if that author has books or not
    if(flag==0)  // checking if the first flag still zero then this author has no books in the library
        printf("This author doesn't have any books in this library\n\n");
}
void in_order_to_authour(struct node *root,char arr[20],int* flag,int *x){  // iterating over the tree in in order way to find the books related to that author
    if(root){
        in_order_to_authour(root->left,arr,flag,x);
        if(strcasecmp(root->author_name,arr)==0){  // if the current book has the same author as the one we are searching for
            if(*x==0) {  // printing that this author has books in the library and changing the second flag to print it once only
                printf("This author has these books in the library:\n");
                *x=1;
            }
            printf("Book's name : %s , ",root->book_name);     // printing the information of that book
            printf("Author's name : %s , ",root->author_name);
            printf("Publishing date : %s , ",root->publish_date);
            printf("Publisher address : %s , ",root->publisher_address);
            printf("Book's category : %s\n",root->category);
            *flag=1; // changing the flag that was used to check if that author has any book in the library to true
        }
        in_order_to_authour(root->right,arr,flag,x);
    }
}
struct node* delete(struct node *p,char key[20]){  // a function to delete a nod from the library
    if(!p) {
        printf("The book is not in the library !\n\n");
        return 0;
    }
    if(strcmp(key,p->book_name)==-1)  // searching for the book using its key
        p->left=delete(p->left,key);
    else if(strcmp(key,p->book_name)==1)   // searching for the book using its key
        p->right=delete(p->right,key);
    else{
        printf("Deleted successfully\n\n");
        if(!p->right&&!p->left){  // checking if the nod we want to delete does not have any children and delete it without performing any thing if so
            if(p==root)  // removing the root if that nod was the root
                root=0;
            return 0;
        }
        struct node* q=in_order_pre(p->left);   // if the nod has al least one child then we need to find its in order predecessor or in order successor here i will always use the predecessor
        strcpy(p->book_name,q->book_name);    // copyig the data of the predecessor to the nod we want to delete
        strcpy(p->author_name,q->author_name);
        strcpy(p->publish_date,q->publish_date);
        strcpy(p->publisher_address,q->publisher_address);
        strcpy(p->category,q->category);
        p->left=delete(p->left,q->book_name);  // calling the delete function recursively for every deleted nod
    }
    if(!p)
        return 0;
    p->height=height_node(p);  // updating the height of the nod
    if(balance_factor(p)==-2 && balance_factor(p->right)==1)
        return RLrotation(p);
    else if(balance_factor(p)==-2 && balance_factor(p->right)==-1)
        return RRrotation(p);                                          // perform the rotation on the new new nod if needed to maintain the balance factors
    else if(balance_factor(p)==2 && balance_factor(p->left)==1)
        return LLrotation(p);
    else if(balance_factor(p)==2 && balance_factor(p->left)==-1)
        return LRrotation(p);
    return p;


}
struct node* in_order_pre(struct node *p){  // a function to find the in order predecessor for a nod
    while(p&&p->right)
        p=p->right;
    return p;
}
void user_read(){  // a function to add a new book from the user ( works as same as the file read function)
    char arr[20];
    struct node* t=(struct node *)malloc(sizeof(struct node));
    printf("Enter the name of the book you want to add : \n");
    getchar();
    gets(arr);
    strtok(arr,"\n");
    strcpy(t->book_name,arr);
    printf("Enter the name of author of the book you want to add : \n");
    gets(arr);
    strcpy(t->author_name,arr);
    printf("Enter the publish date of the book you want to add : \n");
    gets(arr);
    strcpy(t->publish_date,arr);
    printf("Enter the address of publisher of the book you want to add : \n");
    gets(arr);
    strcpy(t->publisher_address,arr);
    printf("Enter the category of the book you want to add : \n");
    gets(arr);
    strcpy(t->category,arr);
    root=insert(root,t);
    printf("Added successfully\n\n");
}
void update(char arr[20],struct node*root){  // a function to update a book information
    if(!root){  // chwcking if we reach null while searching then the book is not found
        int st;
        printf("The Book is not in the library !");
        printf("To add new book to the library please type 1 or 2 if not");  // giving an option for the user to add the not found  book to the library
        scanf("%d",&st);
        if(st==1)
            user_read();
        else
            return;
    }
    if(strcmp(arr,root->book_name)==1)
        update(arr,root->right);                  // moving to the right or to the left subtree depending on the key value
    else if(strcmp(arr,root->book_name)==-1)
        update(arr,root->left);
    else{  // letting the user update the book if it was found
        char x[20];
        printf("Book was found \n");
        printf("Please enter the new data fields \n");
        printf("Please enter the new author name : \n");
        gets(x);
        strtok(x,"\n");
        strcpy(root->author_name,x);
        printf("Please enter the new publish date : \n");
        gets(x);
        strtok(x,"\n");
        strcpy(root->publish_date,x);
        printf("Please enter the new publisher address name : \n");
        gets(x);
        strtok(x,"\n");
        strcpy(root->publisher_address,x);
        printf("Please enter the new category  : \n");
        gets(x);
        strtok(x,"\n");
        strcpy(root->category,x);
        printf("Updated successfully\n\n");
        return ;
    }
}
void in_order_fiction(struct node*root){  //  function that will iterate in an in order way ( to print them sorted) and print the fiction books
    if(root) {
        in_order_fiction(root->left);
        if (strcasecmp(root->category, "fiction") == 0) {  // printing the book information is its category is fiction
            printf("Book's name : %s  ", root->book_name);
            printf("Author's name : %s  ", root->author_name);
            printf("Publishing date : %s  ", root->publish_date);
            printf("Publisher address : %s  ", root->publisher_address);
            printf("Book's category : %s  ", root->category);
            printf("\n");
        }
        in_order_fiction(root->right);
    }
}
void in_order_non_fiction(struct node*root){ //  function that will iterate in an in order way ( to print them sorted) and print the non fiction books
    if(root) {
        in_order_non_fiction(root->left);
        if (strcasecmp(root->category, "Non-fiction") == 0) {  // printing the book information is its category is non fiction
            printf("Book's name : %s  ", root->book_name);
            printf("Author's name : %s  ", root->author_name);
            printf("Publishing date : %s  ", root->publish_date);
            printf("Publisher address : %s  ", root->publisher_address);
            printf("Book's category : %s  ", root->category);
            printf("\n");
        }
        in_order_non_fiction(root->right);
    }
}
void print_sroted(){ // a function that will call the fiction and non fiction books print functions
    printf("The list of fictions books: \n");
    in_order_fiction(root);
    printf("\n");
    printf("The list of non-fictions books: \n");
    in_order_non_fiction(root);
    printf("\n");
}
int main() {
menu();
    return 0;
}
