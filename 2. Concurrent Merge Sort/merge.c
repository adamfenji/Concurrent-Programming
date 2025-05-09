// -----------------------------------------------------------
// NAME : Adam Fenjiro User ID: afenjiro
// DUE DATE : 02/03/2025
// PROGRAM ASSIGNMENT #2
// FILE NAME : merge.c
// PROGRAM PURPOSE :
// Sorts assigned subarrays using recursive merge sort and writes sorted results back to shared memory.
// -----------------------------------------------------------

//Imports
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/ipc.h>

//Swap Function: helper function that swaps
void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

//Binary Search Function: Finds the correct position to insert a value in a sorted array
int binarySearch(int value, int left, int right, int *temp)
{
	int mid = (right + left) / 2;
	if (right <= left) return (value < temp[left]) ? left : left + 1; //return position
	if (value < temp[mid]) return binarySearch(value, left, mid, temp); //Recursively, check the left half
	else return binarySearch(value, mid + 1, right, temp); //Recursively, check the right half
}

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

//Process Creation: helper that forks a new process and executes another instance of this program
void createProcess(char *program, int left, int right, char *shmID, char *zID, char *fullSize)
{
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("Fork error");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) //Child process
	{ 
		//Convert int to strings for passing as arguments
		char leftString[10], rightString[10];
		sprintf(leftString, "%d", left);
		sprintf(rightString, "%d", right);
		char *argv[] = {program, leftString, rightString, shmID, zID, fullSize, NULL};

		//Execute here
		execvp(program, argv);
		perror("execvp error");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{
	int left = atoi(argv[1]);
	int right = atoi(argv[2]);
	char buffer[500];

	//Attach to shared memory arrays, using helper here
	int *shmArr = attachSharedMemory(atoi(argv[3]));
	int *zArr = attachSharedMemory(atoi(argv[4]));

	//If this is the root process then print the entire array
	if ((right - left + 1) == atoi(argv[5]))
	{
		sprintf(buffer, "   ### M-PROC(%d): entering with a[%d..%d]\n", getpid(), left, right);
		for (int i = left; i <= right; i++) sprintf(buffer + strlen(buffer), "   %d", shmArr[i]);
		sprintf(buffer + strlen(buffer), "\n");
		write(1, buffer, strlen(buffer));
	}
	else //Otherwise print which process created this one, aka the root
	{
		sprintf(buffer, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d]\n", getpid(), getppid(), left, right);
		for (int i = left; i <= right; i++) sprintf(buffer + strlen(buffer), "   %d", shmArr[i]);
		sprintf(buffer + strlen(buffer), "\n");
		write(1, buffer, strlen(buffer));
	}

	//Base Case: If we only have two elements then just sort them directly
	if ((right - left) == 1)
	{
		if (shmArr[right] < shmArr[left])
		{
			swap(&shmArr[left], &shmArr[right]); //Swap, using helper here
			sprintf(buffer, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d] -- sorted\n", getpid(), getppid(), left, right);
			for (int i = left; i <= right; i++) sprintf(buffer + strlen(buffer), "   %d", shmArr[i]);
			sprintf(buffer + strlen(buffer), "\n");
			write(1, buffer, strlen(buffer));
		}
		exit(0);
	}

	//Recursively split and sort each half, using the create process helper here
	int mid = (right + left) / 2;
	createProcess("./merge", left, mid, argv[3], argv[4], argv[5]);
	createProcess("./merge", mid + 1, right, argv[3], argv[4], argv[5]);

	//waiting here on childs
	wait(NULL);
	wait(NULL);

	sprintf(buffer, "   ### M-PROC:(%d) created by M-PROC(%d): both array sections sorted. start merging\n", getpid(), getppid());
	write(1, buffer, strlen(buffer));

	//Create a process for each element to place them in the correct position
	for (int i = left; i <= right; i++)
	{
		pid_t pid = fork();
		if (pid < 0) exit(-1);
		else if (pid == 0)
		{
			sprintf(buffer, "      $$$ B-PROC(%d) created by M-PROC(%d) for a[%d] = %d is created\n", getpid(), getppid(), i, shmArr[i]);
			write(1, buffer, strlen(buffer));

			//Find correct position using binary search helper function
			int k = (i < mid + 1) ? binarySearch(shmArr[i], mid + 1, right, shmArr) : binarySearch(shmArr[i], left, mid, shmArr);
			int index = (i < mid + 1) ? k + i - left - mid - 1 + left : i - mid - 1 + k;
			zArr[index] = shmArr[i];

			sprintf(buffer, "      $$$ B-PROC(%d): a[%d] = %d is written to temp[%d]\n", getpid(), i, shmArr[i], index);
			write(1, buffer, strlen(buffer));

			sprintf(buffer, "      $$$ B-PROC(%d) created by M-PROC(%d) for a[%d] = %d is terminated\n", getpid(), getppid(), i, shmArr[i]);
			write(1, buffer, strlen(buffer));

			detachSharedMemory(zArr);
			exit(0);
		}
	}

	//Wait for all child processes here
	for (int i = left; i <= right; i++) wait(NULL);

	//Copy the sorted values from temp storage back to shared memory
	for (int i = 0; i <= right - left + 1; i++) shmArr[i + left] = zArr[i + left];

	if ((right - left + 1) == atoi(argv[5])) sprintf(buffer, "   ### M-PROC(%d) created by M-PROC(%d): merge sort a[%d...%d] completed:\n", getpid(), getppid(), left, right);

	for (int i = left; i <= right; i++) sprintf(buffer + strlen(buffer), "   %d", shmArr[i]);
	sprintf(buffer + strlen(buffer), "\n");
	write(1, buffer, strlen(buffer));

	//Detach shared memory before exiting
	detachSharedMemory(shmArr);
	return 0;
}
