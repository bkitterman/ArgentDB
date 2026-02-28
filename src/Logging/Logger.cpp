// Strings
#include <print>
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

#include <Logging/ISink.hpp>
#include <Logging/ConsoleSink.hpp>
#include <Logging/Logger.hpp>

using namespace std;

//! Word of caution: Unit tests are not possible/headache inducing with singleton - Need a .reset() method
//TODO In all reality, this is a future problem. Once I start testing, ill look into reset vs. DI.
Logger::Logger() {
    // EVENTUAL - auto initialize sinks based on settings/config.
    //? Defaulting to just cmd line.
    addSink(make_unique<ConsoleSink>());

    // FINAL STEP, start thread. Inform awake.
    worker = thread(&Logger::logThread, this);
    log(LogLevel::INFO, "Logger Initialized");
}

Logger::~Logger() {
    {
        lock_guard lock{queueLock};
        closingProgram = true;
    }
    queueConditionalVariable.notify_all();
    if(worker.joinable()) worker.join();
}


//* INFO must come preformatted. The rest will be put together in worker thread.
void Logger::log(LogLevel severity, string info, thread::id threadID, source_location source) {
    // Build logEntry struct
    LogEntry message = {
        .time = chrono::system_clock::now(),
        .severity = logLevelToString(severity),
        .severityEnum = severity,
        .message = move(info),
        .source = source,
        .threadID = threadID
    };

    {
        lock_guard lock{queueLock};
        messageQueue.push(move(message));
    }
    queueConditionalVariable.notify_one();
    
    //TODO Fatal crashes. Just secure a way to make sure seg-faults dont make it so the program just dies without info.
    /*
    if(fatal)
        force worker thread to abort using atomic flag
        push as much out synchronously as possible on the crashing thread.
            - Needs to work if the async sinks segfault.
            I know reality, its a arbitrary CPU kill. Might need to use secondary app or wrapper to protect it.
            - Primary goal should be to focus the fatal out. Anything else is great but fatal is most important message.
            TL;DR, FLush the fatal and attempt to shutdown as gracefully as possible by self-termination. 
                Just be faster than the OS :tm:
    
            */

    //TODO Log balancer. Essentially,if queue.size > some amount, write synchonously.
    //? This will slow down main thread activities but its better to go slow than drop logs.
}

string_view Logger::logLevelToString(LogLevel level) {
    switch(level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO ";
        case LogLevel::WARN: return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

// I will NOT permit sink changes while running. At least for now. Just dont want to solve this issue yet.
void Logger::addSink(unique_ptr<ISink> sink) {
    sinks.push_back(move(sink));
} 

void Logger::removeSink(ISink* sinkToRemove) {
    erase_if(sinks, [sinkToRemove](const std::unique_ptr<ISink>& s) 
        {return s.get() == sinkToRemove;});
}

void Logger::logThread() {
    queue<LogEntry> localQueue;
    while(true) {
        LogEntry message;
        {
            unique_lock lock{queueLock};

            //Acquire new message
            queueConditionalVariable.wait(lock, [this]{ return !messageQueue.empty() || closingProgram; });

            if(closingProgram && messageQueue.empty()) return;

            swap(localQueue, messageQueue);
        }

        // Send to all sinks, pause modification of sinks
        while(!localQueue.empty()) {
            message = localQueue.front();
            std::erase_if(sinks, [&](auto& sink) {
                try {
                    sink->write(message);
                    return false;
                } catch (const std::exception& e) {
                    log(LogLevel::ERROR, std::format("Error printing to ISink: {}. This sink has been removed.", e.what()));
                    
                    // Return true to remove this sink from the vector
                    return true; 
                }
            });
            localQueue.pop();
        }
    }
}