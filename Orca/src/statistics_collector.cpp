#include "stdafx.h"

#include <iostream>

#include "statistics_collector.h"

StatisticsCollector::StatisticsCollector()
{

}

StatisticsCollector::~StatisticsCollector()
{

}

void StatisticsCollector::accumEvent(std::string const &event_name)
{
    eventCounter[event_name]++;
}

uint64_t StatisticsCollector::eventCount(std::string const & event_name)
{
    return eventCounter[event_name];
}

void StatisticsCollector::dump()
{
    for (const auto &stat : eventCounter) {
        std::cout << stat.first << ": " << stat.second << std::endl;
    }
}
