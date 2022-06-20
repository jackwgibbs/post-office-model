#ifndef __QUEUE__
#define __QUEUE__

/*Define Structures*/
struct customerStruct
{
    double waitingTolerance;
    double serviceTime;
    int customerNumber;
    int timeSpentInQueue;
};


struct queueStruct
{
    struct customerStruct *data;
    struct queueStruct *nextNode;
};

typedef struct queueStruct node;
typedef struct customerStruct customer;

/*Function Prototypes*/
static node *createCustomerNode(customer *);
extern void addCustomer(node **, customer **);
extern void removeCustomer(node **);
extern void printLinkedList(node *);
extern int numberOfPeopleInQueue(node *);
extern void removeBoredCustomer(node **, int);

#endif
