// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 02/17/25
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.h
// PROGRAM PURPOSE :
// Contains all definitions used for P3.
// -----------------------------------------------------------

#include "ThreadClass.h"

// Global variables
extern int inputLength;
extern int *inputArray;

class EvenOddThreadSort : public Thread
{
public: EvenOddThreadSort(int indexArg);

private:
    int index;
    void ThreadFunc();
};