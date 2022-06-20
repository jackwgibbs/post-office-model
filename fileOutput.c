#include<fileOutput.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>


void writeParametersToFile(FILE *fpt, int  *numServicePoints, int *maxQueueLength, int *closingTime, double *poissonMean, double *waitingTimeParameterA, double *waitingTimeParameterB, double *serviceTimeParameterA, double *serviceTimeParameterB)
{
    /*Check can write to file*/
    if (fpt == NULL)
    {
        fprintf(stderr, "Could not write to file\n");
        fclose(fpt);
        exit(-1);
    }

    /*Write the data to the file*/
    fprintf(fpt, "Parameters read in from input file:\n");
    fprintf(fpt, "Number of service points: %d\n", *numServicePoints);
    fprintf(fpt, "Max queue length: %d\n", *maxQueueLength);
    fprintf(fpt, "Closing time: %d\n", *closingTime);
    fprintf(fpt, "Poisson Mean Value: %f\n", *poissonMean);
    fprintf(fpt, "Gamma waiting time parameter A: %f\n", *waitingTimeParameterA);
    fprintf(fpt, "Gamma waiting time parameter B: %f\n", *waitingTimeParameterB);
    fprintf(fpt, "Gamma service time parameter A: %f\n", *serviceTimeParameterA);
    fprintf(fpt, "Gamma service time parameter B: %f\n\n", *serviceTimeParameterB);
}

void writeIntervalToFile(FILE *fpt, int intervalNum, int numPeopleBeingServed, int peopleInQueue, int *fulfilledCustomers, int *unfulfilledCustomers, int *timedOutCustomers )
{
    /*Check can write to file*/
    if (fpt == NULL)
    {
        fprintf(stderr, "Could not write to file\n");
        fclose(fpt);
        exit(-1);
    }	

    /*Write the data to the file*/
    fprintf(fpt, "Interval %d\n",intervalNum );
    fprintf(fpt, "Number of people currently being served: %d\n", numPeopleBeingServed);
    fprintf(fpt, "Number of people currently in queue: %d\n", peopleInQueue);
    fprintf(fpt, "Fulfilled customers so far: %d\n", *fulfilledCustomers);
    fprintf(fpt, "Unfulfilled customers so far: %d\n", *unfulfilledCustomers);
    fprintf(fpt, "Timed-out customers so far: %d\n\n", *timedOutCustomers);
	
}

void writeSimulationResults(FILE *fpt,int  *fulfilledCustomers, int *unfulfilledCustomers, int *timedOutCustomers)
{
     /*Check the file can be writen ti*/
    if (fpt == NULL)
    {
        fprintf(stderr, "Could not write to file\n");
        fclose(fpt);
        exit(-1);
    }
	
    /*Write the data to the file*/
    fprintf(fpt, "Number of fulfilled customers: %d", *fulfilledCustomers);
    fprintf(fpt, "Number of unfulfilled customers: %d", *unfulfilledCustomers);
    fprintf(fpt,"Number of timed-out customers: %d", *timedOutCustomers);
}

void writeClosingTimeMarker(FILE *fpt)
{
    /*Check the file can be written to*/
    if (fpt == NULL)
    {
        fprintf(stderr, "Could not write to file\n");
        fclose(fpt);
        exit(-1);
    }	
	
    /*Write the data to the file*/
    fprintf(fpt, "Post Office now closed to new customers.\n\n");
}

void writeSimulationTimes(FILE *fpt, int timeAfterClosingTime, double avgQueueingTime)
{
    /*Check the file can be written to*/
    if (fpt == NULL)
    {
        fprintf(stderr, "Could not write to file\n");
        fclose(fpt);
        exit(-1);
    }

    /*Write the data to the file*/
    fprintf(fpt, "Time taken from closing time until all remaining customers to be served: %d\n", timeAfterClosingTime);
    fprintf(fpt, "Average waiting time for fulfilled customers in the queue: %f\n", avgQueueingTime);
}

void writeSimulationAverages(FILE *fpt, int fulfilledCustomers, int unfulfilledCustomers, int timedOutCustomers, double avgWaitingTimes, int afterClosingTime, int numSims)
{
    /*Check the file can be written to*/
    if (fpt == NULL)
    {
        fprintf(stderr, "Could not write to file\n");
        fclose(fpt);
        exit(-1);
    }

    int avgFulfilledCustomers = fulfilledCustomers / numSims;
    int avgUnfulfilledCustomers = unfulfilledCustomers / numSims;
    int avgTimedOutCustomers = timedOutCustomers / numSims;
    int avgAfterClosingTime = afterClosingTime / numSims;

    /*Write the data to the file*/
    fprintf(fpt, "Average number of fulfilled customers: %d\n", avgFulfilledCustomers);
    fprintf(fpt, "Average number of unfulfilled customers: %d\n", avgUnfulfilledCustomers);
    fprintf(fpt, "Average number of timed-out customers: %d\n", avgTimedOutCustomers);
    fprintf(fpt, "Average waiting time for fulfilled customers: %f\n", avgWaitingTimes);
    fprintf(fpt, "Average time it takes after closing to serve all remaining customers: %d\n", avgAfterClosingTime);
}

FILE *openFile(char *fileToOpen)
{
    FILE *fpt;
    /*Check the file can be opened*/
    if ((fpt = fopen(fileToOpen, "w")) == NULL)
    {
        printf("Unable to open file to write access.\n");
        fprintf(stderr, "error &d: %s\n", errno, strerror(errno));
        exit(-1);
    }

    /*Return the file pointer*/	
    return fpt;
}
