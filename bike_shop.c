#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// basic linked list node struct to hold customer information
typedef struct repairTicket
{
    char customer_fname[64];
    char customer_lname[64];
    char payment_info[64];
    char bike_make[64];
    char bike_model[64];
    char request_time[64]; //make this pull from api
    float amount_owed;
    char progress[64];
    char parts_needed[128]; //make this an array?
    struct repairTicket* next;
} repairTicket;

// basic queue struct to keep track of first and last nodes
typedef struct queue
{
    struct repairTicket* first, * last;
} queue;

// print queue
int print_queue(struct queue* q)
{
    if(q->first == NULL)
    { 
        printf("No bike repairs in progress\n");
        return 0;
    };
    // start with first node, then iterate with next until you reach NULL
    struct repairTicket* tmp = q->first;
    while(tmp != NULL)
    {
        printf("\nCustomer Name: %s %s\n", tmp->customer_fname, tmp->customer_lname);
        printf("Bike Make and Model: %s %s\n", tmp->bike_make, tmp->bike_model);
        printf("Request Time: %s\n", tmp->request_time);
        printf("Amount Owed: $%.2f\n", tmp->amount_owed);
        printf("Repair Progress: %s\n", tmp->progress);
        printf("Parts Needed: %s\n", tmp->parts_needed);
        tmp = tmp->next;
    }
    printf("\n");

    return 1;
}

// function to simplify node creation
struct repairTicket* create_ticket(char customer_fname[64], char customer_lname[64], char payment_info[64], char bike_make[64], char bike_model[64], char request_time[64])
{
    // declare struct pointer and allocate memory
    repairTicket* tmp = (repairTicket*)malloc(1 * sizeof(repairTicket));

    // fill in data
    strncpy(tmp->customer_fname, customer_fname, 64);
    strncpy(tmp->customer_lname, customer_lname, 64);
    strncpy(tmp->payment_info, payment_info, 64);
    strncpy(tmp->bike_make, bike_make, 64);
    strncpy(tmp->bike_model, bike_model, 64);
    strncpy(tmp->request_time, request_time, 64);
    tmp->amount_owed = 0;
    strcpy(tmp->progress, "Not started");
    strcpy(tmp->parts_needed, "");
    
    // set next to NULL
    tmp->next = NULL;

    return tmp;
}

// insert into end of queue (FIFO)
void Enqueue(struct queue* q, struct repairTicket* ticket)
{
    // if list is empty, first and last = n
    if(q->first == NULL)
    {
        q->first = ticket;
        q->last = ticket;
    }
    // if there is already something in the list, then (old)last->next is new node, and (new)last becomes new node
    else{
        q->last->next = ticket;
        q->last = ticket;
    }
}

// FIFO for queues, so we remove from front
void Dequeue(struct queue* q)
{
    // if list is empty, return
    if(q->first == NULL)
    {
        printf("No repairs in progress.\n");
        return;
    }
    // if there is only one element in the queue
    else if (q->first == q->last)
    {
        free(q->first);
        // set both first and last to NULL
        q->first = NULL;
        q->last = NULL;
    }
    // point tmp to second in list, free first, point first to second
    // order is important; we could lose link to remaining nodes
    else
    {
        struct repairTicket* tmp = q->first->next;
        free(q->first);
        q->first = tmp;
    }
    
}

// always init pointers to NULL
void init_queue(struct queue* q)
{
    q->first = NULL;
    q->last = NULL;
}

// recursive function to delete/free all nodes in a singly (single direction) linked list
void delete_tickets(struct repairTicket* t)
{
    // if list is empty, return
    if(t == NULL) return;

    // iterate through all elements with next
    delete_tickets(t->next);

    // free everything in reverse (after recursive call)
    free(t);
}

// function to delete/free entire queue
void delete_queue(struct queue* q)
{
    // run recursive function above
    delete_tickets(q->first);

    // set first and last to NULL
    q->first = NULL;
    q->last = NULL;
}

int main()
{
    //initialize queue
    struct queue q;
    init_queue(&q);

    //declare variables
    struct repairTicket* newTicket;
    int choice;
    do {

        //menu to user
        printf("\n-------- Menu --------\n");
        printf("1. Add Ticket to Queue\n");
        printf("2. Mark Ticket Complete\n");
        printf("3. Print Queue\n");
        printf("4. Add Part to Order\n");
        printf("5. Print Ticket History\n");
        printf("6. Empty Queue\n");
        printf("0. Exit\n");
        printf("----------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: // enqueue
            printf("Enter the new ticket information:\n");
            char customer_fname[64], customer_lname[64], payment_info[64], bike_make[64], bike_model[64], request_time[64];
            printf("Customer First Name: ");
            scanf("%63s", customer_fname);
            printf("Customer Last Name: ");
            scanf("%63s", customer_lname);
            printf("Payment Information: ");
            scanf("%63s", payment_info);
            printf("Bike Make: ");
            scanf("%63s", bike_make);
            printf("Bike Model: ");
            scanf("%63s", bike_model);
            printf("Request Time: ");
            scanf("%63s", request_time);
            newTicket = create_ticket(customer_fname, customer_lname, payment_info, bike_make, bike_model, request_time);
            Enqueue(&q, newTicket);
            break;
        case 2: // dequeue
            Dequeue(&q);
            break;
        case 3: // print queue
            printf("Queue: ");
            print_queue(&q);
            break;
        case 4: // add part to order
            // what I'm thinking is that we'll turn the parts variable in the ticket struct
            // into an array and then in this section append parts to the array for the current
            // ticket. we'll also have to handle updating the amount due based on the price, so this
            // will need user input for the part to add and the price of the part
            break;
        case 5: // print ticket history
            // what I want to do is instead of deleting the tickets when they're complete, create
            // an array of tickets so that the "mark ticket complete" will change the status of the ticket
            // to complete and append it to the array (or insert to the front depending on how we want our log to be structured)
            // so then, we can print out the history of tickets completed. We could even have it output to
            // a file, but we don't really need to so why make this more complicated than it needs to be?
            break;
        case 6: // empty queue
            printf("Emptying the queue\n");
            delete_queue(&q);
            init_queue(&q);
            break;
        case 0: //exit
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 0);

    // Clean up memory before exiting
    delete_queue(&q);

    return 0;
}
