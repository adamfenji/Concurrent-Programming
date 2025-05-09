// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 04/04/2025
// PROGRAM ASSIGNMENT #5
// FILE NAME : santa-monitor.cpp
// PROGRAM PURPOSE :
// Synchronization logic for Santa's workshop monitor
// -----------------------------------------------------------

#include "thread.h" 
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

// Constructor: Initialize monitor variables and conditions
MonitorAF::MonitorAF(int reindeer, int deliveries):Monitor("Monitor", HOARE) {
	// Initialize conditions for various Santa's operations
	condition_freeReindeer = new Condition("Release the reindeer");
	condition_answerSanta = new Condition("Santa is answering questions");
	condition_sleighReady = new Condition("Sleigh is ready to fly");
	condition_lockElves = new Condition("Elves are busy");
	condition_ready = new Condition("Ready for takeoff");
	condition_freeElves = new Condition("Release the elves");
	condition_wakeSanta = new Condition("Santa needs to wake up");
	condition_reindeerBack = new Condition("All reindeer are back");
	condition_reindeerPending = new Condition("Waiting to attach reindeer");
	condition_lockReindeer = new Condition("Reindeer are busy");
	// Initialize variables to track number of reindeer, deliveries, and elf-related operations
	numReindeer = reindeer;
	deliveryGoal = deliveries;
	countElf = elfGroupReady = elfQuestionsAnswered = isDeliveryDone = isSantaAsleep = reindeerQueued = lastReindeer = NumDelivery = countReindeer = reindeerAttach = sleighStatus = 0;
	elfArray = (int*)malloc(3 * sizeof(int));// Allocate memory for tracking elves [3]
}

// Returns flag if last reindeer is back
int MonitorAF::CheckReindeerReturn() {
    MonitorBegin();
    MonitorEnd();
    return lastReindeer; // Return the status of the last reindeer
}

// Checks if sleigh has departed
int MonitorAF::HasSleighDeparted() {
	
	MonitorBegin();
	int done = (NumDelivery >= deliveryGoal) ? 1 : 0;  // Check if sleigh has departed based on delivery goal
	MonitorEnd();
	return done; // Return departure status

}

// Elves ask Santa questions based on their group
void MonitorAF::AskQuestion(int count) {
	MonitorBegin();

	// printf("Debug: Monitor started. Elf count = %d, elfGroupReady status = %d, Delivery status = %d\n", countElf, elfGroupReady, delivery);
	
	if((elfGroupReady == 1) && (NumDelivery < deliveryGoal)) condition_freeElves->Wait(); // Wait if not ready for new questions		
	countElf++;

	if(NumDelivery < deliveryGoal) {  // Check if delivery goal is not met
		std::cout << "         Elf " << count + 1 << " has a question" << std::endl;
		if(countElf == 3) { // If three elves have questions
			elfGroupReady = 1; // Set group ready status
			elfArray[countElf-1] = count; // Record elf's index
			if(isSantaAsleep == 1) { // If Santa is asleep
				std::cout << "         Elves " << elfArray[0] + 1 << ", " << elfArray[1] + 1 << ", and " << elfArray[2] + 1 << " approach Santa with their question" << std::endl;
				condition_wakeSanta->Signal(); // Wake up Santa
			}
			else {
				condition_lockElves->Wait();	// Elves wait if Santa is not asleep
				// printf("Debug: Santa is not asleep. Elves must wait until he is ready.\n");
				if(NumDelivery < deliveryGoal) std::cout << "         Elves " << elfArray[0] + 1 << ", " << elfArray[1] + 1 << ", and " << elfArray[2] + 1 << " approach Santa with their question" << std::endl;
			}
		}
		else {
			elfQuestionsAnswered = 0;	// Reset questions answered status
			elfArray[countElf-1] = count; // Record elf's index
			// printf("Debug: Elf %d is waiting for others. Not enough elves to form a elfGroupReady.\n", count + 1);
			condition_freeElves->Signal(); // Signal next elf to ask question
		}
		if((elfQuestionsAnswered == 0) && (NumDelivery != deliveryGoal)) condition_answerSanta->Wait();
		countElf--;
		if(countElf == 0) { // If no elves left
			std::cout << "         Elf " << elfArray[0] + 1 << " goes back to work" << std::endl;
			std::cout << "         Elf " << elfArray[1] + 1 << " goes back to work" << std::endl;
			std::cout << "         Elf " << elfArray[2] + 1 << " goes back to work" << std::endl;
			elfGroupReady = 0; // Reset group readiness
			condition_freeElves->Signal(); // Signal that elves are free
			// printf("Debug: All elves have returned to work.\n");
		}
	}
	MonitorEnd(); // Unlock monitor
}

// Process reindeer returning and potentially waking Santa
int MonitorAF::ReindeerBack(int rein) {
	MonitorBegin(); // Lock monitor
	
	int last = 0; // Flag to indicate if this is the last reindeer
    countReindeer++;
	std::cout << "    Reindeer " << rein + 1 << " returns" << std::endl;

	if (countReindeer == numReindeer) { // If all reindeer are back
		last = 1; // Set last flag
        lastReindeer = 1; // Indicate last reindeer has returned
		std::cout << "    The last reindeer " << rein + 1 << " goes to wake up Santa" << std::endl;

	 	if(isSantaAsleep == 1) condition_wakeSanta->Signal(); // If Santa is asleep, wake him
		else condition_lockReindeer->Wait(); // Otherwise, wait for reindeer to be free
		condition_reindeerBack->Signal();		// Signal that reindeer is back
	}	
		
	MonitorEnd(); // Unlock monitor
	return last; // Return last flag
}

// Called when sleigh takes off
void MonitorAF::FlyOff() {
	MonitorBegin(); // Lock monitor
	
	reindeerAttach++;
	if(reindeerAttach == numReindeer) { // Check if all reindeer are harnessed
		NumDelivery++;
		std::cout << "With all reindeer harnessed in, the crew sets off (" << NumDelivery << ")!" << std::endl;
		condition_sleighReady->Signal(); // Signal that sleigh is ready	
	}
	else {
		condition_sleighReady->Wait(); // Wait if not all reindeer are harnessed
		condition_sleighReady->Signal(); // Continue signaling readiness
		for(int i = 0; i < numReindeer; i++) {
			// printf("Debug: current count = %d\n", i);
        }
	}
	reindeerAttach--;

	if(reindeerAttach == 0) {
		sleighStatus = 0; // Reset sleigh status for next use
		isDeliveryDone = 1; // Mark delivery as completed
		condition_freeReindeer->Signal(); // Signal that reindeer are free to go

		// Initialize unique set and flags array
        int uniqueSet2[numReindeer];
        for(int i = 0; i < numReindeer; i++) uniqueSet2[i] = i + 1;
        
        srand(time(NULL)); // Shuffle using a simple random swap algorithm
        for(int i = 0; i < numReindeer; i++) {
            int r2 = rand() % numReindeer;
            int temp2 = uniqueSet2[i];
            uniqueSet2[i] = uniqueSet2[r2];
            uniqueSet2[r2] = temp2;
        }

        int reindeerLoopCount2 = 0;
        while(reindeerLoopCount2 < numReindeer){
            std::cout << "    Reindeer " << uniqueSet2[reindeerLoopCount2] << " goes on vacation" << std::endl;
			reindeerLoopCount2++;
        }
	}
	else if(isDeliveryDone == 0) {
		condition_freeReindeer->Wait(); // Wait if not all deliveries are done
		condition_freeReindeer->Signal();	// Signal readiness for more reindeer 
		// printf("Debug: Not all deliveries finished, awaiting more reindeer\n");
	}	
	
	MonitorEnd(); //unlock
}

// Santa answers elf questions
void MonitorAF::AnswerQuestion() {
	MonitorBegin(); //lock
	std::cout << "Santa answers questions by elves " << elfArray[0] + 1 << ", " << elfArray[1] + 1 << ", and " << elfArray[2] + 1 << std::endl;

	elfQuestionsAnswered = 1; // Mark questions as answered
	for (int i = 0; i < 3; i++) condition_answerSanta->Signal(); // Signal that questions have been answered
	MonitorEnd(); //unlock
}

// Sync reindeer with elves before departure
void MonitorAF::SyncWithElves() {

	MonitorBegin(); // Lock monitor

	for (int i = 0; i < numReindeer; i++) {
		// if (i < countReindeer) {
		// 	printf("Debugging: Reindeer %d status: Back\n", i + 1);
		// } else {
		// 	printf("Debugging: Reindeer %d status: Not back yet\n", i + 1);
		// }
	}

	if(countReindeer != numReindeer) condition_reindeerBack->Wait(); // Wait for all reindeer to return
	condition_reindeerBack->Signal();	 // Signal that reindeer are back

	MonitorEnd(); // Unlock monitor
}  

// Prepare for departure once all reindeer are ready
void MonitorAF::ReadyForDeparture() {
	MonitorBegin(); // Lock monitor

	reindeerQueued++;
	if(reindeerQueued == numReindeer) {
		condition_reindeerPending->Signal(); // If all reindeer are ready

		// Initialize unique set and flags array
        int uniqueSet[numReindeer];
        for(int i = 0; i < numReindeer; i++) uniqueSet[i] = i + 1;
        
        srand(time(NULL)); // Shuffle using a simple random swap algorithm
        for(int i = 0; i < numReindeer; i++) {
            int r = rand() % numReindeer;
            int temp = uniqueSet[i];
            uniqueSet[i] = uniqueSet[r];
            uniqueSet[r] = temp;
        }

        int reindeerLoopCount = 0;
        while(reindeerLoopCount < numReindeer){
            std::cout << "    Reindeer " << uniqueSet[reindeerLoopCount] << " is attached to the sleigh" << std::endl;
			reindeerLoopCount++;
        }
	}		
	else {
		condition_reindeerPending->Wait();
		condition_reindeerPending->Signal();	
		for(int i = 0; i < numReindeer; i++) {
			// printf("Debug: Waiting for more reindeer, count = %d\n", i);
        }
	}
	
	if(sleighStatus == 0) {
		condition_ready->Wait(); // Wait for more reindeer if not ready
		condition_ready->Signal(); // Signal pending reindeer
		// printf("Debug: Sleigh is not ready, waiting and signaling\n");			
	}	
	countReindeer = 0; // Reset reindeer count
	lastReindeer = 0; // Reset last reindeer flag
	reindeerQueued--; // Decrement queue count

	MonitorEnd(); // Unlock monitor
}

// Santa sleeps or wakes up based on the state of reindeer and elves
void MonitorAF::Sleep() {
	MonitorBegin(); // Lock monitor
	std::cout << "Santa goes back to sleep" << std::endl;
	if (countReindeer == numReindeer || countElf == 3) { // Check if conditions to wake Santa are met
		std::cout << "Santa wakes up" << std::endl;
        isSantaAsleep = 0; // Update Santa's sleep status
        if (countReindeer == numReindeer) condition_lockReindeer->Signal(); // Signal reindeer if all are present
        if (countElf == 3) condition_lockElves->Signal(); // Signal elves if all are ready
    }
	else {
		std::cout << "Santa wakes up" << std::endl; 
		isSantaAsleep = 1; // Update Santa's sleep status
		condition_wakeSanta->Wait(); // Santa waits to be woken up
		isSantaAsleep = 0; // Reset Santa's sleep status
	}		
	MonitorEnd(); // Unlock monitor
}

// Prepares sleigh for flight
void MonitorAF::AttachReindeerToSleigh() {
	MonitorBegin(); //lock
	
	std::cout << "Santa sees all the reindeer have returned and prepares the sleigh" << std::endl;
	isDeliveryDone = 0;	 // Reset delivery status
	sleighStatus = 1; // Update sleigh status to ready

	for (int i = 1; i <= numReindeer; i++) {
        // printf("Debugging: Checking reindeer %d status before signaling ready...\n", i);
    }

	condition_ready->Signal();	 // Signal that sleigh is ready

	MonitorEnd(); //unlock
}

// Checks if elves were helped after last delivery
int MonitorAF::ElfGroupHelped() {
    MonitorBegin();
    bool isDone = NumDelivery >= deliveryGoal; // Check if deliveries are complete
    if (isDone) {
        condition_freeElves->Signal(); // Signal that elves are free
        for (int i = 0; i < 3; i++) condition_answerSanta->Signal(); // Signal that questions have been answered
        condition_lockElves->Signal(); // Signal that elves can lock up
    }
    MonitorEnd();
    return isDone ? 1 : 0; // Return whether the elf group was helped
}

// Reindeer wait after delivery
void MonitorAF::ReindeerVacation() {
	MonitorBegin();

	for (int i = 0; i < 5; i++) {
        // printf("Debug: Iteration %d, Finished Delivery Status: %d\n", i, isDeliveryDone);
    }

	if(isDeliveryDone == 0) {
		condition_freeReindeer->Wait(); // Wait if delivery is not done
		condition_freeReindeer->Signal();		 // Signal that reindeer can go on vacation
	}

	for (int i = 0; i < 5; i++) {
        // printf("Debug: Post-Wait Iteration %d, Finished Delivery Status: %d\n", i, isDeliveryDone);
    }

	MonitorEnd();
}
