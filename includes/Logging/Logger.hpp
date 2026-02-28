#ifndef LOGGER_H
#define LOGGER_H

// Stings
#include <string_view>

// Data Structures
#include <vector>
#include <queue>

// Multithreading
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include <source_location>

#include "ISink.hpp"

using namespace std;

enum class LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
};

struct LogEntry {
    chrono::system_clock::time_point time;
    string_view severity;
    LogLevel severityEnum;
    string message;
    source_location source;
    thread::id threadID;
};

class Logger {
private: 
    thread worker;
    mutex queueLock;
    mutex sinkLock;
    condition_variable queueConditionalVariable;
    queue<LogEntry> messageQueue;

    vector<unique_ptr<ISink>> sinks;
    bool closingProgram = false;

    Logger();
    
    void logThread();
public:
    ~Logger();
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    };

    void log(LogLevel severity, string info, thread::id threadID = this_thread::get_id(),  source_location source = source_location::current());

    void addSink(unique_ptr<ISink> sink);
    void removeSink(ISink* sinkToRemove);

    string_view logLevelToString(LogLevel level);

    // Removes constructor and copy operators from use to confirm singleton.
    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;
};
#endif