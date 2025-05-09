// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 03/19/2025
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
// This file contains the main function for a multithreaded simulation where a landlord monitors and manages student parties in an apartment's social room, employing threads and synchronization mechanisms.
// -----------------------------------------------------------

#include "thread.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{

	// Check if the correct number of arguments is provided
	if (argc != 4)
	{
		printf("ERROR: ./prog4 m n k\n");
		return -1;
	}

	// Convert command-line arguments to integers
	int m; // Number of times the landlord checks the room
	m = atoi(argv[1]);
	int n; // Maximum allowed students before breaking up a party
	n = atoi(argv[2]);
	int k; // Total number of students
	k = atoi(argv[3]);

	TLandlord *landlordT = new TLandlord(m, n); // Create landlord thread

	// Store student thread
	std::vector<TStudent *> studentsT;
	studentsT.reserve(k);

	// Start the landlord thread
	landlordT->Begin();

	// Create and start student threads
	for (int i = 0; i < k; i++)
	{
		studentsT.push_back(new TStudent(i));
		studentsT[i]->Begin();
	}

	// Wait for all student threads to finish execution
	for (int i = 0; i < k; i++)
	{
		studentsT[i]->Join();
	}

	// Wait for the landlord thread to finish execution
	landlordT->Join();

	// Print the final message
	cout << "Having evicted all the students, the landlord sells the building and retires\n";

	return 1;
}