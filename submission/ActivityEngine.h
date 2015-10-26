#include <vector>
#include <string>
#include "DataType.h"

#ifndef ACTIVITYENGINE_H
#define ACTIVITYENGINE_H

class ActivityEngine {
private:
    string logFilePath;
    bool isSingleLog;
public:
    void init(string logFilePath,bool isSingleLog);
    void generateLogs(vector<Event> events,vector<Stat> stats,int days,string userName);
    void writeDailyLogEntries(int currentDay,string userName,vector<EventData> eventDatas);
    void cleanOldLogs();

    //helper functions of generation
    static double carmenGaussianRandom(double mean,double std);
    static string genTime();
    static bool compare(EventData eData_1,EventData eData_2);
};

#endif
