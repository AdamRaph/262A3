#include<iostream>
#include<fstream>
#include<cstdlib>
#include<sstream>
using namespace std;

struct event
{
	string name;
	char CDE;
	int min;
	int max;
	string units;
	int weight;
	bool logged;
};

struct stat
{
	string name;
	double mean;
	double stdDev;
	bool logged;
};

/*Functions*/
//Reads in data from the events file
void readInEvents(ifstream& inFile, event eventArray[50], int& numEvents);
//Reads in data from the stats file
void readInStats(ifstream& inFile, stat statArray[50], int& numStats);
//Generates logs
bool generateLogs(event eventArray[50], int& numEvents, int days, string username);

void analysisEngine(int days){
	
	ifstream completeLog("totalLogs.txt"); // open complete log file
	
	/*Possible Variables*/
	long logins=0, Elogins=0, Dlogins=0;
	long timeonline=0;
	long emailsent=0;
	long download=0;
	long moneymade=0;
	
	
	
	int c=0,e=0,d=0;
	char eventType;
	string event;
	
	// Dont need first 4 lines of file
	string junk;
	getline(completeLog,junk);
	getline(completeLog,junk);
	getline(completeLog,junk);
	getline(completeLog,junk);
	
	
	
	completeLog >> eventType;
	if (eventType == 'D'){
		d++;
		completeLog >> event >> event;
		
	}
	else if (eventType == 'E'){
		e++;
		completeLog >> event >> event;
		
	}
	else if (eventType == 'C'){
		c++;
		completeLog >> event >> event;
		
	}
	
	
	
	
	
	cout << "***** Begin Report *****" << endl;
	cout << "Number of days: " << days << endl;
	for(int i = 1; i <= days; i++){
		cout << "Day " << i << " of " << days << ":" << endl;
		
		
		
	}
	
	
	
}


int main(int argc, char* argv[])
{
	
	if(argc != 5)
	{
		cerr << "Usage: IDS <eventsFile> <username> <statsFile> <days>" << endl;
		return -1;
	}
	
	/*Capture colonnd line arguments.*/
	int days = atoi(argv[4]);
	string username = argv[2];
	string eventsFile = argv[1];
	string statsFile = argv[3];
	int numStats, numEvents;
	/*Start reading in from Events file.*/
	ifstream inFile;
	inFile.open(eventsFile.c_str());
	
	if(!inFile.good())
	{
		cerr << "Error reading Events file. Exiting..." << endl;
		return -1;
	}
	
	event eventArray[50] = {"",'Z',-1,-1,"",-1};
	readInEvents(inFile, eventArray, numEvents);
	cout << "Events read in success." << endl;
	
	/*Start reading in from Stats file.*/
	inFile.open(statsFile.c_str());
	if(!inFile.good())
	{
		cerr << "Error reading Stats file. Exiting..." << endl;
		return -1;
	}
	
	stat statArray[50] = {"",-1,-1.0, false};
	readInStats(inFile, statArray, numStats);
	cout << "Stats read in success." << endl;
	
	/*Reading in from Stats file complete*/
	/* Generate Logs */
	
	if(!generateLogs(eventArray, numEvents, days, username))
	{
		cerr << "Generating files unsuccessful. Exiting..." << endl;
		return -1;
	}
	
	/*Analysis Engine - Generate Report */
	analysisEngine(days);
	
	return 0;
}


bool generateLogs(event eventArray[50], int& numEvents, int days, string username)
{
	srand(time(NULL));
	bool done;
	int randIndex;
	int randVal, dailyEvents;
	/*OutFiles are the individual day logs*/
	ofstream outFile[days];
	ofstream completeLog;
	/*CompleteLog is a single file to store all logs*/
	completeLog.open("totalLogs.txt");
	if(!completeLog.good())
	{
		cerr << "Error creating total logs. Exiting..." << endl;
		return false;
	}
	completeLog << "User: " << username << endl;
	
	string dayFiles[days];
	stringstream ss;
	
	cout << "Generating Logs." << endl;
	for(int i=0;i<days;i++)
	{
		/*Creates filenames*/
		ss << "Day" << i+1 << "Logs.txt";
		dayFiles[i] = ss.str();
		ss.str("");
	}
	for(int daysLogged=0;daysLogged<days;daysLogged++)
	{
		/*Randomly selects how many events happen on this day*/
		dailyEvents = rand() % 20 + 5;
		/*Opens each individual day file and creates a log*/
		outFile[daysLogged].open(dayFiles[daysLogged].c_str());
		if(outFile[daysLogged].good())
		{
			outFile[daysLogged] << "User: " << username << endl << "-----------" << endl;
			done = false;
			completeLog << "-----------" << endl << "<Day " << daysLogged+1 << ">" << endl << endl;
			
			for(int eventCount=0;eventCount<dailyEvents;eventCount++)
			{
				randIndex = rand() % numEvents + 1; //generates an index from 1 to numEvents
				if(eventArray[randIndex-1].CDE == 'C')
				{
					/*Continuous event, has min and max field*/
					randVal = rand() % eventArray[randIndex-1].max + eventArray[randIndex-1].min;
					outFile[daysLogged]  << eventArray[randIndex-1].CDE << " event: " << eventArray[randIndex-1].name << ": " << randVal << " " <<  eventArray[randIndex-1].units<< endl;
					completeLog << eventArray[randIndex-1].CDE << " event: " << eventArray[randIndex-1].name << ": " << randVal << " " <<  eventArray[randIndex-1].units<< endl;
				}
				else if(eventArray[randIndex-1].CDE == 'D')
				{
					/*Discrete event, has no max field*/
					randVal = rand() % 10 + eventArray[randIndex-1].min;
					outFile[daysLogged] << eventArray[randIndex-1].CDE << " event: " << eventArray[randIndex-1].name << ": " << randVal  << endl;
					completeLog << eventArray[randIndex-1].CDE << " event: " << eventArray[randIndex-1].name << ": " << randVal  << endl;
				}
				else if(eventArray[randIndex-1].CDE == 'E')
				{
					/*E event, has no min or max field*/
					randVal = rand();
					outFile[daysLogged]  << eventArray[randIndex-1].CDE << " event: " << eventArray[randIndex-1].name << ": " << randVal << " " <<  eventArray[randIndex-1].units << endl;
					completeLog << eventArray[randIndex-1].CDE << " event: " << eventArray[randIndex-1].name << ": " << randVal << " " <<  eventArray[randIndex-1].units << endl;
				}
				done = true;
			}
		}
		else
		{
			cerr << "Error creating daily logs. Exiting..." << endl;
			return false;
		}
	}
	completeLog << "-----------";
	
	/*Close files for insertion*/
	for(int i=0;i<days;i++)
		outFile[i].close();
	completeLog.close();
}

void readInStats(ifstream& inFile, stat statArray[50], int& numStats)
{
	char colon, tmp;
	inFile >> numStats;
	inFile.get(tmp);
	string temp;
	
	for(int i=0;i<numStats;i++)
	{
		getline(inFile, statArray[i].name, ':');
		getline(inFile, temp, ':');
		statArray[i].mean = atof(temp.c_str());
		getline(inFile, temp, ':');
		statArray[i].stdDev = atof(temp.c_str());
		getline(inFile, temp, '\n');
		/*Output Test
		 cout << statArray[i].name << ":" << statArray[i].mean << ":" << statArray[i].stdDev << ":" << endl;*/
	}
	inFile.close();
}


void readInEvents(ifstream& inFile, event eventArray[50], int& numEvents)
{
	char colon, tmp;
	inFile >> numEvents;
	inFile.get(tmp);
	inFile.get(tmp);
	string temp;
	
	for(int i=0;i<numEvents;i++)
	{
		getline(inFile, eventArray[i].name, ':');
		getline(inFile, temp, ':');
		eventArray[i].CDE = temp[0];
		getline(inFile, temp, ':');
		if(!temp.empty())
		{
			eventArray[i].min = atoi(temp.c_str());
		}
		else
			eventArray[i].min = -1;
		getline(inFile, temp, ':');
		if(!temp.empty())
		{
			eventArray[i].max = atoi(temp.c_str());
		}
		else
			eventArray[i].max = -1;
		getline(inFile, temp, ':');
		if(!temp.empty())
		{
			eventArray[i].units = temp;
		}
		getline(inFile, temp, ':');
		if(!temp.empty())
		{
			eventArray[i].weight = atoi(temp.c_str());
		}
		getline(inFile, temp, '\n');
		
		/*Output Test
		 cout << eventArray[i].name << ":" << eventArray[i].CDE << ":" << eventArray[i].min << ":" << eventArray[i].max << ":" << eventArray[i].units << ":" << eventArray[i].weight << ":" << endl;*/
	}
	
	inFile.close();
	/*Reading in from Events file complete*/
}
