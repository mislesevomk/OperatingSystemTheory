/***************************************************************************************
CSCI 480 - Assignment 3 - Fall 2016
Programmer: Jacob Muzikoski
Section:    0002
TA:         Mark White
Due Date:   September 25, 2016
Purpose:    This program creates three processes. It calls three pipes and sends a message with number
	    attributes that get modified everytime it is passed to another process by pipe. 
	    
	    pipe[0] = read
	    pipe[1] = write

	    Parent -> PipeA -> Child -> PipeB -> Grandchild -> PipeC -> Parent

	    PipeA 
	    [0]reads from PipeC
            [1]writes to PipeB

            PipeB
            [0]reads from PipeA
            [1]writes to PipeC

            PipeC
            [0]reads from PipeB
            [1]writes to PipeA

****************************************************************************************/

#include <iostream>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

using namespace std;

/***************************************************************************************
Function:    PWork

Use:         Writes a message one byte at a time to the child process.
	     Enters a loop that then reads from pipeC, manipulates the Integers, and writes to pipeA

Arguments:   pipeC[0] and pipeA[1]

Returns:     Void
***************************************************************************************/
void PWork(int pipeRead, int pipeWrite)
{
	char InBuffer[50];
	char OutBuffer[50];
	char buf[50];
	int M = 1;
	int Counter = 0;
	int L;
	
	// Initial Message from PWork() exclusively
	cerr << "Message " << Counter << " from process Parent:       Value = " << M << endl;
	
	// Stores M into OutBuffer
	sprintf(OutBuffer, "%d", M);
	
	// Stores the length of OutBuffer into L
	L = strlen(OutBuffer);
	
	// Writes the length of OutBuffer to pipeWrite for the child process
	write(pipeWrite, &L, 1);

	// Writes contents of OutBuffer to pipeWrite for the child process one byte at a time
	for(int i = 0; i < L; i++)
	{
		write(pipeWrite, &OutBuffer[i], 1);
	}
	
	while(Counter < 8)
	{
		// Reads in the first byte from the grandchild process
		read(pipeRead, &InBuffer, 1);
		
		// Stores the first byte as the size of the buffer that will be read in
		L = InBuffer[0];

		for(int i = 0; i < L; i++)
		{
			// Reads message from pipe to buffer one byte at a time
			read(pipeRead, &InBuffer[i], 1);
		}
		
		// Adds a null terminator to end of buffer
		InBuffer[L] = '\0';

		// Converts M from buffer to an int for arthmetic
		M = atoi(InBuffer);
		M = 2 * M + 3;
		Counter++;
		
		// Formats the message
		sprintf(buf, "Message %d from process Parent:       Value = %d", Counter, M);
		cerr << buf << endl;

		// Stores M back into the OutBuffer
		sprintf(OutBuffer, "%d", M);
		
		// Store length of OutBuffer into L
		L = strlen(OutBuffer);
		
		// Writes contents of OutBuffer to the pipeWrite
		write(pipeWrite, &L, 1);
		for(int i = 0; i < L; i++)
		{
			write(pipeWrite, &OutBuffer[i], 1);
		}		
	}
	close(pipeRead);
	close(pipeWrite);
}

/************************************************************************************************
Function:    CWork

Use:         Enters a loop that reads from pipeA, manipulates the Integers, and writes to pipeC

Arguments:   pipeA[0] and pipeB[1]

Returns:     Void
*************************************************************************************************/
void CWork(int pipeRead, int pipeWrite)
{
	char InBuffer[50];
	char OutBuffer[50];
	char buf[50];
	int M = 1;
	int Counter = 0;
	int L;

	while(Counter < 8)
	{
		// Reads in the size of the buffer and stores it into InBuffer
		read(pipeRead, &InBuffer, 1);
		L = InBuffer[0];

		// Reads the rest of the contents from pipe into buffer one byte at a time
		for(int i = 0; i < L; i++)
		{
			read(pipeRead, &InBuffer[i], 1);
		}
		
		// Adds a null terminator to end of buffer
		InBuffer[L] = '\0';

		// Converts M from buffer to an int for arthmetic
		M = atoi(InBuffer);
		M = 2 * M + 3;
		Counter++;
		
		// Formats and displays the message
		sprintf(buf, "Message %d from process Child:        Value = %d", Counter, M);
		cerr << buf << endl;

		// Stores M into OutBuffer
		sprintf(OutBuffer, "%d", M);

		// Stores the length of OutBuffer into L
		L = strlen(OutBuffer);
		
		// Writes the length of the buffer to the next pipe
		write(pipeWrite, &L, 1);

		// Writes the message one byte at a time to the next pipe
		for(int i = 0; i < L; i++)
		{
			write(pipeWrite, &OutBuffer[i], 1);
		}
	}
	close(pipeRead);
	close(pipeWrite);
}

/************************************************************************************************
Function:    GWork

Use:         Enters a loop that reads from pipeB, manipulates the Integers, and writes to pipeA

Arguments:   pipeB[0] and pipeA[1]

Returns:     Void
*************************************************************************************************/
void GWork(int pipeRead, int pipeWrite)
{
	char InBuffer[50];
	char OutBuffer[50];
	char buf[50];
	int M = 1;
	int Counter = 0;
	int L;

	while(Counter < 8)
	{
		// Reads in the size of the buffer
		read(pipeRead, &InBuffer, 1);
		L = InBuffer[0];

		// Reads in one byte at a time from InBuffer to pipeRead
		for(int i = 0; i < L; i++)
		{
			read(pipeRead, &InBuffer[i], 1);
		}
		
		// Adds a null terminator to end of buffer
		InBuffer[L] = '\0';

		// Converts M from buffer to an int for arthmetic
		M = atoi(InBuffer);
		M = 2 * M + 3;
		Counter++;
		
		//Formats and displays the message
		sprintf(buf, "Message %d from process Grandchild:   Value = %d", Counter, M);
		cerr << buf << endl;

		// Stores M into OutBuffer
		sprintf(OutBuffer, "%d", M);

		// Stores the length of OutBuffer into L
		L = strlen(OutBuffer);
		
		// Writes the length of the buffer to the next pipe
		write(pipeWrite, &L, 1);

		// Writes the message one byte at a time to the next pipe
		for(int i = 0; i < L; i++)
		{
			write(pipeWrite, &OutBuffer[i], 1);
		}
	}
	close(pipeRead);
	close(pipeWrite);
}

int main()
{
	int pipeA[2];
	int pipeB[2];
	int pipeC[2];
	pid_t pid = 0;
	
	// Calls and Error Checks pipes
	if (pipe(pipeA) == -1)	
	{
		cerr << ("PipeA had an error") << endl;
		exit(-5);
	}

	if (pipe(pipeB) == -1)	
	{
		cerr << ("PipeB had an error") << endl;
		exit(-5);
	}

	if (pipe(pipeC) == -1)	
	{
		cerr << ("PipeC had an error") << endl;
		exit(-5);
	}

	// Creates a child process
	pid = fork();

	if(pid < 0)
	{
		cerr << ("The first fork had an error") << endl;
		exit(-5);
	}
	//Child Process
	else if(pid == 0)
	{
		pid = fork();
	
		if(pid < 0)
		{
			cerr << ("The second fork had an error") << endl;
			exit(-5);
		}
		//Grandchild Process
		else if(pid == 0)
		{
			close(pipeB[1]);
			close(pipeC[0]);
			GWork(pipeB[0], pipeC[1]);
			exit(0);
		}
		else
		{
			close(pipeA[1]);
			close(pipeB[0]);
			CWork(pipeA[0], pipeB[1]);
			
			// Waits for grandchild process to exit
			wait(0);
			exit(0);
		}
	}
	//Parent Process
	else
	{	
		close(pipeC[1]);
		close(pipeA[0]);
		PWork(pipeC[0], pipeA[1]);
		
		// Waits for child process to exit
		wait(0);
		exit(0);
	}	
	
	return 0;
}
