#include<string.h>
#include<simQ.h>
#include<fileOutput.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>


int main(int argc, char **argv)
{
    /*Call the mainLoop function with the program parameters*/
    mainLoop(argv[1], argv[2], argv[3]);

    return 0;
}
void mainLoop(char *fileInParameter, char *numSimsParameter, char *fileOutParameter)
{
    /*Define variavles*/
    /*Define variable and associated pointer for the number of service points*/
    int numServicePointsValue = 0;	
    int *numServicePoints = &numServicePointsValue;

    /*Define variable and associated pointer for the maximum queue length*/
    int maxQueueLengthValue = 0;
    int *maxQueueLength = &maxQueueLengthValue;

    /*Define variable and associated pointer for the closing time of the post office*/
    int closingTimeValue = 0;
    int *closingTime = &closingTimeValue;

    /*Define variable to store the poisson mean value parameter from input file*/
    double poissonMeanValue = 0;
    double *poissonMean = &poissonMeanValue;

    /*Define variable to store the gamma paramater A for the waiting time random number*/
    double waitingTimeParameterAValue = 0;
    double *waitingTimeParameterA = &waitingTimeParameterAValue;

    /*Define variable to store the gamma parameter B for the waiting time random number*/
    double waitingTimeParameterBValue = 0;
    double *waitingTimeParameterB = &waitingTimeParameterBValue;

    /*Define variable to store the gamma parameter for the service time random number*/
    double serviceTimeParameterAValue=0;
    double *serviceTimeParameterA = &serviceTimeParameterAValue;

    /*Define variable to store the gamma parameter for the service time random number*/
    double serviceTimeParameterBValue = 0;
    double *serviceTimeParameterB = &serviceTimeParameterBValue;	

    /*Define variable and associated pointer for the number of customers in the post office*/
    int noCustomersValue = 0;	
    int *noCustomers = &noCustomersValue;

    /*Define variable and associated pointer for the number of fulfilled customers*/
    int fulfilledCustomersValue = 0;
    int *fulfilledCustomers = &fulfilledCustomersValue;

    /*Define variable and associated pointer for the number of unfulfilled customers*/
    int unfulfilledCustomersValue = 0;
    int *unfulfilledCustomers = &unfulfilledCustomersValue;

    /*Define variavle and associated pointer for the number of timed-out customers*/
    int timedOutCustomersValue = 0;
    int *timedOutCustomers = &timedOutCustomersValue;

    /*Define variavles for recording the totals for use when number of simulations > 1 to be able to calculate averages*/
    int timeAfterClosingTime = 0;
    int totalTimeAfterClosingTime = 0;
    int totalTimeSpentQueueingValue = 0;
    int *totalTimeSpentQueueing = &totalTimeSpentQueueingValue;

    /*Define pointers which will store the program parameters*/
    char *fileIn;	
    char *numSimsChr;
    char *fileOut;
    node *frontOfQueue = NULL;

    /*Define variables to keep track of total customers during multiple simulations*/
    int totalNumberCustomers = 0;
    int totalFulfilledCustomers = 0;
    int totalUnfulfilledCustomers = 0;
    int totalTimedOutCustomers = 0;
	
    /*Store program parameters in the relevant memory locations defined by the relevant pointers*/
    setString(&fileIn,fileInParameter);
    setString(&numSimsChr, numSimsParameter);
    setString(&fileOut,fileOutParameter);
    int numSims = atoi(numSimsChr);

    if(numSims<=0)
    {
        fprintf(stderr, "Ensure number of simulations is greater than 0\n");
        exit(-3);
    }
	
    /*Read in the input file data*/
    readInputFile(maxQueueLength, numServicePoints, closingTime, poissonMean, waitingTimeParameterA, waitingTimeParameterB, serviceTimeParameterA, serviceTimeParameterB, fileIn);

    /*Define an array of service points*/
    servicePoint servicePointsArray[(*numServicePoints +1)];

    /*Open the output file and write the parameters being used.*/
    FILE *fpt = openFile(fileOut);
    writeParametersToFile(fpt, numServicePoints, maxQueueLength, closingTime, poissonMean, waitingTimeParameterA, waitingTimeParameterB, serviceTimeParameterA, serviceTimeParameterB);

    int simulationCount;
    for (simulationCount = 1; simulationCount <= numSims; simulationCount++)
    { 
        /*Reset variable values for the next simulation*/	
        resetSimulation(noCustomers, fulfilledCustomers, unfulfilledCustomers, timedOutCustomers);
        timeAfterClosingTime = 0;
		
        /*Set up random number generator specific too this simulation*/
        gsl_rng *r;
        setupRandomNumberGenerator(&r);

        /*Set up and initialise service points*/	
        servicePoint  *p = servicePointsArray;
        initialiseServicePoints(p,numServicePoints);	
		
        int intervalCount = 1;
        int numCustomersLeftPostOffice = -1;

        /*Loop continues while post office is open or there are still customers inside*/
        while (numCustomersLeftPostOffice !=* noCustomers || intervalCount <= *closingTime)
        {	
            /*Step 1: Have customers finished being served*/
            removeServedCustomers(p, *numServicePoints, fulfilledCustomers);
							
            /*Step 2: Move front of queue customer to available service point*/
            fillCustomerServicePoints(p, &frontOfQueue, *numServicePoints, totalTimeSpentQueueing, r, poissonMean);
			
            /*Step 3:Any bored customers leave queue - check waiting tolerance*/
            checkBoredCustomers(&frontOfQueue, timedOutCustomers);
		
            /*Step 4: 0 or more customers arrive and join the back of the queue if the post office is still open.*/
            if (intervalCount<(*closingTime))
            {
                newCustomers(&frontOfQueue, noCustomers,unfulfilledCustomers,* maxQueueLength, r, poissonMean, waitingTimeParameterA, waitingTimeParameterB, serviceTimeParameterA, serviceTimeParameterB);
            }
            else if (intervalCount == *closingTime && numSims == 1)
            {
                /*If post office is closing on this interval step, write marker to the output file.*/
                writeClosingTimeMarker(fpt);
                timeAfterClosingTime++;
            }
            else
            {
                timeAfterClosingTime++;
            }	

            /*Print out the current state of the queue after simulation run*/
            /*printLinkedList(frontOfQueue);*/	
		
            /*Check how many people have left the post office*/	
            numCustomersLeftPostOffice = (*fulfilledCustomers)+(*unfulfilledCustomers) + (*timedOutCustomers);	
			
            if (numSims == 1)
            {
                int numPeopleInQueue= numberOfPeopleInQueue(frontOfQueue);
                int numberOfCustomersBeingServed = numberOfPeopleBeingServed(p, *numServicePoints);
                writeIntervalToFile(fpt, intervalCount, numberOfCustomersBeingServed, numPeopleInQueue, fulfilledCustomers, unfulfilledCustomers, timedOutCustomers);			
            }
            intervalCount++;
        }

        /*Update totals since simulation has finished. Totals used after all simulations to calculate averages*/
        totalTimeAfterClosingTime = (totalTimeAfterClosingTime) + timeAfterClosingTime;
        totalNumberCustomers = totalNumberCustomers + *noCustomers;
        totalFulfilledCustomers = totalFulfilledCustomers + *fulfilledCustomers;
        totalUnfulfilledCustomers = totalUnfulfilledCustomers + *unfulfilledCustomers;
        totalTimedOutCustomers = totalTimedOutCustomers + *timedOutCustomers;
	
        /*Free the r pointer for the random number generator*/	
        destroyRandomNumberGenerator(r);
    }
	
    /*Calculate the average queuing time for fulfilled customes*/
     double avgQueueingTimes =(double) *totalTimeSpentQueueing /(double) totalFulfilledCustomers;

    if (numSims == 1)
    {
        /*Write times data to output file if number of simulations = 1.*/
        writeSimulationTimes(fpt, totalTimeAfterClosingTime, avgQueueingTimes);
    }
    else
    {
        /*Write averages data to file if number of simulations > 1.*/
        writeSimulationAverages(fpt,totalFulfilledCustomers, totalUnfulfilledCustomers, totalTimedOutCustomers,avgQueueingTimes , totalTimeAfterClosingTime, numSims);
    }

    /*Free memory blocks allocated by malloc*/
    free(fileIn);
    free(numSimsChr);
    free(fileOut);
    fclose(fpt);
}

void setString(char **str, char *value)
{
    /*Allocate correct amount of memory*/
    if (!(*str = (char *)malloc(sizeof(char)*20)))
    {
        fprintf(stderr, "Insufficient memory\n");
        exit(-1);
    }
    /*Copy value into *str*/
    strcpy(*str, value);
}

void newCustomers(node **front, int *noCustomers, int *unfulfilledCustomers, int maxQueueLength, gsl_rng *r, double *poissonMean, double *waitingParamA, double *waitingParamB, double *serviceParamA, double *serviceParamB)
{
    /*Generate a random number from the poisson distribution*/
    long int numberOfCustomersArriving = poissonRandomNumber(r, poissonMean);	
    int custCount;

    for (custCount = 0; custCount < numberOfCustomersArriving; custCount++)
    {
        /*Ceate a new customer*/
        customer *newCustomer;
        (*noCustomers)++;	

        /*Allocate enough memory for the new customer*/
        if ((newCustomer = (customer *)malloc(sizeof(customer))) == NULL)
        {
            fprintf(stderr, "Insufficient Memory");
            exit(-1);
        }	
        /*Fill in customer details*/
        newCustomer->customerNumber = (*noCustomers);
        newCustomer->waitingTolerance = gaussianRandomNumber(r, waitingParamA, waitingParamB);
        newCustomer->serviceTime = gaussianRandomNumber(r, serviceParamA, serviceParamB);	
        newCustomer->timeSpentInQueue = 0;
		
        /*Check if there is enough room in queue for the new customer*/
        int numPeopleInQueue = numberOfPeopleInQueue(*front);
        if (numPeopleInQueue <  maxQueueLength || maxQueueLength == -1)
        {	
            /*If the queue is not full, add customer to the back of it*/
            addCustomer(&(*front), &newCustomer);
        }
        else
       {
            /*If the queue is full, the customer does not join it and leaves unfulfilled*/
            (*unfulfilledCustomers)++;
       }
    }
}

void  fillCustomerServicePoints(servicePoint *p, node **front, int numServicePoints, int *totalTimeSpentQueueing, gsl_rng *r, double *poissonMean)
{
    int isServicePointOccupied;
    int servicePointCount;

    for (servicePointCount = 0; servicePointCount < numServicePoints; servicePointCount++)
    {	
        /*Check if service point is occupied*/
        isServicePointOccupied = p[servicePointCount].occupied;	
        if ((isServicePointOccupied == 0) && (*front !=NULL))
        {
            /*If not occupied and the queue not empty, add customer to the service point*/
            customer nextCustomerToBeServed = *((*front)->data);
            p[servicePointCount].customerBeingServed=nextCustomerToBeServed;
            p[servicePointCount].occupied = 1;
            p[servicePointCount].serviceTime = poissonRandomNumber(r, poissonMean);
            int timeSpentQueueing = nextCustomerToBeServed.timeSpentInQueue;
			
            /*Add how long they queued for to the queueing total, to calculate averages at the end*/
            (*totalTimeSpentQueueing) = *totalTimeSpentQueueing + timeSpentQueueing;		
            customer *custToRemove = ((*front)->data);
            node *custNodeToRemove = (*front);

            /*Remove customer from the queue*/
            removeCustomer(&(*front));
				
            /*Free the memory block accociated with customer*/
            free(custToRemove);
            free(custNodeToRemove);	
        }
    }
}

int numberOfPeopleBeingServed(servicePoint *p, int numServicePoints)
{
    int isServicePointOccupied;
    int servicePointCount;
    int numPeopleBeingServed = 0;
    /*Iterate over each service point*/
    for (servicePointCount = 0; servicePointCount <= numServicePoints; servicePointCount++)
    {	
        /*Check if the service point is occupied*/
        isServicePointOccupied = p[servicePointCount].occupied;
        if (isServicePointOccupied == 1)
        {
            numPeopleBeingServed++;
        }
    }
    return numPeopleBeingServed;
}

void initialiseServicePoints(servicePoint *p, int * numServicePoints)
{
    int servicePointCount;
    for (servicePointCount = 0; servicePointCount < *numServicePoints; servicePointCount++)
    {
        /*For each position in the service point array, ad a customer point structure.*/
        servicePoint newSP;
        newSP.servicePointNumber = servicePointCount;		
        newSP.occupied = 0; 
        p[servicePointCount] = newSP;
    }
}

void removeServedCustomers(servicePoint *p, int numServicePoints, int *fulfilledCustomers)
{
    int servicePointCount;
    for (servicePointCount = 0; servicePointCount <= numServicePoints; servicePointCount++)
    {
         /*Check if the service point is occupied*/
         int isServicePointOccupied = p[servicePointCount].occupied;
         if (isServicePointOccupied == 1)
             {
             double serviceTimeRemaining = p[servicePointCount].serviceTime;
             if (serviceTimeRemaining <= 0)
             {
                 /*If customer has finished being served, set service point to not occupied*/
                 customer customerServed = p[servicePointCount].customerBeingServed;
                 p[servicePointCount].occupied = 0;
		
                 /*Increase fulfilled customers by 1*/
                 (*fulfilledCustomers)++;
             }
             else
             {
                 /*If not yet finsihed being served, decrease service time by 1.*/
                 p[servicePointCount].serviceTime--;
             }
        }
    }
}

void checkBoredCustomers(node **frontOfQueue, int *timedOutCustomers)
{
    /*Loop through the queue (linked list) and check tolerance levels of customers*/
    node *temp = *frontOfQueue;
    customer *currentCustomer = NULL;

    while (temp != NULL)
    {
        /*Get the tolerance of the current customer*/
        currentCustomer = (temp)->data;
        double tolerance = currentCustomer->waitingTolerance;
		
        /*Check if their tolerance level has been reached (less than 0)*/
        if (tolerance <= 0)
        {	
            /*If tolerance level has been reached, remove customer from queue*/
            int custNum = currentCustomer->customerNumber;
            removeBoredCustomers(&(*frontOfQueue),custNum);
            (*timedOutCustomers)++;
            break;
        }
        else
        {
            /*Move onto the next customer*/
            temp = temp->nextNode;
        }
    }

    /*Set the temporary queue equal to the front of the actual queue*/
    temp = *frontOfQueue;

    /*Reduce each customers tolerance by one and increase time spent queueing by 1.*/
    while (temp != NULL)
    {
        currentCustomer = (temp)->data;
        double tolerance = currentCustomer->waitingTolerance;
        int timeSpentQueueingSoFar = currentCustomer->timeSpentInQueue;
        int updatedTimeSpentQueueing = timeSpentQueueingSoFar + 1;
        double newTolerance = tolerance - 1;
        (temp->data)->waitingTolerance = newTolerance;
        (temp->data)->timeSpentInQueue = updatedTimeSpentQueueing;
        temp=temp->nextNode;
    }

    /*Free the memory associated with temp*/
    free(temp);
}

void resetSimulation(int *noCustomers, int *fulfilledCustomers, int *unfulfilledCustomers, int *timedOutCustomers)
{
     /*Reset values pointed to by pointers for next simulation run*/
     *noCustomers = 0;
     *fulfilledCustomers = 0;
     *unfulfilledCustomers = 0;
     *timedOutCustomers = 0;
}

void setupRandomNumberGenerator(gsl_rng **r)
{	
    /*Set up random number generator using GSL library*/
    const gsl_rng_type *T;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    *r = gsl_rng_alloc(T);
    gsl_rng_set(*r, time(0));
}

long int poissonRandomNumber( gsl_rng *r, double *poissonMean)
{
    /*Return a number from the poisson distribution with a mean of poissonMean*/
    return gsl_ran_poisson(r, *poissonMean);
}

double gaussianRandomNumber(gsl_rng *r, double *gammaParameterA, double *gammaParameterB)
{
    /*Return a number from the gamma distribution with parameters A and B*/
    return gsl_ran_gamma(r, *gammaParameterA, *gammaParameterB);
}

void destroyRandomNumberGenerator(gsl_rng *r)
{
    /*Free memory*/
    gsl_rng_free(r);
}

void readInputFile(int *maxQueueLength, int *numServicePoints, int *closingTime, double  *poissonMean, double *gammaParameterA, double *gammaParameterB, double *gammaParameterServiceA, double *gammaParameterServiceB, char *inputFile)
{
    /*Define variables/pointers required to read file*/
    FILE *fp;

    if ((fp = fopen(inputFile, "r"))==NULL)
    {
        /*Check if file cannot be opened*/
        fprintf(stderr,"File not openable\n");
        exit(-2);
    }
    int inputArray[4];
    double parameterArray[6];
    char tempString[30];
    int count = 0;
    int parameterCount = 0;

    /*Read the file*/
    while (!feof(fp))
    {
        /*Read in simulator parameters*/

	
        if (count <= 2)
        {
            if ((fscanf(fp,"%s - %d\n",&tempString,  &inputArray[count]) != 2) || inputArray[count] <=0)
	    {
                fprintf(stderr, "Invalid file format\n");
                fclose(fp);
                exit(-2);
            }
        }	
        else
        {
        /*Read in random number parameters*/
            if (fscanf(fp, "%s - %lf\n", &tempString, &parameterArray[parameterCount]) !=2 )
            {
                fprintf(stderr, "Invalid file format");
                fclose(fp);
                exit(-2);
            }
            parameterCount++;
        }
        count++;
   }
   /*Write the read values into the correct memory address via pointers*/
    *maxQueueLength = 10;
    *numServicePoints = inputArray[1];
    *closingTime = inputArray[2];
    *poissonMean = parameterArray[0];
    *gammaParameterA = parameterArray[1];
    *gammaParameterB = parameterArray[2];
    *gammaParameterServiceA = parameterArray[3];
    *gammaParameterServiceB = parameterArray[4];

    /*Check values in range*/
    if ((*maxQueueLength <=-2 && *closingTime<=0 && *numServicePoints <= 1))
    {
        fprintf(stderr, "Invalid input parameters\n");
        fclose(fp);
        exit(-2);
    }
    /*Close the file*/
    fclose(fp);
}
