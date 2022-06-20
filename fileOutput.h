#ifndef __FILEOUTPUT__
#define __FILEOUTPUT__

#include<stdio.h>

/*Function Prototypes*/
extern void writeIntervalToFile(FILE *, int, int, int, int*, int*, int*);
extern void writeParametersToFile(FILE *, int *, int *, int *, double *, double *, double*, double*, double*);
extern void writeSimulationResults(FILE *, int *, int *, int *);
extern void writeClosingTimeMarker(FILE *);
extern void writeSimulationTimes(FILE *, int, double);
extern void writeSimulationAverages(FILE *, int, int, int, double, int, int);
extern FILE *openFile(char *);

#endif
