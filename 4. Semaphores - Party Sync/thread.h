// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 03/19/2025
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread.h
// PROGRAM PURPOSE :
// This header file defines classes and synchronization for managing a simulated environment with students and a landlord sharing a common party room.
// -----------------------------------------------------------

#include "ThreadClass.h"

// Shared synchronization
extern Mutex printMutex;						// Ensures orderly printing to avoid race conditions
extern Semaphore exitSemaphore, enterSemaphore; // Control entry & exit to the room

class TStudent : public Thread
{
public:
	TStudent(int i);					  // Constructor initializes student ID
	static int currentStudentTracker;	  // Tracks number of students currently in the room.
	static int totalActiveStudentProcess; // Tracks total active student processes.

private:
	void ThreadFunc(); // Defines student behavior throughout execution
	int GoToParty();   // Handles student entry, stay, and exit from the room
	int studentID;
};

class TLandlord : public Thread
{
public:
	TLandlord(int m, int n); // Constructor initializes landlord's parameters
	static int completed;	 // Declare static member

private:
	void ThreadFunc();		// Defines landlord behavior, including room inspections
	int CheckRoom();		// Inspects room conditions and evicts students if needed
	int maxLandlordCheck;	// Number of times the landlord will check the room.
	int maxStudentsAllowed; // Maximum allowable students in the room before it's considered a party
	int counter;			// Counter for the number of completed room checks.
};