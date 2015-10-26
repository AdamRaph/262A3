#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "../header/DataHandler.h"

using namespace std;
/*****************************************************************
 *                      major functions                          *
 *****************************************************************/

/**
 * init data handler from the command arguments
 */
void DataHandler::init(string eventsFilePathN,string userNameN,string statsFilePathN,string daysN) {
    this->eventsFilePath = eventsFilePathN;
    this->userName = userNameN;
    this->statsFilePath = statsFilePathN;
    this->days = atoi(daysN.c_str());
}

/**
 * read events from file
 */
vector<Event> DataHandler::readInEvents(string eventsFilePathN) {
    string errMsg;
    if(eventsFilePathN=="") eventsFilePathN = this->eventsFilePath;
    ifstream inFile;
    inFile.open(eventsFilePathN.c_str());
    //check if file exists
    if (!inFile.good()) {
        errMsg = "Error reading Events file. Exiting...";
        throw runtime_error(errMsg);
    }
    //start reading
    int numEvents;
    inFile >> numEvents;
    string line;
    getline(inFile, line);
    vector<Event> events;

    for (int i = 0; i < numEvents; i++) {
        getline(inFile, line);
        //split on line by :
        vector<string> dataStrs = this->split(line, ':');
        //read by order
        Event event;
        event.name = dataStrs[0];
        event.eventType = getEventType(dataStrs[1]);
        event.min = (dataStrs[2] == "" ? LOWER_BOUND : atoi(dataStrs[2].c_str()));
        event.max = (dataStrs[3] == "" ? UPPER_BOUND : atoi(dataStrs[3].c_str()));
        event.units = dataStrs[4];
        event.weight = atoi(dataStrs[5].c_str());
        events.push_back(event);
    }
    inFile.close();
    cout << "Events.txt read in success." << endl;

    return events;
}

/**
 * read statistics from file
 */
vector<Stat> DataHandler::readInStats(string statsFilePathN,bool isPrint) {
    string errMsg;
    if(statsFilePathN=="") statsFilePathN = this->statsFilePath;
    ifstream inFile;
    inFile.open(statsFilePathN.c_str());
    //check if file exists
    if (!inFile.good()) {
        errMsg = "Error reading Stats file. Exiting...";
        throw runtime_error(errMsg);
    }
    //start reading
    int numStats;
    inFile >> numStats;
    string line;
    getline(inFile, line);
    vector<Stat> stats;

    for (int i = 0; i < numStats; i++) {
        getline(inFile, line);
        if(isPrint) cout<<line<<endl;
        //split on line by :
        vector<string> dataStrs = this->split(line, ':');
        //read by order
        Stat stat;
        stat.name = dataStrs[0];
        stat.mean = atof(dataStrs[1].c_str());
        stat.stdDev = atof(dataStrs[2].c_str());
        stats.push_back(stat);
    }
    inFile.close();
    if(!isPrint) cout << "Stats.txt read in success." << endl;

    return stats;
}

/**
 * getter of days
 */
int DataHandler::getDays(){
    return this->days;
}

/**
 * getter of user name
 */
string DataHandler::getUserName() {
    return this->userName;
}

/*****************************************************************
 *                      helper functions                         *
 *****************************************************************/
/**
 * split string by delimiter
 */
vector<string> DataHandler::split(const string &str, char delimiter) {
    vector<string> elems;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delimiter)) {
        elems.push_back(item);
    }
    return elems;
}

/**
 * return enum type CDE
 */
EventType DataHandler::getEventType(string typeStr){
    if(typeStr=="C"){
        return C;
    }
    if(typeStr=="D"){
        return D;
    }
    if(typeStr=="E"){
        return E;
    }
    return C;
}

/**
 * return the name of enum type
 */
string DataHandler::getEventTypeName(EventType eventType){
    if(eventType==C){
        return "C";
    }
    if(eventType==D){
        return "D";
    }
    if(eventType==E){
        return "E";
    }
    return "C";
}

