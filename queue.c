#include<queue.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

static node *createCustomerNode(customer *newCustomer)
{
    /*Create a new node for the customer*/
    node *newNode = NULL;
	
    /*Allocate enough memory*/
    newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation error for newNode\n");
        exit(-1);
    }
	
    /*Set the node data to the customer structure, and nextNode to NULL since it is at the end of the queue*/
    newNode->data = newCustomer;
    newNode->nextNode = NULL;
    return newNode;
}

extern void addCustomer(node **frontOfQueue, customer **newCustomer)
{
    /*Create a new customer node*/
    node *newNode = createCustomerNode(*newCustomer);
    if (!*frontOfQueue)
    {	
        /*If the queue is empty, make it the head*/
        *frontOfQueue = newNode;
    }
    else
    {
        /*If the queue is not empty, go to the end of the linked list and add there (to the back of the queue)*/
        node *temp;
        temp = *frontOfQueue;

        while (temp->nextNode != NULL)
        {
            temp = temp->nextNode;
        }

        temp->nextNode = newNode;
        temp = newNode;
    }
}

extern void removeCustomer(node **frontOfQueue)
{
    /*Change the nextNode the first element in linked list is pointing to, so that
 *  the second element in the queue moves to the front*/
    if (*frontOfQueue != NULL)
    {
        *frontOfQueue = (*frontOfQueue)->nextNode;
    }	
}

extern void printLinkedList(node *frontOfQueue)
{	
    /*Check if the queue is empty*/
    if (frontOfQueue != NULL)
    {
        printf("Customer %d\n", (frontOfQueue->data)->customerNumber);
        printLinkedList(frontOfQueue->nextNode);
    }
}

extern int numberOfPeopleInQueue(node *frontOfQueue)
{ 
    int count = 0;
 
    /*Traverse the linked list and maintain the count. */
    while(frontOfQueue != NULL)
    {
        frontOfQueue = frontOfQueue->nextNode;
        count++;
    }

    return count;
}

extern void removeBoredCustomers(node **frontOfQueue, int custNum)
{
    /*Define prev and temp pointers*/
    node *prev = NULL;
    node *temp = *frontOfQueue;

    /*Get the customer number of the customer at the front of the queue*/
    int frontOfQueueCustNum = ((*frontOfQueue)->data)->customerNumber;
	
    /*Check if the customer number equals that of the customer that needs to be removed, i.e. the customer to be removed is at head of queue*/
    if ((*frontOfQueue != NULL) && (custNum == frontOfQueueCustNum))
    {
        *frontOfQueue = (*frontOfQueue)->nextNode;
        return;
    }
    else
    {
        /*If customer to be removed is not at the head of the queue, traverse the queue to find them*/
        while(temp != NULL && frontOfQueueCustNum != custNum)
        {
            prev = temp;
            temp = temp->nextNode; 
            frontOfQueueCustNum = (temp->data)->customerNumber;
            if (temp == NULL)
            {
               fprintf(stderr, "Error loacating customer in queue");
            }
         }
         prev->nextNode = temp->nextNode;
    }
}
