// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 04/04/2025
// PROGRAM ASSIGNMENT #5
// FILE NAME : santa-monitor.h
// PROGRAM PURPOSE :
// Header declaring Santa's monitor synchronization methods
// -----------------------------------------------------------

#include "ThreadClass.h"

class MonitorAF : public Monitor
{
private:
        int *elfArray;                        // Stores the IDs of elves asking questions

        int elfQuestionsAnswered;             // Flag to indicate if elves' questions were answered
        
        int lastReindeer;                     // Flag to identify the last reindeer to return
        
        int numReindeer;                      // Total number of reindeer
        
        int deliveryGoal;                     // Number of total deliveries required
        
        int isDeliveryDone;                   // Flag to check if delivery is done
        
        int sleighStatus;                     // Flag to track sleigh readiness
        
        int reindeerQueued;                   // Number of reindeer queued for sleigh
        
        int reindeerAttach;                   // Number of reindeer attached to sleigh
        
        int NumDelivery;                      // Counter for completed deliveries
        
        int countReindeer;                    // Counter of reindeer currently returned
        
        int countElf;                         // Counter of elves currently asking questions
        
        int isSantaAsleep;                    // Flag to track if Santa is asleep
        
        int elfGroupReady;                    // Flag for when 3 elves are ready
        
        Condition *condition_lockElves;       // Blocks elves until Santa is ready
        
        Condition *condition_ready;           // Signals when sleigh is ready
        
        Condition *condition_freeReindeer;    // Signals when reindeer are free
        
        Condition *condition_reindeerPending; // Blocks until all reindeer are queued
        
        Condition *condition_reindeerBack;    // Blocks until all reindeer are back
        
        Condition *condition_freeElves;       // Signals when elves can ask again
        
        Condition *condition_lockReindeer;    // Blocks reindeer until Santa is ready
        
        Condition *condition_sleighReady;     // Signals sleigh departure
        
        Condition *condition_wakeSanta;       // Wakes up Santa
        
        Condition *condition_answerSanta;     // Signals when Santa answers elves
public:
        MonitorAF(int reindeer, int deliveries); // Constructor to initialize monitor
        
        void FlyOff();                           // Called when sleigh takes off
        
        void Sleep();                            // Handles Santa's sleep/wake logic
        
        void AnswerQuestion();                   // Santa answers elf questions
        
        int ReindeerBack(int rein);              // Reindeer return handle
        
        void AskQuestion(int count);             // Elf asks Santa a question

        int ElfGroupHelped();          // Checks if elves were helped after last delivery
        
        void ReadyForDeparture();      // Reindeer prepare to attach to the sleigh
        
        int CheckReindeerReturn();     // Returns flag if last reindeer is back
        
        int HasSleighDeparted();       // Checks if sleigh has departed
        
        void AttachReindeerToSleigh(); // Prepares sleigh for flight
        
        void ReindeerVacation();       // Reindeer wait after delivery
        
        void SyncWithElves();          // Sync point while reindeer wait for elves
};