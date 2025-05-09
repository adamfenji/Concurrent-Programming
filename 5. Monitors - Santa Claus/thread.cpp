// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 04/04/2025
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
// Implements thread behaviors for Santa, elves, and reindeer
// -----------------------------------------------------------

#include "thread.h"
#include <iostream>
using namespace std;

// Constructor: Initialize Santa thread and print start message
TSanta::TSanta(MonitorAF *monitor, int reindeer, int deliveries): numReindeer(reindeer), numDeliveries(0), deliveryGoal(deliveries), monitor(monitor){std::cout << "Santa thread started" << std::endl;}
// Constructor: Initialize Elf thread and print start message
TElf::TElf(MonitorAF *monitor, int count): count(count), monitor(monitor){std::cout << "         Elf " << (count + 1) << " thread started" << std::endl;}
// Constructor: Initialize Reindeer thread and print start message
TReindeer::TReindeer(MonitorAF *monitor, int count): count(count), monitor(monitor){std::cout << "    Reindeer " << (count + 1) << " thread started" << std::endl;}

void TSanta::ThreadFunc() {
	Thread::ThreadFunc(); // Start thread context
	while(numDeliveries < deliveryGoal) { // Loop until delivery goal met
		monitor->Sleep(); // Santa sleeps or wakes depending on events
		if((monitor->CheckReindeerReturn()) == 1) { // All reindeer returned
			monitor->AttachReindeerToSleigh(); // Prepare sleigh
			numDeliveries++; // Increment delivery count
			Delay(350);
			monitor->ReindeerVacation(); // Signal reindeer vacation
		}
		else {
			Delay(250);
			monitor->AnswerQuestion(); // Handle elf questions
		}
	}	
	monitor->ElfGroupHelped(); // Allow waiting elves to exit
	Exit(); // End thread
}


void TElf::ThreadFunc() {
	Thread::ThreadFunc(); // Start thread context
	while(0 == 0) { // Infinite loop until delivery complete
		Delay(450);
		monitor->AskQuestion(count); // Ask Santa a question
		Delay(250);
		if((monitor->ElfGroupHelped()) == 1) Exit(); // Exit if Santa retires
	}	
	Exit(); // End thread
}


void TReindeer::ThreadFunc() {
	Thread::ThreadFunc(); // Start thread context
	while(0 == 0) {
		Delay(650);
		if((monitor->ReindeerBack(count)) != 1) monitor->SyncWithElves(); // Sync if not last
		monitor->ReadyForDeparture(); // Wait to attach to sleigh
		monitor->FlyOff(); // Participate in delivery
		Delay(150);
		if((monitor->HasSleighDeparted()) == 1) Exit(); // Exit if all done
	}
	Exit(); // End thread
}