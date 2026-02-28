#include <print>

#include <Logging/Logger.hpp>
#include <Logging/ConsoleSink.hpp>

using namespace std;

int main() {
    println("Hello, world!");

    Logger& log = Logger::getInstance();
    
    sleep(5);

    log.log(LogLevel::INFO, "Gday!");
    log.log(LogLevel::DEBUG, "Buggy?");
    log.log(LogLevel::WARN, "Look out!");

    sleep(5);


    log.log(LogLevel::FATAL, "Fatal Test");
    // For the pretties
    log.log(LogLevel::INFO, "Program Terminated");
    println();

    return 0;
}