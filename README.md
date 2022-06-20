# post-office-model
Post Office Modelling Program.

## Description
A C program to simulate a post ofice. Simulates customers arriving, queuing, being served, and leaving. It uses a Queue structure to keep track of entering customers, and random number distributions (Poisson and Gamma distributions).

## How to run

Step 1:
The first step is to ensure you have permission to run the compile and link script. Enter the following command to ensure you do:

chmod u+x compileSim

Step 2:
Now we definitely have permission, we can run the compileSim script to compile and link the code. In order to do this, run the following command:

./compileSim

Step 3:
To run the simulator, run the following command 

./simQ testInput.txt 1000 outputFile.txt

This command will run the simulator with input file textInput.txt, run 1000 simulations, and write the results to a text file called outputFile.txt

Alternatively, without using the dedicated compileSim script, you can compile, link and run the simulator with the following commands.

gcc -ansi -I./ -c simQ.c -o simQ.o

gcc -ansi -I./ -c queue.c -o queue.o

gcc -ansi -I./ -c fileOutput.c -o fileOutput.o

gcc simQ.o queue.o fileOutput.o -lgsl -lgslcblas -o simQ

./simQ testInput.txt 1000 outputFile.txt
