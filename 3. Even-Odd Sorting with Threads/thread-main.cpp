// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 02/17/25
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
// Contains the main implementations of the P3.
// -----------------------------------------------------------

#include <iostream>
#include "thread.h"

int inputLength;
int *inputArray;

int main(int argc, char *argv[])
{
    cin >> inputLength; //Read the number of elements in the array from standard input
    
    //Allocate memory and read elements for the input array
    inputArray = new int[inputLength];
    for (int i = 0; i < inputLength; i++)
    {
        cin >> inputArray[i];
    }

    //Output the initial state of the array
    cout << "Concurrent Even-Odd Sort\n\n";
    cout << "Number of input data = " << inputLength << "\nInput array:\n";
    for (int i = 0; i < inputLength; i++)
    {
        cout << inputArray[i] << " ";
    }
    cout << endl;

    //Flag to track if a swap happened in the last pass
    bool isSwapped = true;
    int iteration = 0;

    //Perform sorting until no swaps are needed
    while (isSwapped)
    {
        isSwapped = false;
        cout << "Iteration " << iteration + 1 << ":\n";

        //Handle even indexed elements
        cout << "Even Pass:\n";
        EvenOddThreadSort *threadArray[inputLength];
        for (int j = 1; j < inputLength; j += 2)
        {
            threadArray[j] = new EvenOddThreadSort(j);
            threadArray[j]->Begin(); //Start the thread
        }
        for (int j = 1; j < inputLength; j += 2)
        {
            threadArray[j]->Join(); //Wait for the thread to finish
            delete threadArray[j];
        }

        //Handle odd indexed elements
        cout << "Odd Pass:\n";
        for (int j = 2; j < inputLength; j += 2)
        {
            threadArray[j] = new EvenOddThreadSort(j);
            threadArray[j]->Begin();
        }
        for (int j = 2; j < inputLength; j += 2)
        {
            threadArray[j]->Join();
            delete threadArray[j];
        }

        //Check the array for any remaining unsorted elements
        for (int j = 1; j < inputLength; j++)
        {
            if (j + 1 < inputLength && inputArray[j - 1] > inputArray[j])
            {
                isSwapped = true;
                break;
            }
        }

        //Output the result after current iteration
        cout << "Result after iteration " << iteration + 1 << ":\n";
        for (int i = 0; i < inputLength; i++)
        {
            cout << inputArray[i] << " ";
        }
        cout << endl;
        iteration++;
    }

    //Output the final sorted array
    cout << "Final result after " << iteration << " iterations:\n";
    for (int i = 0; i < inputLength; i++)
    {
        cout << inputArray[i] << " ";
    }
    cout << endl;

    //Clean up allocated memory
    delete[] inputArray;
    return 0;
}
