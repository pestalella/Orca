#pragma once

#include <cstdint>
#include <map>
#include <string>

class StatisticsCollector
{
private:
    std::map<std::string, uint64_t> eventCounter;

public:
    StatisticsCollector();
    virtual ~StatisticsCollector();

    void accumEvent(std::string const &event_name);
    uint64_t eventCount(std::string const &event_name);
    void dump();
};