#include <vector>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "../header/AlertEngine.h"
#include "../header/DataType.h"

/**
 * check the anomaly in daily stat log
 */
void AlertEngine::checkAnomaly(vector<Stat> currentStats,vector<vector<EventTotal> > eventsTotals,vector<Event> events){
    //create daily counters
    vector<double> counters;
    for (int i = 0; i < eventsTotals.size(); ++i) {
        counters.push_back(0);
    }
    //calculate daily thresholds
    vector<double> thresholds;
    for (int i = 0; i < eventsTotals.size(); ++i) {
        double threshold = 0;
        for (int j = 0; j < eventsTotals[i].size(); ++j) {
            threshold += eventsTotals[i][j].amount*events[j].weight;
        }
        threshold*=2;
        thresholds.push_back(threshold);
    }
    //calculate daily counters
    for (int i = 0; i < eventsTotals.size(); ++i) {
        for (int j = 0; j < eventsTotals[i].size(); ++j) {
            //formula: weight * abs(total - mean) / stdDev
            double diff = eventsTotals[i][j].total-currentStats[j].mean;
            if(diff<0) diff = -diff;
            counters[i] += (events[j].weight * diff / currentStats[j].stdDev);
        }
    }
    //print the anomaly status
    cout<<setw(5)<<"Day"<<setw(10)<<"Counter"<<setw(10)<<"Threshold"<<setw(10)<<"Status"<<endl;
    for (int i = 0; i < eventsTotals.size(); ++i) {
        string status = counters[i]>thresholds[i]?"Abnormal":"Okay";
        cout<<setw(5)<<i+1<<setw(10)<<counters[i]<<setw(10)<<thresholds[i]<<setw(10)<<status<<endl;
    }
}


