// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 04/04/2025
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
// Main file to initialize and manage Santa's threads
// -----------------------------------------------------------

#include "thread.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
	
	//Help message if not enough arguments
	if (argc != 4) {
        std::cout << "Usage: ./prog5 <number_of_elves> <number_of_reindeer> <number_of_deliveries>" << std::endl;
		return -1;
    }

	//Get the necessary variables:
	//numElves, numReindeer, numDeliveries
	int numElves = atoi(argv[1]);
    int numReindeer = atoi(argv[2]);
    int numDeliveries = atoi(argv[3]);
	
	//Initialize the santaMonitor and the threads:
	//Santa thread, Elf threads, Reindeer threads
	MonitorAF *santaMonitor = new MonitorAF(numReindeer, numDeliveries);
	TSanta *santaThread = new TSanta(santaMonitor, numReindeer, numDeliveries);
	TReindeer *reindeerThreads[numReindeer];
	TElf *elveThreads[numElves];
	
	santaThread->Begin(); //Start the Santa Thread

	//Loop to initialize and start reindeer and elve threads
	int maxThreads = max(numReindeer, numElves);
	for(int i = 0; i < maxThreads; i++) {
		if(i < numReindeer) {
			reindeerThreads[i] = new TReindeer(santaMonitor, i);
			reindeerThreads[i]->Begin();
		}
		if(i < numElves) {
			elveThreads[i] = new TElf(santaMonitor, i);
			elveThreads[i]->Begin();
		}
	}

	//Join both the elve threads, reindeer threads, then the santa thread
	for(int i = 0; i < numReindeer; i++) reindeerThreads[i]->Join();
	for(int i = 0; i < numElves; i++) elveThreads[i]->Join();
	santaThread->Join();

	//Print end message after execution
	std::cout << "Santa retires after " << numDeliveries << " deliveries" << std::endl;

	return 0;
}	