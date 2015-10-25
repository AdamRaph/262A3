#ifndef ANALYSISENGINE_H
#define ANALYSISENGINE_H

#include "../header/DataType.h"

using namespace std;

class AnalysisEngine {
private:
    string logFilePath;
    bool isSingleLog;
public:
    void init(string logFilePath, bool isSingleLog);
    vector<vector<EventData> > readInDailyLogs(int days);
    vector<vector<EventTotal> > analyse(vector<vector<EventData> > dailyLogs, vector<Event> events);
    void writeCurrentStats(vector<Stat> currentStat);
    void writeEventsTotals(vector<vector<EventTotal> > eventsTotals);

    //helper functions for statistic
    vector<vector<EventTotal> > calTotal(vector<vector<EventData> > dailyLogs, vector<Event> events);
    vector<Stat> calMeanAndStdDev(vector<vector<EventTotal> > eventsTotals, vector<Event> events);

};

#endif
