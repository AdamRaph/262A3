#ifndef ALERTENGINE_H
#define ALERTENGINE_H

#include <string>
#include "DataType.h"
using namespace std;

class AlertEngine {
public:
    void checkAnomaly(vector<Stat> currentStats,vector<vector<EventTotal> > eventsTotals,vector<Event> events);
};

#endif
