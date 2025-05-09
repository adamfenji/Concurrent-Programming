// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 03/19/2025
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
// This file implements the functionality for the TStudent and TLandlord classes, defining thread behavior and synchronization logic to manage interactions in a shared party room environment.
// -----------------------------------------------------------

#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

Semaphore exitSemaphore("LeaveRoom", 1);
Semaphore enterSemaphore("AccessRoom", 1);
Mutex printMutex("OutputSync");

int TStudent::currentStudentTracker = 0;
int TStudent::totalActiveStudentProcess = 0;
int TLandlord::completed = 0;

TStudent::TStudent(int i) : studentID(i + 1) {}
TLandlord::TLandlord(int m, int n) : maxLandlordCheck(m), maxStudentsAllowed(n), counter(0) {}

void TStudent::ThreadFunc()
{

    Thread::ThreadFunc();

    // Print when the student thread starts
    printMutex.Lock();
    std::cout << "     Student " << studentID << " starts\n";
    printMutex.Unlock();

    int partyAttempts = 0; // Track party attempts for debug

    // Keep going to the party until the landlord evicts all students
    while (!TLandlord::completed)
    {
        if (TLandlord::completed)
            break; // Check eviction before partying
        Delay();
        GoToParty();
        partyAttempts++;
        Delay();
    }

    // Print eviction message and decrement the total student count
    printMutex.Lock();
    std::cout << "     Student " << studentID << " is evicted and terminates\n";
    TStudent::totalActiveStudentProcess--;
    printMutex.Unlock();

    Exit();
}

void TLandlord::ThreadFunc()
{

    Thread::ThreadFunc(); // Initialize the thread

    int studentsRemaining = 0;                 // Debug: Track remaining students
    bool evictionStarted = false;              // Debug: Track eviction state
    int lastCheckIteration = maxLandlordCheck; // Debug: Track last iteration

    // Print when the landlord thread starts
    printMutex.Lock();
    std::cout << "The landlord starts\n";
    printMutex.Unlock();

    // Loop for the specified number of apartment checks
    for (int i = 0; i < maxLandlordCheck; i++)
    {

        counter = i;
        Delay();
        CheckRoom();

        // Debug: Track number of students after each check
        studentsRemaining = TStudent::totalActiveStudentProcess;

        // If this is the last check, begin eviction process
        if ((i + 1) == maxLandlordCheck)
        {

            evictionStarted = true; // Debug: Mark eviction started

            // Print eviction decision message
            printMutex.Lock();
            std::cout << "After checking the room " << maxLandlordCheck << " times, the landlord decides to evict all the students\n";
            printMutex.Unlock();

            TLandlord::completed = 1; // Mark eviction as started

            // Print final clearance message
            printMutex.Lock();
            std::cout << "The landlord finishes clearing the room for the last time, and now evicts the students\n";
            printMutex.Unlock();

            // Wait for all students to finish before proceeding
            while (TStudent::totalActiveStudentProcess != 0)
            {
                studentsRemaining = TStudent::totalActiveStudentProcess; // Update debug counter
                // printMutex.Lock();
                // printf("DEBUG: Waiting for students... Remaining: %d\n", studentsRemaining);
                // printMutex.Unlock();
                Delay();
            }
            break; // Exit the loop after eviction is complete, else it will go crazy
        }
        Delay();
    }

    Exit();
}

int TStudent::GoToParty()
{

    Delay();

    // Prevent students from entering if eviction has started
    if (TLandlord::completed)
        return 1;

    // Print that the student is waiting to enter the room
    printMutex.Lock();
    std::cout << "     Student " << studentID << " waits to enter the room\n";
    printMutex.Unlock();

    enterSemaphore.Wait(); // Acquire entry semaphore to ensure controlled access to the room

    int studentsBeforeEntry = TStudent::currentStudentTracker; // DEbug: Capture room count before entry

    // Update student count and print that the student has entered
    printMutex.Lock();
    TStudent::currentStudentTracker++;
    std::cout << "     Student " << studentID << " enters the room (" << TStudent::currentStudentTracker << " students in the room)\n";
    printMutex.Unlock();

    enterSemaphore.Signal(); // Release the entry semaphore for the next student

    // debug: Print that the student is preparing to leave the room
    // printMutex.Lock();
    // printf("DEBUG: Student %d is preparing to leave the room\n", studentID);
    // printMutex.Unlock();

    // Print that the student is preparing to leave the room
    printMutex.Lock();
    std::cout << "     Student " << studentID << " waits to leave the room\n";
    printMutex.Unlock();

    exitSemaphore.Wait(); // Acquire exit semaphore to ensure controlled exit from the room

    // Debug: Capture room count before exit
    int studentsBeforeExit = TStudent::currentStudentTracker;

    // Update student count and print that the student has left
    printMutex.Lock();
    TStudent::currentStudentTracker--;
    std::cout << "     Student " << studentID << " leaves the room (" << TStudent::currentStudentTracker << " students in the room)\n";
    printMutex.Unlock();

    exitSemaphore.Signal(); // Release the exit semaphore for the next student

    return 1;
}

int TLandlord::CheckRoom()
{

    // Acquire entry and exit semaphores to prevent students from entering or leaving
    enterSemaphore.Wait();
    exitSemaphore.Wait();

    // Need for debugging
    int studentsBeforeCheck = TStudent::currentStudentTracker;
    int studentsAfterCheck = studentsBeforeCheck;
    bool evictionStarted = false;
    int checkIteration = counter + 1;

    // Print that the landlord is checking the room
    printMutex.Lock();
    std::cout << "The landlord checks the room the " << counter + 1 << "th time\n";
    printMutex.Unlock();

    // Case 1: No students in the room
    if (TStudent::currentStudentTracker == 0)
    {
        printMutex.Lock();
        std::cout << "The landlord finds the room has no students and leaves\n";
        printMutex.Unlock();
    }
    // Case 2: Too many students in the room (party detected)
    else if (TStudent::currentStudentTracker >= maxStudentsAllowed)
    {

        printMutex.Lock();
        std::cout << "The landlord finds " << TStudent::currentStudentTracker << " students in the room and starts breaking up the party\n";
        std::cout << "The landlord finishes checking and forces everyone to leave\n";
        printMutex.Unlock();

        evictionStarted = true; // Mark eviction process

        // Wait for all students to leave before allowing new entries
        while (TStudent::currentStudentTracker != 0)
        {
            Delay();
        }

        // Update student count after eviction
        studentsAfterCheck = TStudent::currentStudentTracker;

        printMutex.Lock();
        std::cout << "The landlord leaves the room after clearing it of party-goers\n";
        printMutex.Unlock();
    }
    // Case 3: Some students are in the room, but under the party limit
    else
    {
        printMutex.Lock();
        std::cout << "The landlord finds there are only " << TStudent::currentStudentTracker << " students in the room and leaves\n";
        printMutex.Unlock();
    }

    // Debugging: Print if eviction happened
    //  if (evictionStarted) {
    //      printMutex.Lock();
    //      printf("DEBUG: Eviction process was triggered at check iteration %d\n", checkIteration);
    //      printMutex.Unlock();
    //  }

    // Release the semaphores, allowing students to enter and leave again
    enterSemaphore.Signal();
    exitSemaphore.Signal();

    return 1;
}
