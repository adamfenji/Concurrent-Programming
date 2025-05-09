// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 04/04/2025
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.h
// PROGRAM PURPOSE :
// Header defining thread interfaces and structures
// -----------------------------------------------------------

#include "ThreadClass.h"
#include "santa-monitor.h"

class TSanta : public Thread
{
private:
	void ThreadFunc();	// Main thread logic for Santa
	
	MonitorAF *monitor; // Shared monitor with threads
	
	int numReindeer;	// Total number of reindeer
	
	int numDeliveries;	// Current delivery count
	
	int deliveryGoal;	// Target number of deliveries
public:
	TSanta(MonitorAF *monitor, int reindeer, int deliveries); // Constructor
};

class TReindeer : public Thread
{
private:
	void ThreadFunc();	// Main thread logic for reindeer
	
	int count;			// count
	
	MonitorAF *monitor; // Shared monitor with threads
public:
	TReindeer(MonitorAF *monitor, int count); // Constructor
};

class TElf : public Thread
{
private:
	void ThreadFunc();	// Main thread logic for elf
	
	int count;			// count
	
	MonitorAF *monitor; // Shared monitor with threads
public:
	TElf(MonitorAF *monitor, int count); // Constructor
};