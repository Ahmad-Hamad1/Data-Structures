#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node   // the main struct containing all data fields with a pointer to the node
{
    int num_shares, price, day, month, year;
    char company[20];
    struct node *next;
} *head = NULL, *tail = NULL;
struct node *top1 = NULL, *top2 = NULL, *front = NULL, *rear = NULL; //global nodes one for the head of the list and two top nodes for the two stacks and front and rear pointers for the queue
int gain = 0, loss = 0;

void insert(struct node *t);

void sort();

void push_stack(struct node *t, int num);

void file_read();                                     // declaration for all functions used in the project
void user_read();

void print_total();

void sell();

void file_print();
void free();

void menu()     // main menu for the user
{
    int f = 1;
    printf("--------------welcome------------------\n\n");
    while (f)
    {
        printf("-------------program menu-----------------\n\n");
        printf("TO read shares from the file please type   1\n");
        printf("TO buy the shares manually please type     2\n");
        printf("TO sell shares please type                 3\n");
        printf("TO get gain and loss please type           4\n");
        printf("TO save the data to the file please type   5\n");
        printf("TO quit from the program please type       6\n");
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
            sell();
            break;
        case 4:
            print_total();
            break;
        case 5 :
            file_print();
            printf("Results saved successfully\n\n");
            break;
        case 6:
            file_print();
            printf("Thank you all results were saved");
            free_();
            f=0;
            break;
        default:
            printf("invalid entry ! \n");
            break;

        }
    }

}

void user_read()     // a function to take the data of shares from the user
{
    int num, p;
    struct node *t = (struct node *) malloc(sizeof(struct node));
    if (!t)    // checking if there is a space in the heap
    {
        printf("List is full !\n\n");
        return;
    }
    char date[20], comp[20];
    printf("Please enter the number of shares to buy: ");
    scanf("%d", &num);
    t->num_shares = num;
    printf("\n");
    printf("Please enter the price of each share you want to buy: ");
    scanf("%d", &p);
    t->price = p;
    printf("\n");
    printf("Please enter the name of the company you want to buy shares in: ");
    getchar();
    gets(comp);
    strcpy(t->company, comp);
    printf("\n");
    printf("Please enter the date of buying in 'day-month-year' format: ");
    gets(date);
    char *s = strtok(date, "-");
    t->day = atoi(s);
    s = strtok(NULL, "-");
    t->month = atoi(s);
    s = strtok(NULL, "-");
    t->year = atoi(s);
    printf("\n");
    insert(t); // sending the node that contains the taken data to the insert function
    printf("Bought successfully\n\n");
}

void insert(struct node *t)     // a function to insert a new node the list
{
    if (!head)   // checking if the head is still null and creating and assigning it with the tail pointer to the new node
    {
        head = (struct node *) malloc(sizeof(struct node));
        tail = (struct node *) malloc(sizeof(struct node));
        head->next = t;
        tail = head->next;
        t->next = NULL;
    }
    else      // inserting the new node to the end of the list
    {
        tail->next = t;
        t->next = NULL;
        tail = t;
    }
}

void sort()    // a function to sort the data taken from the file
{
    if (!head || !head->next) // checking if the list is empty and breaking if it is empty
        return;
    struct node *p = head->next;
    struct node *t;
    while (p)      // basic bubble sort that checks if the next node is newer than the current one and swapping if if it is newer
    {
        if (!p->next)
            break;
        int x = p->year * 365 + p->month * 30 +
                p->day;  // calculating the total number of days for the first node to use it while comparing
        t = p->next;
        while (t)
        {
            int y = t->year * 365 + t->month * 30 +
                    t->day; // calculating the total number of days for the second node to use it while comparing
            if (y < x)    // swapping the data of the nodes if the next node is newer
            {
                int shares = t->num_shares;
                int price = t->price;
                int day = t->day;
                int month = t->month;
                int year = t->year;
                char company[20];
                strcpy(company, t->company);
                t->num_shares = p->num_shares;
                t->price = p->price;
                t->day = p->day;
                t->month = p->month;
                t->year = p->year;
                strcpy(t->company, p->company);
                p->num_shares = shares;
                p->price = price;
                p->day = day;
                p->month = month;
                p->year = year;
                strcpy(p->company, company);
                x = p->year * 365 + p->month * 30 + p->day; // recalculating the total number if days after swapping
            }
            t = t->next;
        }
        p = p->next;
    }

}

void push_stack(struct node *t, int num)    // a function to push a new node to the stack
{
    if (num == 1)    // a flag to know what stack we are pushing to
    {
        if (!top1)
        {
            top1 = (struct node *) malloc(sizeof(struct node));
            top1 = t;
            t->next = 0;
        }
        else
        {
            t->next = top1;
            top1 = t;
        }
    }
    else     // pushing to second stack
    {
        if (!top2)
        {
            top2 = (struct node *) malloc(sizeof(struct node));
            top2 = t;
            t->next = 0;
        }
        else
        {
            t->next = top2;
            top2 = t;
        }

    }
}

void file_read()    // a functiong to read data from the given file
{
    FILE *f = fopen("shares.txt", "r");
    if (!f)
    {
        printf("file not found\n");
        return;
    }
    char arr[100];
    char comp[20];
    int num = 0;
    char date[20];
    while (fgets(arr, 100, f))
    {
        struct node *temp = (struct node *) malloc(sizeof(struct node));
        if (!temp)    // checking if there is a space in the heap
        {
            printf("List is full !\n\n");
            return;
        }
        strtok(arr, "\n");
        char *t = strtok(arr, ";");
        temp->num_shares = atoi(t);
        t = strtok(NULL, ";");
        temp->price = atoi(t);
        t = strtok(NULL, ";");                // reading the file line by line and splitting it by the ";"
        strcpy(temp->company, t);
        t = strtok(NULL, ";");
        char *d = strtok(t, "-");  // spliting the date to day,month,year by the "-"
        temp->day = atoi(d);
        d = strtok(NULL, "-");
        temp->month = atoi(d);
        d = strtok(NULL, "-");
        temp->year = atoi(d);
        insert(temp);  // inserting the new node to the list
    }
    sort();  // sorting the read data
    fclose(f);  // closing the file after reading from  it
    printf("Read and sorted Successfully \n\n");
}

struct node *pop_stack(int num)  // a function to pop the top of the stack
{
    if (num == 1)    // a flag to know what stack we are popping from
    {
        struct node *p = top1, *t;
        if (top1->next)
        {
            top1 = top1->next;
            t = p;
        }
        else
        {
            t = p;
            top1 = 0;
        }
        return t;
    }
    else
    {
        struct node *p = top2, *t;
        if (top2->next)
        {
            top2 = top2->next;
            t = p;
        }
        else
        {
            t = p;
            top2 = 0;
        }
        return t;
    }
}

void enqueue(struct node *t)    // a function to add new node to back of the queue
{
    if (!front)
    {
        front = (struct node *) malloc(sizeof(struct node));
        rear = (struct node *) malloc(sizeof(struct node));
        front = t;
        rear = front;
    }
    else
    {
        rear->next = t;
        rear = t;
    }
}

struct node *dequeue()    // a function to pop from the front of the queue
{
    struct node *t, *p = front;
    if (p->next)
    {
        t = p;
        front = front->next;
    }
    else
    {
        t = p;
        front = rear = NULL;
    }
    return t;
}

void fifo_sell(char comp[20], int shares, int price)    // a function to sell shares using the fifo style
{
    struct node *p = head->next, *t;
    while (p)  // copying all the nodes in the list to a queue using the enqueue method
    {
        if (!p->next)
        {
            head = 0;
        }
        t = p;
        p = p->next;
        enqueue(t);
    }
    p = front;
    int l = 0, g = 0, check = shares;
    while (p)
    {
        if (strcmp(p->company, comp) ==
                0) // checking if the current node is from the same company the user want to sell from
        {
            if (p->num_shares <=
                    shares)  // checking if the node has shares less than the remaining shares the user wants to sell
            {
                int x = (price - p->price) * p->num_shares; // calculating the total gain after this operation
                if (x > 0) // adding the total to the gain if the total gain is positive
                {
                    g += x;
                    gain += x;
                }
                else     // adding the total to the loss if the total gain is negative
                {
                    l -= x;
                    loss -= x;
                }
                shares -= p->num_shares;
                struct node *del = dequeue();  // popping the node without pushing it anywhere
                //free(del); // deleting the unused node
            }
            else if (shares >
                     0)    // if the node has shares less tha  the remaining shares to sell ( only and only if the remaining shares are not zero)
            {
                p->num_shares -= shares;  // subtracting the remaining shares from the total shares the node has
                int x = (price - p->price) * shares;
                if (x > 0)
                {
                    g += x;
                    gain += x;
                }
                else
                {
                    l -= x;
                    loss -= x;
                }
                shares = 0;
                insert(dequeue());  // inserting the node to the list again on its same position
            }
            else    // inserting nodes from  the same company to the list again if no need for their shares
            {
                insert(dequeue());
            }
        }
        else   // inserting nodes from a different company to the list again
        {
            insert(dequeue());
        }
        p = front; // assigning p to the front of the queue again
    }
    if (shares == 0)
    {
        printf("Sold successfully using lifo\n\n");
    }
    else if (shares == check)
    {
        printf("you do not have any shares to sell in %s company!\n\n", comp);
    }                                                                                                // checking if all shares are sold or not and giving suitable message to the user
    else
    {
        printf("You do not have enough shares to sell only %d shares were sold!\n\n", check - shares);
    }
    if (g - l >= 0)
    {
        printf("You have  gained  %d $ after this process\n\n", g - l);
    }
    else
        printf("You have lost %d $ after this process\n\n", l - g);


}

void sell_lifo(char comp[20], int shares, int price)  // a function using lifo style
{
    struct node *p = head->next, *t;
    int share = 0;
    while (p)  // copying all the nodes in the list to the first stack using the push method
    {
        if (!p->next)
        {
            head = 0;
        }
        t = p;
        p = p->next;
        push_stack(t, 1);
    }
    p = top1;
    int g = 0, l = 0, check = shares;
    while (p)                                  // the most of this function is same as the fifo method
    {
        if (strcmp(p->company, comp) == 0)
        {
            if (p->num_shares <= shares)
            {
                int x = (price - p->price) * p->num_shares;
                if (x > 0)
                {
                    g += x;
                    gain += x;
                }
                else
                {
                    l -= x;
                    loss -= x;
                }
                shares -= p->num_shares;
                struct node *del = pop_stack(1);
                //free(del);
            }
            else if (shares > 0)
            {
                p->num_shares -= shares;
                int x = (price - p->price) * shares;
                if (x > 0)
                {
                    g += x;
                    gain += x;
                }
                else
                {
                    l -= x;
                    loss -= x;
                }
                shares = 0;
                push_stack(pop_stack(1),
                           2);   // pushing the node to the second stack to restore the pushed nodes in the same order
            }
            else
                push_stack(pop_stack(1), 2);  // pushing nodes from the same company to the second stack

        }
        else
        {
            push_stack(pop_stack(1),2);   // pushing nodes from  the same company to the second stack  if no need for their shares
        }
        p = top1;
    }
    if (shares == 0)
    {
        printf("Sold successfully using lifo\n\n");
    }
    else if (shares == check)
    {
        printf("you do not have any shares to sell in %s company!\n\n", comp);
    }
    else                                                                                                          // checking if all shares are sold or not and giving suitable message to the user
    {
        printf("You do not have enough shares to sell only %d shares were sold!\n\n", check - shares);
    }
    if (g - l >= 0)
    {
        printf("You have  gained  %d $ after this process\n\n", g - l);
    }
    else
        printf("You have lost %d $ after this process\n\n", l - g);
    p = top2;
    while (p)    // inserting the remaining nodes again from the second stack to the list
    {
        insert(pop_stack(2));
        p = top2;
    }

}

void sell()   // a function to sell nodes
{
    if (!head || !head->next)    // checking if the list has any nodes for selling
    {
        printf("You do not have any shares to sell ! \n \n ");
        return;
    }
    int state;
    printf("To sell from last (lifo) please type 1 and to sell from first (fifo) please type 2: ");
    scanf("%d", &state);  // taking the value of the state variable from the user to determine the selling style
    printf("\n");
    if (state != 1 && state != 2)
    {
        printf("Invalid entry ! \n");
        return;
    }
    int n, p;
    char comp[20];
    printf("Plese enter the number of shares you want to sell : ");
    scanf("%d", &n);
    printf("\n");
    printf("Plese enter the price per share you want to sell : ");                // taking the selling details from the user
    scanf("%d", &p);
    printf("\n");
    printf("Plese enter the name of the company you want to sell shares in : ");
    scanf("%s", comp);
    printf("\n");
    if (state == 1)  // sending the taken details to the lifo sell function if the user choose the lifo style
        sell_lifo(comp, n, p);

    else  // sending the taken details to the fifo sell function if the user choose the fifo style
        fifo_sell(comp, n, p);
}

void
print_total()   // a function that calculate the total gain and loss for the user ,also it calculates the total money for the user
{
    printf("You have gained %d $\n", gain);
    printf("You have lost %d $\n", loss);
    if(gain-loss>=0)
        printf("You have gained a total of %d $\n", gain - loss);
    else
        printf("You have lost a total of %d $\n", loss - gain);
}

void file_print()              // a function to write the new shares after selling and buying to the file
{
    FILE *f = fopen("shares.txt", "w");
    if (!head->next)    // printing empty line to the file if the list is empty
    {
        fprintf(f, "");
        return;
    }
    struct node *p = head->next;
    while (p)    // traversing through list from the oldest to the newest shares
    {
        fprintf(f, "%d;%d;%s;%d-%d-%d\n", p->num_shares, p->price, p->company, p->day, p->month, p->year);
        p = p->next;
    }
    fclose(f);  // closing the file after writing to it

}
void free_(){  // a function to free and the data structure after exiting the program
    struct node *p=head,*t;
    if(p)
    {
        while(p)
        {
            t=p->next;
            free(p);
            p=t;
        }
    }
    p=top1;
    if (p){
        while(p){
        t=pop_stack(1);
        free(t);
        p=top1;
        }
    }
      p=top2;
    if (p){
        while(p){
        t=pop_stack(2);
        free(t);
        p=top2;
        }
    }
    p=front;
    if(p){
        while(p){
            t=dequeue();
            free(t);
            p=front;
        }
    }
free(p);
free(t);

}
int main()
{

    menu();

    return 0;
}
