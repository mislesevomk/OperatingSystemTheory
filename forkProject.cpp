/****************************************************************************************
CSCI 480  - Assignment 2 - Fall 2016
Programmer: Jacob Muzikoski
Section:    0002
TA:         Mark White
Due Date:   September 10, 2016
Purpose:    This program calls other processes by using fork()
	    After being called it prints out statements of which process it is and what the parent process is
	    It shows the user which processors are produced by calling fork()
****************************************************************************************/

#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>


using namespace std;

int main()
{
	pid_t pid = 0;

	//Print a message indentifying the process as the original thread and providing its PID and it's parents's PID
    	cout << "I am the original process. My PID is " << getpid() << " and my parent's PID is " << getppid() << endl;

	// Call fork()
	cout << "Now we have the first fork." << endl;
	pid = fork();

	// If fork() fails, print an error message
	if (pid < 0)
	{
		cerr << "The first fork failed." << endl;
		exit(-1);
	}
	//Child Process
	else if(pid == 0)
	{	
		cout << "I am the child. My PID is " << getpid() << " and my parent's PID is " << getppid() << endl;			
		pid = fork();
				
		if(pid < 0)
		{
			cerr << "The second fork failed." << endl;
			exit(-1);
		}
		//Grandchild Process
		else if(pid == 0)
		{
			cout << "I am the granchild. My PID is " << getpid() << " and my parent's PID is " << getppid() << endl;
			cout << "I am the grandchild, about to exit." << endl;
			exit(0);
		}
		//Child Process
		else
		{
			cout << "Now we have the second fork." << endl;
			cout << "I am still the child. My PID is " << getpid() << " and my parent's PID is " << getppid() << endl;
			wait(0);
			cout << "I am the child, about to exit." << endl;
			exit(0); 
		}

	}
	//Parent Process
	else
	{
		cout << "I am the Parent. My PID is " << getpid() << " and my parent's PID is " << getppid() << endl;
		sleep(2);
		cout << "I am the Parent, about to call ps." << endl;
		system("ps");
		wait(0);
		cout << "I am the parent, about to exit." << endl;;
		exit(0);
	}

	return 0;
}
