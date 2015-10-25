#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
using namespace std;

//system constants
#define UPPER_BOUND 9999999
#define LOWER_BOUND 0
#define COMPLETE_LOG_FILENAME "complete_logs.txt"
#define DAYLY_LOG_PREFIX "day_"
#define DAYLY_LOG_POSTFIX "_log.txt"
#define CURRENT_STAT_FILENAME "current_stats.txt"
#define DAILY_TOTAL_FILENAME "daily_totals.txt"

/**
 * event type
 * C: continuous event
 * D: discrete event only counting on times
 * E: discrete event with integer value
 */
enum EventType{C,D,E};

/**
 * event format
 */
struct Event
{
    string name;
    EventType eventType;
    int min;
    int max;
    string units;
    int weight;
};

/**
 * statistic model
 */
struct Stat
{
    string name;
    double mean;
    double stdDev;
};

/**
 * event data
 */
struct EventData
{
    string time;
    string name;
    EventType eventType;
    double value;
    string units;
};

/**
 * the total of each event in each day
 */
struct EventTotal
{
    string name;
    double total;
    int amount; // how many times had happened
};

#endif //262A3_DATATYPE_H
