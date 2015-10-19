#include<iostream>
#include<fstream>
#include<cstdlib>
using namespace std;

struct event
{
	string name;
	char CDE;
	int min;
	int max;
	string units;
	int weight;
};

struct stat
{
	string name;
	double mean;
	double stdDev;
};

void readInEvents(fstream& inFile, event eventArray[50])
{
	int numEvents1;
	char colon, tmp;
	inFile >> numEvents1;
	inFile.get(tmp);
	string temp;
	
	for(int i=0;i<numEvents1;i++)
	{
		getline(inFile, eventArray[i].name, ':');
		getline(inFile, temp, ':');
		eventArray[i].CDE = temp[0];
		getline(inFile, temp, ':'); //HL 19/10 added brackets to IF statements for neatness
		if(!temp.empty()){
			eventArray[i].min = atoi(temp.c_str());
		}
		getline(inFile, temp, ':');
		if(!temp.empty()){
			eventArray[i].max = atoi(temp.c_str());
		}
		getline(inFile, temp, ':');
		if(!temp.empty()){
			eventArray[i].units = temp;
		}
		getline(inFile, temp, ':');
		if(!temp.empty()){
			eventArray[i].weight = atoi(temp.c_str());
		}
		getline(inFile, temp, '\n');
		
		/*Output Test
		cout << eventArray[i].name << ":" << eventArray[i].CDE << ":" << eventArray[i].min << ":" << eventArray[i].max << ":" << eventArray[i].units << ":" << eventArray[i].weight << ":" << endl;*/
	}

	inFile.close();
	/*Reading in from Events file complete*/
}		

void readInStats(fstream& inFile, stat statArray[50])
{
	int numEvents2;
	char colon, tmp;
	inFile >> numEvents2;
	inFile.get(tmp);
	string temp;
	
	for(int i=0;i<numEvents2;i++)
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
	
	/*Start reading in from Events file.*/
	fstream inFile;
	inFile.open(eventsFile.c_str());

	if(!inFile.good())
	{
		cerr << "Error reading Events file. Exiting..." << endl;
		return -1;
	}
	
	event eventArray[50] = {"",'Z',-1,100,"",-1};
	readInEvents(inFile, eventArray);
	cout << "Events read in success." << endl;		
				
	/*Start reading in from Stats file.*/
	inFile.open(statsFile.c_str());
	if(!inFile.good())
	{
		cerr << "Error reading Stats file. Exiting..." << endl;
		return -1;
	}
	
	stat statArray[50] = {"",-1,-1.0};
	readInStats(inFile, statArray);	
	cout << "Stats read in success." << endl;
	
	/*Reading in from Stats file complete*/
	
	return 0;
}
