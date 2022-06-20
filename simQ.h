#ifndef __SIMQ__
#define __SIMQ__
#include<queue.h>
#include<stdio.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>

/*Define Structures*/
struct servicePointStruct
{	
    int servicePointNumber;
    double serviceTime;
    int occupied;
    struct customerStruct customerBeingServed;
	
};

typedef struct servicePointStruct servicePoint;

/*Define function prototypes*/
void mainLoop(char *, char *, char *);
void setString(char **, char *);
void newCustomers(node **, int *, int *, int, gsl_rng *, double *,double *, double *, double *, double *);
void fillCustomerServicePoints(servicePoint *, node **, int, int *, gsl_rng *, double *);
int numberOfPeopleBeingServed(servicePoint *, int);
void initialiseServicePoints(servicePoint *, int *);
void removeServedCustomers(servicePoint *, int , int *);
void checkBoredCustomers(node **, int *);
void resetSimulation(int *, int *, int *, int *);
void setupRandomNumberGenerator( gsl_rng **);
long int poissonRandomNumber( gsl_rng *, double *);
double gaussianRandomNumber(gsl_rng *, double *, double *);
void destroyRandomNumberGenerator(gsl_rng *);
void readInputFile(int *, int *, int *, double *, double *, double *, double *, double *, char *);

#endif
