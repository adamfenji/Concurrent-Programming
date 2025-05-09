// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 02/17/25
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
// Contains all class implementations of the sort used in P3.
// -----------------------------------------------------------

#include <iostream>
#include <sstream>
#include <algorithm>
#include "thread.h"

EvenOddThreadSort::EvenOddThreadSort(int indexArg) { index = indexArg; } //Initialize thread with its specific index

void EvenOddThreadSort::ThreadFunc()
{

    Thread::ThreadFunc(); //Thread setup
    std::ostringstream os; //Stream for building output strings

    //Log thread creation with its index
    os << "        Thread " << index << " Created\n";
    write(1, os.str().c_str(), os.str().length());
    os.str("");

    //Log comparison of current and next array elements
    os << "        Thread " << index << " compares x[" << index - 1 << "] = " << inputArray[index - 1]
       << " and x[" << index << "] = " << inputArray[index] << "\n";
    write(1, os.str().c_str(), os.str().length());
    os.str("");

    //Swap if necessary and log the swap action
    if (index > 0 && inputArray[index - 1] > inputArray[index])
    {
        std::swap(inputArray[index - 1], inputArray[index]); //Swap elements

        //Log swap
        os << "        Thread " << index << " swaps x[" << index - 1 << "] = " << inputArray[index - 1]
           << " and x[" << index << "] = " << inputArray[index] << "\n";
        write(1, os.str().c_str(), os.str().length());
        os.str("");
    }

    //Log thread exit
    os << "        Thread " << index << " exits\n";
    write(1, os.str().c_str(), os.str().length());
    os.str("");

    //Properly exit the thread
    Exit();
}
