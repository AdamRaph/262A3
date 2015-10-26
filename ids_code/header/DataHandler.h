#include <string>
#include <vector>
#include "DataType.h"

using namespace std;

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

class DataHandler {
private:
    //variables
    string userName;
    string statsFilePath;
    string eventsFilePath;
    int days;

public:
    //Initial Input functions
    void init(string,string,string,string);
    vector<Event> readInEvents(string filePath = "");
    vector<Stat> readInStats(string filePath = "",bool isPrint = false);
    void checkConsistency(vector<Event> events,vector<Stat> stats);

    //getter and setter
    int getDays();
    string getUserName();

    //helper functions for engines
    static vector<string> split(const string& str,char delimiter);
    static EventType getEventType(string typeStr);
    static string getEventTypeName(EventType eventType);
};


#endif //262A3_DATAINITIALIZER_H
