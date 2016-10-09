/***************************************************************************************
CSCI 480 - Assignment 1 - Fall 2016

Programmer: Jacob Muzikoski
Section:    0002
TA:         Mark White
Due Date:   September 3, 2016

Purpose:    This program parses through different text files that belong to the operating system. 
	    This C++ program retrieves specific information about the operating system being used. 
	    
	    Files Paths Used:
	    /proc/cpuinfo
	    /proc/uptime
	    /proc/sys/kernel
****************************************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/***************************************************************************************
Function:   getOStype

Use: 	    Obtains information about what kind of Operating System is being used from /proc/sys/kernel/ostype

Arguments:  None

Returns:    String
****************************************************************************************/
string getOStype()
{
	ifstream file;
	string ostype;

	file.open("/proc/sys/kernel/ostype");

	if(!file)
	{
		cerr << "Error File or Path: /proc/sys/kernel/ostype" << endl << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	
	//Stores the operating system type into a string
	while(!file.eof())
	{
		getline(file, ostype);
		return ostype;
	}

	file.close();
	return ostype;
}

/****************************************************************************************
Function:   getHostname

Use: 	    Obtains information about who the host is from /proc/sys/kernel/ostype

Arguments:  None

Returns:    String
*****************************************************************************************/
string getHostname()
{
	ifstream file;
	string hostname;

	file.open("/proc/sys/kernel/hostname");

	if(!file)
	{
		cerr << "Error File or Path: /proc/sys/kernel/hostname" << endl << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}

	//Stores the hostname type into a string
	while(!file.eof())
	{
		getline(file, hostname);
		return hostname;
	}

	file.close();
	return hostname;
}

/****************************************************************************************
Function:   getOSRelease

Use: 	    Obtains information about when the operating being used was released from /proc/sys/kernel/ostype

Arguments:  None

Returns:    String
*****************************************************************************************/
string getOSRelease()
{
	ifstream file;
	string osrelease;

	file.open("/proc/sys/kernel/osrelease");

	if(!file)
	{
		cerr << "Error File or Path: /proc/sys/kernel/osrelease" << endl << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}

	//Stores the Operating System's release date into a string
	while(!file.eof())
	{
		getline(file, osrelease);
		return osrelease;
	}

	file.close();
	return osrelease;
}

/****************************************************************************************
Function:   getVersion

Use: 	    Obtains information about what version of the Operating System is being used from /proc/sys/kernel/ostype

Arguments:  None

Returns:    String
*****************************************************************************************/
string getVersion()
{
	ifstream file;
	string version;

	file.open("/proc/sys/kernel/version");
		
	if(!file)
	{
		cerr << "Error File or Path: /proc/sys/kernel/version" << endl << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}

	//Stores the version of the Operating System into a string
	while(!file.eof())
	{
		getline(file, version);
		return version;
	}

	file.close();
	return version;
}

int main() 
{
	//File variables
	ifstream file;	 			//Ifstream object that holds file information
	string cpuinfo;				//Stores cpuinfo file into a string
	string proc0;				//Stores lines from only processor 0 into a string
	string uptime;				//Stores uptime file into a string

	//Processor and uptime variables
	int pCount = 0;				//Counts how many processors there are
	vector<string> pid;			//Vector to retrieve a unique size amount of physical multi-core chips
	int chip = 0;				//Stores how many physical multi-core chips there are
	string fpu;				//Stores whether or not there is an fpu into a string
	string vendor_id;			//Stores the vendor id into a string
	string model;				//Stores the model name into a string
	string cpuSpeed;			//Stores the speed of the CPU in MHz into a string
	string pSize;				//Stores the physical size of the addresses into a string
	string vSize;				//Stores the virtual size of the addressess into a string
	string flag;				//Stores all of the flags into a string
	int flagCount = 0;			//Store the number of flags as an integer
	string stringSeconds;			//Stores how many seconds the os has been up into a string
	int seconds;				//Stores how many seconds the os has been up into an int

	//Operating System variables
	string ostype;				//Stores the operating system type into a string
	string hostname;			//Stores the hostname into a string
	string osrelease;			//Stores the Operating System's release into a string
	string version;				//Stores the version of the Operating System into a string

	//CPUINFO FILE
	file.open("/proc/cpuinfo");
	if(!file)
	{
		cerr << "Error File or Path: /proc/cpuinfo" << endl << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	
	while(!file.eof())
	{
		//Retrieves one line at a time from file, and stores it into cpuinfo
		getline(file, cpuinfo); 
	
		//If the string 'processor' is in the next line
		if(cpuinfo.find("processor") == 0)
		{
			//Adds one everytime getline reads in 'processor'
			pCount++;
	
			//Finds how many physical id's are in /proc/cpuinfo and puts them into a vector 
			int position = cpuinfo.find(":") + 2;
			pid.push_back(cpuinfo.substr(position));
		
			//Used to determine what process we are extracting data from
			int procPosition = cpuinfo.find(":") + 2;
			string procLoc = cpuinfo.substr(procPosition);
			
			//If the processor found is processor 0
			if(procLoc == "0")
			{
				getline(file, proc0);
				while(proc0 != "")
				{
					//Finds the string in /proc/cpuinfo that states whether or not it has an FPU with a yes or no
					if(proc0.find("fpu") == 0)
                			{
                				int position = proc0.find(":") + 1;
			                        fpu = proc0.substr(position);
        	        		}
			
					//Finds the string in /proc/cpuinfo that has the vendor id and stores it into a string variable
					else if(proc0.find("vendor_id") == 0)
					{
						int position = proc0.find(":") + 1;		
						vendor_id = proc0.substr(position);
					}
	
					//Finds the string in /proc/cpuinfo that has the model name and stores it into a string variable
					else if(proc0.find("model name") == 0)
					{
						int position = proc0.find(":") + 1;
						model = proc0.substr(position);
					}
	
					//Finds the string in /proc/cpuinfo that has the speed of the cpu and stores it into a string variable
					else if(proc0.find("cpu MHz") == 0)
					{
						int position = proc0.find(":") + 1;
						cpuSpeed = proc0.substr(position);
					}

					//Finds the strings of both Physical and Virtual address sizes and stores it into string variables
					else if(proc0.find("address sizes") == 0)
					{		
						int pPosition = proc0.find(":") + 1;
						pSize = proc0.substr(pPosition, 8);
				
						int vPosition = pPosition + 18;	
						vSize = proc0.substr(vPosition, 8);
					}

					//Finds the string in /proc/cpuinfo that lists all of the flags and stores them into a string variable
					else if(proc0.find("flags") == 0)
					{
						int position = proc0.find(":") + 1;
						flag = proc0.substr(position);				
					}
					
					//Next line	
					getline(file, proc0);
				}
			}
		}
	}

	//Makes all values in vector unique
	pid.erase(unique(pid.begin(), pid.end()), pid.end());
	chip = pid.size();
	file.close();
	
	//Counts how many spaces are in the string flag	
	for(int i = 0; i < flag.length(); i++)
	{
		if(flag[i] == ' ')
		{
			flagCount++;
		}
	}
	
	//UPTIME FILE
	file.open("/proc/uptime");

	if(!file)
	{
		cerr << "Error File or Path: /proc/uptime" << endl << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}

	while(!file.eof())
	{
		//Stores the amount of time the system has been up into string uptime
		getline(file, uptime);

		//Stores string inside uptime until a space is found
		stringSeconds = uptime.substr(0, uptime.find(" "));
		
		//Stores the size of the string into an integer and removes decimals
		int secondsLength = stringSeconds.size() - 2;
		
		//Stores each character in stringSeconds into a character array
		char sec[10];					    
		for(int i = 0; i <= secondsLength; i++)
		{
			sec[i] = stringSeconds[i];
		}
	
		//Converts character array into an integer
		seconds = atoi(sec);
	}
	file.close();
	
	//Converts seconds into days/hours/minutes/seconds format
	int secondsLong = seconds;
	int days = seconds / 60/60/24;
	int hours = (seconds/60/60) % 24;
	int minutes = (seconds/60) % 60;
	seconds = seconds % 60;	

	//Function calls for operating system information
	ostype = getOStype();
	hostname = getHostname();
	osrelease = getOSRelease();
	version = getVersion();

	//OUTPUT
	cout << "Assignment 1: File Parsing" << endl << endl;
	
	cout << "A: Questions about hopper:" << endl;
	cout << "1. Hopper has " << pCount << " processor(s)" << endl;
	cout << "2. Hopper has " << chip << " physical multi-core chip(s)" << endl;
	cout << "3. Hopper has been up for " << secondsLong << " seconds" << endl;
	cout << "4. Hopper has been up for "  << days << " day(s), "  << hours << " hour(s), "  << minutes << " minute(s), " << seconds << " second(s)"  << endl << endl;

	cout << "B. For processor 0, answer these questions:" << endl;
	cout << "1. Does it have a floating point unit (fpu)?:" << fpu << endl;
	cout << "2. The vendor is" << vendor_id << endl;
	cout << "3. The model name is" << model << endl;
	cout << "4. The speed of the cpu is" << cpuSpeed << " MHz" << endl;
	cout << "5. The physical address size is" << pSize << endl;
	cout << "6. The virtual address size is" << vSize << endl;
	cout << "7. Hopper has " << flagCount << " different values listed under 'flags'" << endl << endl;
	
	cout << "C. Print the following information from files in /proc/sys/kernel:" << endl;
	cout << "1. The Operating System is " << ostype << endl;
	cout << "2. The hostname is " << hostname << endl;
	cout << "3. The Operating System release is " << osrelease << endl;
	cout << "4. The Operating System version is " << version << endl;

	return 0;
}
