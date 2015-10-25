#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <cmath>
using namespace std;

// For the Standard Deviation Arrays
#define MAX 500

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
void readInEvents(ifstream& inFile, event eventArray[50], int& numEvents);
void readInStats(ifstream& inFile, stat statArray[50], int& numStats);
bool generateLogs(event eventArray[50], int& numEvents, int days, string username);

double calcMean(int numInstances, long total){
	if ((numInstances ==0) || (total == 0)){
		return 0;
	}
	else return total/numInstances;
}
double calcDev(long * Array, int arraySize ,double mean){
	
	double StandardDeviation=0;
	for(int i = 0; i < arraySize; ++i){
		StandardDeviation+=(Array[i]-mean)*(Array[i]-mean);
	}
	return sqrt(StandardDeviation/arraySize);
}


void analysisEngine(int days){
	cout << endl << "***** Begin Report *****" << endl;
	cout << "Number of days: " << days << endl;
	
	ifstream completeLog("totalLogs.txt"); // open complete log file
	
	// Dont need first 4 lines of file
	string junk;
	getline(completeLog,junk);
	getline(completeLog,junk);
	getline(completeLog,junk);
	getline(completeLog,junk);
	
	int loginsCount=0,timeCount=0,emailsCount=0,downloadCount=0,moneyCount=0;
	long allLogins=0, allTime=0, allEmails=0, allDownload=0, allMoney=0;
	
	long downloadArray[MAX]={0}, loginsArray[MAX]={0}, timeArray[MAX]={0}, emailsArray[MAX]={0}, moneyArray[MAX]={0};
	
	for(int i = 1; i <= days; i++){
		
		char eventType='A';
		long logins=0, loginsTotal=0;
		long time=0, timeTotal=0;
		long emails=0, emailsTotal=0;
		long download=0, downloadTotal=0;
		long money=0, moneyTotal=0;
		
		//int c=0,e=0,d=0;
		string event;
		
		while(eventType != '-'){
			// Read in event type
			completeLog >> eventType;
			if(eventType != '-'){
				//cout << eventType<< " ";	// Debug Line
				/*if (eventType == 'D'){
					d++;
				 }
				 else if (eventType == 'E'){
					e++;
				 }
				 else if (eventType == 'C'){
					c++;
				 }*/
				
				// Read in Event stats
				completeLog >> event >> event;
				//cout << event << " ";					// Debug Line
				string tmpString;
				long tmpLong=0;
				
				if (event == "Download"){
					download++;
					completeLog >> tmpString >> tmpLong >> tmpString;
					downloadTotal = downloadTotal+tmpLong;
					downloadArray[downloadCount] = tmpLong;
					downloadCount++;
					//cout << tmpLong << " ";				// Debug Line
				}
				else if(event == "Time"){
					time++;
					completeLog >> tmpString >> tmpLong >> tmpString;
					timeTotal = timeTotal+tmpLong;
					timeArray[timeCount] = tmpLong;
					timeCount++;
					//cout << tmpLong << " ";				// Debug Line
					
				}
				else if(event == "Money"){
					money++;
					completeLog >> tmpString >> tmpLong >> tmpString;
					moneyTotal = moneyTotal+tmpLong;
					moneyArray[moneyCount] = tmpLong;
					moneyCount++;
					//cout << tmpLong << " ";		// Debug Line
				}
				else if(event == "Emails"){
					emails++;
					completeLog >> tmpString >> tmpLong;
					emailsTotal = emailsTotal+tmpLong;
					emailsArray[emailsCount] = tmpLong;
					emailsCount++;
					//cout << tmpLong << " ";		// Debug Line
				}
				else if(event == "Logins:"){
					logins++;
					completeLog >> tmpLong;
					loginsTotal = loginsTotal+tmpLong;
					loginsArray[loginsCount] = tmpLong;
					loginsCount++;
					//cout << tmpLong << " ";				// Debug Line
				}
				
				//cout << endl;							// Debug Line
			}
		}
		getline(completeLog,junk);
		getline(completeLog,junk);
		
		/* End of Day Printout*/
		cout << "Day " << i << " of " << days << ":" << endl;
		cout << "Total Download volume: " << downloadTotal << " bits" << endl;
		cout << "Total Time Online: " << timeTotal << " Minutes" << endl;
		cout << "Total Money made: " << moneyTotal << " Cents" << endl;
		cout << "Total Emails Sent: " << emailsTotal << endl;
		cout << "Total Logins: " << loginsTotal << endl;
		cout << endl;
		
		/*For Averages*/
		allDownload += downloadTotal;
		allTime += timeTotal;
		allMoney += moneyTotal;
		allEmails += emailsTotal;
		allLogins += loginsTotal;
	}
	
	cout << "**** Overall Statistics *****" << endl;
	cout << "Average Download volume: " << setprecision(50) << calcMean(downloadCount,allDownload) << " bits" << endl;
	cout << "Average Time Online: " << calcMean(timeCount,allTime) << " Minutes" << endl;
	cout << "Average Money made: " << calcMean(moneyCount,allMoney) << " Cents" << endl;
	cout << "Average Emails Sent: " << calcMean(emailsCount,allEmails) << endl;
	cout << "Average Logins: " << calcMean(loginsCount,allLogins) << endl;
	cout << endl;
	
	
	cout << "Standard Deviation Download volume: " << calcDev(downloadArray,downloadCount,calcMean(downloadCount,allDownload)) << " bits" << endl;
	cout << "Standard Deviation Time Online: " << " Minutes" << endl;
	cout << "Standard Deviation Money made: " << " Cents" << endl;
	cout << "Standard Deviation Emails Sent: " << endl;
	cout << "Standard Deviation Logins: " << endl;
	cout << endl;
	
	
	completeLog.close();
	
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
