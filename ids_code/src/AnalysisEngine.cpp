#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <stdexcept>
#include "../header/AnalysisEngine.h"
#include "../header/DataHandler.h"

/**
 * init analysis engine
 */
void AnalysisEngine::init(string logFilePathN,bool isSingleLogN) {
    this->logFilePath = logFilePathN;
    this->isSingleLog = isSingleLogN;
}

/**
 * analyse the daily logs and output stats
 */
vector<vector<EventTotal> > AnalysisEngine::analyse(vector<vector<EventData> > dailyLogs,vector<Event> events){
    //1.gen totals
    vector<vector<EventTotal> > eventsTotals = this->calTotal(dailyLogs,events);
    //2.cal mean and deviation
    vector<Stat> currentStats = this->calMeanAndStdDev(eventsTotals,events);
    //3.write daily total to file
    this->writeEventsTotals(eventsTotals);
    //4.write current stats to file
    this->writeCurrentStats(currentStats);

    return eventsTotals;
}

/**
 * calculate daily totals
 */
vector<vector<EventTotal> > AnalysisEngine::calTotal(vector<vector<EventData> > dailyLogs,vector<Event> events){
    vector<vector<EventTotal> > eventsTotals;
    for (int i = 0; i < dailyLogs.size(); ++i) {
        cout<<"Analysing day "<<i+1<<"..."<<endl;
        vector<EventTotal> eventsTotal;
        //init events total array
        for (int j = 0; j < events.size(); ++j) {
            EventTotal eTotal;
            eTotal.name = events[j].name;
            eTotal.total = 0;
            eTotal.amount = 0;
            eventsTotal.push_back(eTotal);
        }
        //stat total
        for (int j = 0; j < dailyLogs[i].size(); ++j) {
            for (int k = 0; k < eventsTotal.size(); ++k) {
                if(dailyLogs[i][j].name == eventsTotal[k].name){
                    eventsTotal[k].total += dailyLogs[i][j].value;
                    eventsTotal[k].amount += 1;
                }
            }
        }
        eventsTotals.push_back(eventsTotal);
    }
    return eventsTotals;
}

/**
 * calculate overall mean and standard deviation
 */
vector<Stat> AnalysisEngine::calMeanAndStdDev(vector<vector<EventTotal> > eventsTotals,vector<Event> events){
    vector<Stat> currentStats;
    //init current stats
    for (int i = 0; i < events.size(); ++i) {
        Stat stat;
        stat.name = events[i].name;
        stat.mean = 0;
        stat.stdDev = 0;
        currentStats.push_back(stat);
    }
    //cal mean
    for (int i = 0; i < eventsTotals.size(); ++i) {
        for (int j = 0; j < eventsTotals[i].size(); ++j) {
            currentStats[j].mean+=eventsTotals[i][j].total;
        }
    }
    for (int i = 0; i < currentStats.size(); ++i) {
        currentStats[i].mean/=eventsTotals.size();
    }
    //cal standard deviation
    for (int i = 0; i < eventsTotals.size(); ++i) {
        for (int j = 0; j < eventsTotals[i].size(); ++j) {
            currentStats[j].stdDev+=(eventsTotals[i][j].total-currentStats[j].mean)*(eventsTotals[i][j].total-currentStats[j].mean);
        }
    }
    for (int i = 0; i < currentStats.size(); ++i) {
        currentStats[i].stdDev=sqrt(currentStats[i].stdDev/eventsTotals.size());
    }

    return currentStats;
}

/**
 * write the current stats into file
 */
void AnalysisEngine::writeCurrentStats(vector<Stat> currentStat){
    cout<<"Generating current statistics..."<<endl;
    string errorMsg;
    ofstream statsWriter;
    string fileName = this->logFilePath+CURRENT_STAT_FILENAME;
    statsWriter.open(fileName.c_str());
    if(!statsWriter){
        errorMsg = "Current Statistics generating error, exiting...";
        throw runtime_error(errorMsg);
    }
    //write to file
    stringstream ss;
    ss<<currentStat.size();
    string content = ss.str()+"\n";
    for (int i = 0; i < currentStat.size(); ++i) {
        stringstream ss;
        ss<<currentStat[i].name<<":"<<fixed<<setprecision(2)<<currentStat[i].mean<<":"<<fixed<<setprecision(2)<<currentStat[i].stdDev<<":";
        content+=ss.str();
        if((currentStat.size()-1)!=i) content+="\n";
    }
    statsWriter<<content;
    statsWriter.close();
}

/**
 * write daily events totals
 */
void AnalysisEngine::writeEventsTotals(vector<vector<EventTotal> > eventsTotals){
    cout<<"Generating events totals..."<<endl;
    string errorMsg;
    ofstream totalsWriter;
    string fileName = this->logFilePath+DAILY_TOTAL_FILENAME;
    totalsWriter.open(fileName.c_str());
    if(!totalsWriter){
        errorMsg = "Events totals generating error, exiting...";
        throw runtime_error(errorMsg);
    }
    //write to file
    string content = "";
    for (int i = 0; i < eventsTotals.size(); ++i) {
        stringstream ss;
        ss<<"#DAY "<<i+1<<" TOTALS"<<endl;
        for (int j = 0; j < eventsTotals[i].size(); ++j) {
            ss << "name:"<<eventsTotals[i][j].name << "|" <<"total:"<< eventsTotals[i][j].total << "|" << "amount:"<<eventsTotals[i][j].amount <<"|"<<endl;
        }
        content+=ss.str();
    }
    totalsWriter<<content;
    totalsWriter.close();
}

/**
 * read file from complete log or days logs
 */
vector<vector<EventData> > AnalysisEngine::readInDailyLogs(int days){
    string errMsg;
    ifstream logFile;
    vector<vector<EventData> > dailyLogs;
    //single log mode
    if(this->isSingleLog){
        string fileName = this->logFilePath+COMPLETE_LOG_FILENAME;
        logFile.open(fileName.c_str());
        if(!logFile){
            errMsg = "Error reading single log file. Exiting...";
            throw runtime_error(errMsg);
        }
        for (int i = 0; i <= days; i++) {
            vector<EventData> dailyLog;
            while(logFile.good()&&!logFile.eof()){
                string line;
                getline(logFile,line);
                if(line=="") break; // the last line is empty, need to ignore it
                if(line.find("#DAY ")!=-1){
                    break;
                }
                vector<string> dataStrs = DataHandler::split(line,'|');
                EventData eData;
                eData.time = dataStrs[1].substr(5);
                eData.name = dataStrs[2].substr(11);
                eData.eventType = DataHandler::getEventType(dataStrs[3].substr(5));
                eData.value = atof(dataStrs[4].substr(6).c_str());
                eData.units = dataStrs[5].substr(6);
                dailyLog.push_back(eData);
            }
            if(i != 0) {
                dailyLogs.push_back(dailyLog);
            }
        }
        logFile.close();
    }

    //multi log mode
    if(!this->isSingleLog){
        for (int i = 1; i <= days; ++i) {
            stringstream ss;
            ss<<i;
            string dayStr = ss.str();
            vector<EventData> dailyLog;
            string fileName = this->logFilePath + DAYLY_LOG_PREFIX + dayStr + DAYLY_LOG_POSTFIX;
            logFile.open(fileName.c_str());
            if(!logFile){
                errMsg = "Error reading day "+dayStr+" log file. Exiting...";
                throw runtime_error(errMsg);
            }
            while(logFile.good()&&!logFile.eof()){
                string line;
                getline(logFile,line);
                if(line=="") break; // the last line is empty, need to ignore it
                vector<string> dataStrs = DataHandler::split(line,'|');
                EventData eData;
                eData.time = dataStrs[1].substr(5);
                eData.name = dataStrs[2].substr(11);
                eData.eventType = DataHandler::getEventType(dataStrs[3].substr(5));
                eData.value = atof(dataStrs[4].substr(6).c_str());
                eData.units = dataStrs[5].substr(6);
                dailyLog.push_back(eData);
            }
            logFile.close();
            dailyLogs.push_back(dailyLog);
        }
    }

    cout<<"Logs read in success."<<endl;
    return dailyLogs;
}
