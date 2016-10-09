/***************************************************************************************
CSCI 480  - Assignment 4 - Fall 2016
Programmer: Jacob Muzikoski
Section:    0002
TA:         Mark White
Due Date:   October 10, 2016
Purpose:    This program acts as a microshell. The user can type in commands just like on a linux command line and get the same results.
   	    If the user wishes to pipe, instead of using "|" they must use "||"

****************************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

/***************************************************************

Function:	getUserInput

Use:		gets input from the user and returns it as a string variable called userInput

Arguments:	none

Returns:	String

***************************************************************/

string getUserInput()
{
	string userInput;

	cout << "480shell: ";
	getline(cin, userInput);

	return userInput;
}

int main()
{
	int status;
	int index = 0;
	int pipefd[2];
	int i = 0;
	int j = 0;
	char* argPointerOne[30];
	char* argPointerTwo[30];
	pid_t pidOne;
	pid_t pidTwo;
	string userCommandOne;
	string userCommandTwo;
	string commandOne[30];
	string commandTwo[30];
	istringstream inputStream;

	// Free up memory space
	for(int i = 0; i < 30; i++)
	{
		argPointerOne[i] = (char*) malloc (sizeof(char*));
		argPointerTwo[i] = (char*) malloc (sizeof(char*));
	}
	
	// Loop that only breaks when the user types in q or quit
	while(true)
	{
		// Call function that gets user command
		string userInput = getUserInput();
		
		if(userInput == "quit" || userInput == "q")
			break;

		int pipePos = userInput.find(" || ");

		// Two commands using ||
		if(pipePos != string::npos)
		{
			// Store userInput into command strings
			userCommandOne = userInput.substr(0, pipePos);
			userCommandTwo = userInput.substr(pipePos + 4);
			
			// Puts String into the inputStream object for the first command
			inputStream.clear();
			inputStream.str(userCommandOne);

			// Reads in every character from the input stream into a string array
			while(inputStream >> commandOne[i])
			{
				i++;
			}

			// While there are still strings to be read
			index = 0;
			while(commandOne[index] != "")
			{
				// Convert string to char array
				strcpy(argPointerOne[index], commandOne[index].c_str());
				index++;
			}
			argPointerOne[index] = '\0';

			// Put String into the inputStream object for the second command
			inputStream.clear();
			inputStream.str(userCommandTwo);

			// Read in every character from the input stream into a string array
			while(inputStream >> commandTwo[j])
			{
				j++;
			}
			
			// While there are still strings to be read
			index = 0;
			while(commandTwo[index] != "")
			{
				strcpy(argPointerTwo[index], commandTwo[index].c_str());
				index++;
			}
			argPointerTwo[index] = '\0';

			// Make a pipe call
			if(pipe(pipefd) == -1)
			{
				cerr << "Error with the pipe call" << endl;
				exit(-1);
			}

			// Create first fork
			pidOne = fork();

			if(pidOne < 0)
			{
				cerr << "The first fork call failed." << endl;
				exit(-1);
			}
			// Child One
			else if(pidOne == 0)
			{
				close(1);
				if(dup(pipefd[1]) == -1)
				{
					cerr << "Dup Error for the first child" << endl;
					exit(-1);
				}
				close(pipefd[0]);

				if(execvp(argPointerOne[0], argPointerOne) == -1)
				{
					cerr << "Execution Error for " << userCommandOne << endl;
					exit(127);
				}
			}
			// Child Two
			else
			{
				if((pidTwo = fork()) < 0)
				{
					cerr << "The second fork call failed." << endl;
					exit(-1);
				}
				else if(pidTwo == 0)
				{
					close(0);
					if(dup(pipefd[0]) == -1)
					{
						cerr << "Dup Error for the second child" << endl;
						exit(-1);
					}
					close(pipefd[1]);			

					if(execvp(argPointerTwo[0], argPointerTwo) == -1)
					{
						cerr << "Execution Error for " << userCommandTwo << endl;
						exit(127);
					}
				}
				else
				{
					close(pipefd[0]);
					close(pipefd[1]);
					
					waitpid(pidOne, &status, 0);
					waitpid(pidTwo, &status, 0);
				}
					
				userCommandOne.clear();
				userCommandTwo.clear();
			}
		}
		// One command
		else
		{
			// Put String into the inputStream object for command
			inputStream.clear();
			inputStream.str(userInput);

			// Reads in every character from the input stream into a string array
			while(inputStream >> commandOne[i])
			{
				i++;
			}

			// While there are still strings to be read
			index = 0;
			while(commandOne[index] != "")
			{
				strcpy(argPointerOne[index], commandOne[index].c_str());
				index++;
			}
			argPointerOne[index] = '\0';
			
			// Call fork
			pidOne = fork();

			if(pidOne < 0)
			{
				cerr << "Error in the fork for a single command" << endl;
				exit(-1);
			}
			else if(pidOne == 0)
			{
				if(execvp(argPointerOne[0], argPointerOne) == -1)
				{
					cerr << "Execution Error for " << userCommandOne << endl;
					exit(127);
				}
			}
			else
			{
				waitpid(pidOne, &status, 0);			
			}

			userCommandOne.clear();
			userCommandTwo.clear();
		}

		// Reset Counters
		i = 0;
		j = 0;
		index = 0;
		userInput.clear();
		
		// Clear strings
		for(int l = 0; l < 30; l++)
		{
			commandOne[l].clear();
			commandTwo[l].clear();
		}		
	}// End While Loop

	// Free up memory space
	for(int i = 0; i < 30; i++)
	{
		free (argPointerOne[i]);
		argPointerOne[i] = NULL;
		free (argPointerTwo[i]);
		argPointerTwo[i] = NULL;
	}

	return 0;
}
