// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 02/03/2025
// PROGRAM ASSIGNMENT #2
// FILE NAME : main.c
// PROGRAM PURPOSE :
// Creates shared memory, spawns child processes to execute merge sort recursively, and manages process synchronization.
// -----------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/ipc.h>

//Process Creation: helper that forks a new process and executes another instance of this program
void createProcess(char *program, int left, int right, int shmid, int zID, int inputSize) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Child process
        
		// Convert integer values to strings for passing them as arguments
		char leftBound[10], rightBound[10], sharedMemID[10], tempMemID[10], arraySize[10];

        sprintf(leftBound, "%d", left);
        sprintf(rightBound, "%d", right);
        sprintf(sharedMemID, "%d", shmid);
        sprintf(tempMemID, "%d", zID);
        sprintf(arraySize, "%d", inputSize);

        char *argv[] = {program, leftBound, rightBound, sharedMemID, tempMemID, arraySize, NULL};

        execvp(program, argv);
        perror("execvp error");
        exit(EXIT_FAILURE);
    }
}

/** TODO: Find where to replace this, it saved me a lot of trouble in merge.c
//Attach Shared Memory: helper thant attaches to a shared memory segment and returns a pointer
int *attachSharedMemory(int id)
{
	int *pointer = (int *)shmat(id, NULL, 0);
	if (pointer == (void *)-1)
	{
		perror("shmat failed");
		exit(EXIT_FAILURE);
	}
	return pointer;
}

//Detach Shared Memory: helper that detaches from a shared memory segment
void detachSharedMemory(int *pointer)
{
	if (shmdt(pointer) == -1)
	{
		perror("shmdt failed");
		exit(EXIT_FAILURE);
	}
}
*/

int main(void) {

	char buffer[500];
    int inputSize = 0; //store the ints here
    scanf("%d", &inputSize);

	//Shared memeory variables needed
    key_t key = ftok("./", 'x');
	int shmid = shmget(key, inputSize * sizeof(int), IPC_CREAT | 0666);
	int *shm = (int *)shmat(shmid, NULL, 0);
	key_t zKey = ftok("./", 'z');
	int zID = shmget(zKey, inputSize * sizeof(int), IPC_CREAT | 0666);

	//Printing shared memory details
    sprintf(buffer, "*** MAIN: shared memory key = %d\n", key);
    write(1, buffer, strlen(buffer));
    
    sprintf(buffer, "*** MAIN: shared memory created\n");
    write(1, buffer, strlen(buffer));
    
    sprintf(buffer, "*** MAIN: shared memory attached and is ready to use\n");
    write(1, buffer, strlen(buffer));

	//Declare the input array and read values into it
	int inputArray[inputSize];
    sprintf(buffer, "Input array for mergesort has %d elements:\n", inputSize);
    for (int i = 0; i <= inputSize -1; i++) {
        scanf("%d", &inputArray[i]); //read
        shm[i] = inputArray[i]; //store
        sprintf(buffer + strlen(buffer), "   %d", shm[i]); //append
    }
    sprintf(buffer + strlen(buffer), "\n");
    write(1, buffer, strlen(buffer));

    sprintf(buffer, "*** MAIN: about to spawn the merge sort process\n");
    write(1, buffer, strlen(buffer));

	//using the hlper to create a child process
    createProcess("./merge", 0, inputSize - 1, shmid, zID, inputSize);

    wait(NULL); //waiting for the merge here

	//print sorted array
    sprintf(buffer, "*** MAIN: merged array:\n");
    for (int i = 0; i <= inputSize -1; i++) sprintf(buffer + strlen(buffer), "   %d", shm[i]);
    sprintf(buffer + strlen(buffer), "\n");
    write(1, buffer, strlen(buffer));

	//detach mem and exit, after this line
    shmdt(shm);
    sprintf(buffer, "*** MAIN: shared memory successfully detached\n");
    write(1, buffer, strlen(buffer));
    
	shmctl(shmid, IPC_RMID, NULL);
    sprintf(buffer, "*** MAIN: shared memory successfully removed\n");
    write(1, buffer, strlen(buffer));

    shmctl(zID, IPC_RMID, NULL);
    sprintf(buffer, "*** MAIN: exits\n");
    write(1, buffer, strlen(buffer));

    return 0;
}