#include <iostream>
#include "../header/DataHandler.h"
#include "../header/ActivityEngine.h"
#include "../header/AnalysisEngine.h"
#include "../header/AlertEngine.h"

#define IDS_FILE_PATH "./"

//check arguments
void checkParams(int argc, char *argv[]);
//main loop of the IDS system
void run(string eventsFilePath,string userName,string statsFilePath,string days);

//program access
int main(int argc, char *argv[]) {
    //check
    checkParams(argc,argv);
    //init system parameters
    bool notFirstRun = false;
    string eventsFilePath = string(argv[1]);
    string userName = string(argv[2]);
    string statsFilePath = string(argv[3]);
    string days = string(argv[4]);
    //main loop
    do{
        //tips
        if(notFirstRun){
            cout<<endl<<endl;
            char exit;
            cout<<"If you want to quit then input \"q\", else input any word to continue."<<endl;
            cin>>exit;
            if(exit == 'q') return EXIT_SUCCESS;
            //re-input
            cout<<"Input Events.txt file path: ";
            cin>>eventsFilePath;
            cout<<"Input Stats.txt file path: ";
            cin>>statsFilePath;
            cout<<"Input days: ";
            cin>>days;
        }
        //main loop body
        run(eventsFilePath,userName,statsFilePath,days);

        notFirstRun= true;
    }while(true);
}

/**
 * main loop body
 */
void run(string eventsFilePath,string userName,string statsFilePath,string days){
    try {
        //true-single log | false-multi log
        bool isSingleLog = true;

        /*********** Initial Input ***********/
        cout << "================== Running Data Initializer ==================" << endl;
        DataHandler dataHandler;
        //init DataHanlder
        dataHandler.init(eventsFilePath,userName,statsFilePath,days);
        //read Events.txt and Stat.txt
        vector<Event> events = dataHandler.readInEvents();
        vector<Stat> stats = dataHandler.readInStats();
        //TODO,check inconsistency

        /*********** Activity Engine ***********/
        cout << endl << "================== Running Activity Engine ==================" << endl;
        ActivityEngine activityEngine;
        activityEngine.init(IDS_FILE_PATH, isSingleLog);
        activityEngine.generateLogs(events, stats, dataHandler.getDays(), dataHandler.getUserName());

        /*********** Analysis Engine ***********/
        cout << endl << "================== Running Analysis Engine ==================" << endl;
        AnalysisEngine analysisEngine;
        analysisEngine.init(IDS_FILE_PATH, isSingleLog);
        vector<vector<EventData> > dailyLogs = analysisEngine.readInDailyLogs(dataHandler.getDays());
        vector<vector<EventTotal> > eventsTotals = analysisEngine.analyse(dailyLogs, events);

        /********** Alert Engine ***********/
        cout << endl << "================== Running Alert Engine ==================" << endl;
        AlertEngine alertEngine;
        string fileName = string(IDS_FILE_PATH) + string(CURRENT_STAT_FILENAME);
        cout << "------------ current statistics ------------" << endl;
        vector<Stat> currentStats = dataHandler.readInStats(fileName, true);
        cout << "------------ anomaly report ------------" << endl;
        alertEngine.checkAnomaly(currentStats, eventsTotals, events);

    } catch (runtime_error except) {
        cerr << except.what() << endl;
    }
}

/**
 * check if the command arguments is invalid
 */
void checkParams(int argc, char *argv[]) {
    string errMsg;
    if (argc != 5) {
        errMsg = "The number of parameters error.\nUsage: ./IDS <eventsFile> <username> <statsFile> <days>";
        throw runtime_error(errMsg);
    }
    if (atoi(argv[4]) <= 0) {
        errMsg = "The number of days must be greater than 0.";
        throw runtime_error(errMsg);
    }
}