#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "ActivityEngine.h"
#include "DataHandler.h"

/**
 * init activity engine with log file path and mode
 */
void ActivityEngine::init(string logFilePathN, bool isSingleLogN) {
    this->logFilePath = logFilePathN;
    this->isSingleLog = isSingleLogN;
}

/**
 * generate logs daily
 * TODO, money can be negative
 */
void ActivityEngine::generateLogs(vector<Event> events, vector<Stat> stats, int days, string userName) {
    srand(time(NULL));
    //clear the old log
    this->cleanOldLogs();
    //begin generating
    for (int currentDay = 1; currentDay <= days; ++currentDay) {
        cout << "Genterating Day " << currentDay << " events..." << endl;
        vector<double> eventTotals;
        vector<EventData> eventDatas;
        //generate each event total by gaussian random function in a day
        for (int j = 0; j < stats.size(); ++j) {
            double eventTotal = this->carmenGaussianRandom(stats[j].mean, stats[j].stdDev);
            if (events[j].eventType != C) {
                eventTotal = lround(eventTotal);
            }
            eventTotals.push_back(eventTotal);
        }
        //generate instances of events in a day
        for (int j = 0; j < events.size(); ++j) {
            //count how many values have been taken away from total
            double counter = 0;
            //start generate instances of a event
            while (eventTotals[j] - counter > 0) {
                EventData eData;
                eData.name = events[j].name;
                eData.eventType = events[j].eventType;
                eData.units = events[j].units;
                eData.time = genTime();
                //check if min is greater than the rest value
                if(events[j].min>eventTotals[j] - counter&&eventDatas.size()>0){
                    eventDatas[eventDatas.size()-1].value+=eventTotals[j] - counter;
                    break;
                }
                //generate value differ from CDE
                if(eData.eventType == C){
                    double range = ((eventTotals[j] - counter)<events[j].max?(eventTotals[j] - counter):events[j].max)-events[j].min;
                    //if range == 1, then the random number would mostly be zero, need to get rid of this situation
                    eData.value = range<=1?range:(events[j].min + (rand() * range / double(RAND_MAX)));
                }
                if(eData.eventType == D){
                    eData.value = 1;
                }
                if(eData.eventType == E){
                    long range = ((eventTotals[j] - counter)<events[j].max?(eventTotals[j] - counter):events[j].max)-events[j].min;
                    //if range == 1, then the random number would mostly be zero, need to get rid of this situation
                    eData.value = range<=1?range:(events[j].min + rand()%range);
                }
                eventDatas.push_back(eData);
                counter+=eData.value;
            }
        }
        //write daily log
        sort(eventDatas.begin(),eventDatas.end(),compare);
        this->writeDailyLogEntries(currentDay,userName,eventDatas);
    }
}

/**
 * generate random number followed the given distribution
 */
double ActivityEngine::carmenGaussianRandom(double mean, double std) {
    const double norm = 1.0 / (RAND_MAX + 1.0);
    double u = 1.0 - rand() * norm;
    double v = rand() * norm;
    double z = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);
    return mean + std * z;
}

/**
 * generate a random time in a day
 */
string ActivityEngine::genTime(){
    int hour = rand()%24;
    int minute = rand()%60;
    int second = rand()%60;
    stringstream ss;
    if(hour<10) ss<<0;
    ss<<hour<<":";
    if(minute<10) ss<<0;
    ss<<minute<<":";
    if(second<10) ss<<0;
    ss<<second;
    return ss.str();
}

/**
 * write daily logs into single file or files
 */
void ActivityEngine::writeDailyLogEntries(int currentDay, string userName, vector<EventData> eDatas) {
    stringstream ss;
    string errMsg;
    string fileName = logFilePath;
    ss<<currentDay;
    string dayStr = ss.str();
    ofstream logWriter;
    string str;
    //handle parameters differ from single log and multi log
    if (this->isSingleLog) {
        //write into single file
        fileName += COMPLETE_LOG_FILENAME;
        logWriter.open(fileName.c_str(),ios::app);
        errMsg = "Error creating total logs. Exiting...";
        str += "#DAY " + dayStr + "\n";

    } else {
        //write into daily file
        fileName += DAYLY_LOG_PREFIX + dayStr + DAYLY_LOG_POSTFIX;
        logWriter.open(fileName.c_str());
        errMsg = "Error creating day " + dayStr + " logs. Exiting...";
    }
    //check output file
    if (!logWriter) {
        throw runtime_error(errMsg);
    }
    //start produce entries of current day
    for (int i = 0; i < eDatas.size(); ++i) {
        str += "user:" + userName + "|";
        str += "time:" + eDatas[i].time + "|";
        str += "event_name:" + eDatas[i].name + "|";
        str += "type:" + DataHandler::getEventTypeName(eDatas[i].eventType) + "|";
        stringstream ss;
        if(eDatas[i].eventType == C){
            ss<<fixed<<setprecision(2)<<eDatas[i].value;
        }else{
            ss<<eDatas[i].value;
        }
        str += "value:" + ss.str() + "|";
        if (eDatas[i].eventType != D) {
            str += "units:" + eDatas[i].units + "|";
        } else {
            str += "units:time|";
        }
        str += "\n";
    }
    //write entries to file
    logWriter << str;
    logWriter.close();
}

/**
 * compare function for sorting
 */
bool ActivityEngine::compare(EventData eData_1, EventData eData_2) {
    return eData_1.time < eData_2.time;
}

/**
 * clean old logs
 */
void ActivityEngine::cleanOldLogs(){
    ofstream oldLogs;
    string fileName = this->logFilePath + COMPLETE_LOG_FILENAME;
    oldLogs.open(fileName.c_str());
    oldLogs.close();
}
