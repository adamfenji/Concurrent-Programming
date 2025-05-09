// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: M94542250
// DUE DATE : 01/24/2025
// PROGRAM ASSIGNMENT #1
// FILE NAME : prog1.c
// PROGRAM PURPOSE :
// This program simulates three separate processes: 
// calculating Fibonacci numbers, 
// approximating the area of an ellipse using random sampling, 
// and simulating a pinball game to analyze ball distribution across bins.
// -----------------------------------------------------------

#include <stdio.h>      //sprintf(), printf()
#include <stdlib.h>     //malloc(), free(), rand(), srand(), exit(), atol()
#include <math.h>       //pow(), round(), and M_PI
#include <string.h>     //memset(), strlen()
#include <time.h>       //time()
#include <unistd.h>     //write(), fork(), and getpid()
#include <sys/wait.h>   //wait()

//Recursive function to calculate the nth Fibonacci number
long fib(long n) {
   if(n <= 1) return n;
   else return (fib(n - 1) + fib(n - 2));
}
//Start the fib function, handle outputs as well
int fib_start(long n) {
    char *buffer = malloc(128);
    if (buffer == NULL) {
        write(1, "Memory allocation failed\n", 26);
        return -1;
    }

    //Fibonacci process start message
    write(1, "    Fibonacci Process Started\n", 30);
    sprintf(buffer, "    Input Number %ld\n", n);
    write(1, buffer, strlen(buffer));

    //Calculate Fibonacci number
    memset(buffer, 0, 128);
    long result = fib(n);
    sprintf(buffer, "    Fibonacci Number f(%ld) is %ld\n", n, result);
    write(1, buffer, strlen(buffer));

    //Fibonacci process exit message
    memset(buffer, 0, 128);
    write(1, "    Fibonacci Process Exits\n", 28);

    //Free allocated memory
    free(buffer);
    return 0;
}

//Approximates the area of an ellipse
float ellipseAreaApprox (long a, long b, long s) {
	char * buffer = malloc(256);
    if (buffer == NULL) {
        write(1, "Memory allocation failed\n", 26);
        return -1;
    }

    float x, y, hits;
	srand((unsigned) time(NULL)); //Seed random number generator

    //Generate points and calculate hits
    hits = 0.0;
	for (long i = 0; i < s; i++) {
		x = (float)rand() / ((float)RAND_MAX / (float)a);
        y = (float)rand() / ((float)RAND_MAX / (float)b);
		if ((pow(x, 2) / pow((float)a, 2)) + (pow(y, 2) / pow((float)b, 2)) <= 1.0) hits++;
	}
	
	//Print the total hits
    sprintf(buffer, "         Total Hits %ld\n", (long)hits);
    write(1, buffer, strlen(buffer));
    memset(buffer, 0, 256);
	
	//Calculate and print the estimated area
    float estimatedArea = ((hits / s) * (float)a * (float)b) * 4;
    sprintf(buffer, "         Estimated Area is %.6f\n", estimatedArea);
    write(1, buffer, strlen(buffer));
    memset(buffer, 0, 256);
	
	//Calculate and print the actual area
    float actualArea = (M_PI * a * b);
    sprintf(buffer, "         Actual Area is %.6f\n", actualArea);
    write(1, buffer, strlen(buffer));
    memset(buffer, 0, 256);
	
	//Free allocated memory
    free(buffer);

    return estimatedArea;
}
//Start the ellipseAreaApprox function, handle outputs as well
int ellipseAreaApprox_start(long a, long b, long s) {
	char * buffer = malloc(256);
    if (buffer == NULL) {
        write(1, "Memory allocation failed\n", 26);
        return -1;
    }

    //Print process start message
    write(1, "         Ellipse Area Process Started\n", 38);

	//Print input parameters
    sprintf(buffer, "         Total Random Number Pairs %ld\n", s);
    write(1, buffer, strlen(buffer));
    memset(buffer, 0, 256);
    sprintf(buffer, "         Semi-Major Axis Pairs %ld\n", a);
    write(1, buffer, strlen(buffer));
    memset(buffer, 0, 256);
    sprintf(buffer, "         Semi-Minor Axis Pairs %ld\n", b);
    write(1, buffer, strlen(buffer));
    memset(buffer, 0, 256);

    //Call the function
	ellipseAreaApprox(a, b, s);

	//Print exit message
    write(1, "         Ellipse Area Process Exits\n", 36);

    //Free allocated memory
    free(buffer);
    return 0;
}

//Simulates a pinball game to calculate ball distribution across bins
int pinball(long x, long y) {
    char *buffer = malloc(256);
    if (buffer == NULL) {
        write(1, "Memory allocation failed\n", 26);
        return -1;
    }

    //Initialize bins
    long bins[x];
    for (long i = 0; i < x; i++) {
        bins[i] = 0;
    }

    srand((unsigned) time(NULL)); //Seed random number generator

    //Simulate ball droppings
    for (long i = 0; i < y; i++) {

        //Simulate the ball moving through the pins
        long currentBin = 0;
        for (long j = 0; j < x - 1; j++) {
            float random = (float)rand() / ((float)RAND_MAX / 1);
            if (random >= 0.5) currentBin++;
        }
        bins[currentBin]++;
    }

    //Find the highest percentage for scaling
    float highestPercentage = 0.0;
    for (long i = 0; i < x; i++) {
        float percentage = ((float)bins[i] / (float)y) * 100;
        if (percentage > highestPercentage)  highestPercentage = percentage;
    }

    //Print the histogram
    for (long i = 0; i < x; i++) {
        sprintf(buffer, "%3ld-(%7ld)-(%5.2f%%)|", i + 1, bins[i], ((float)bins[i] / (float)y) * 100);
        write(1, buffer, strlen(buffer));

        //Add the histogram asterisks
        for (int j = 0; j < (round((((float)bins[i] / (float)y) * 100 / highestPercentage) * 50)); j++) {
            write(1, "*", 1);
        }
        write(1, "\n", 1);
    }

    free(buffer); //Free allocated memory
    return 1;
}
//Start the pinball function, handle outputs as well
int pinball_start(long x, long y) {
    char *buffer = malloc(256);
    if (buffer == NULL) {
        write(1, "Memory allocation failed\n", 26);
        return -1;
    }

    //Print process start message
    write(1, "Simple Pinball Process Started\n", 31);

    //Print input parameters
    sprintf(buffer, "Number of Bins %ld\n", x);
    write(1, buffer, strlen(buffer));
    sprintf(buffer, "Number of Ball Droppings %ld\n", y);
    write(1, buffer, strlen(buffer));

    //pinball function
    pinball(x, y);

    //Print process exit message
    write(1, "Simple Pinball Process Exits\n", 29);

    free(buffer); //Free allocated memory
    return 0;
}

//Main program entry point, has 3 processes at the same time
int main(int argc, char *argv[]) {
    
    if (argc != 7) {
        printf("Usage: ./prog1 n a b s x y\n");
        return -1;
    }

    //buffer for output
    char *buffer = malloc(256);
    if (buffer == NULL) {
        write(1, "Memory allocation failed\n", 26);
        return -1;
    }

    //Main process starts
    write(1, "Main Process Started\n", 21);

    //Print input parameters
    const char *paramDescriptions[] = {
        "Fibonacci Input            =",
        "Total Random Number Pairs  =",
        "Semi-Major Axis Length     =",
        "Semi-Minor Axis Length     =",
        "Number of Bins             =",
        "Number of Ball Droppings   ="
    };

    for (int i = 1; i <= 6; i++) {
        sprintf(buffer, "%s %ld\n", paramDescriptions[i - 1], atol(argv[i]));
        write(1, buffer, strlen(buffer));
    }

    //Fork child processes
    pid_t pid[3];
    int status;

    //Fibonacci Process
    if ((pid[0] = fork()) == 0) {
        write(1, "Fibonacci Process Created\n", 26);
        fib_start(atol(argv[1]));
        exit(0);
    }

    //Ellipse Area Process
    if ((pid[1] = fork()) == 0) {
        write(1, "Ellipse Area Process Created\n", 29);
        ellipseAreaApprox_start(atol(argv[3]), atol(argv[4]), atol(argv[2]));
        exit(0);
    }

    //Pinball Game Process
    if ((pid[2] = fork()) == 0) {
        write(1, "Pinball Process Created\n", 24);
        pinball_start(atol(argv[5]), atol(argv[6]));
        exit(0);
    }

    //Main process waits for children to finish
    write(1, "Main Process Waits\n", 19);
    for (int i = 0; i < 3; i++) {
        wait(&status);
    }

    //Main process exits
    write(1, "Main Process Exits\n", 19);

    free(buffer);
    return 0;
}