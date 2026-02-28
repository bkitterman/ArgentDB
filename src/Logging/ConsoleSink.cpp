#include <print>
#include <chrono>

#include <string_view>

#include <Logging/ConsoleSink.hpp>
#include <Logging/Logger.hpp>

using namespace std;

namespace Color {
    constexpr string_view RESET  = "\033[0m";
    constexpr string_view RED    = "\033[31m";
    constexpr string_view GREEN  = "\033[32m";
    constexpr string_view YELLOW = "\033[33m";
    constexpr string_view CYAN   = "\033[36m";
    constexpr string_view BOLD_RED = "\033[1;31m";
}

ConsoleSink::ConsoleSink() {

}

ConsoleSink::~ConsoleSink() {

}

string_view ConsoleSink::getSeverityColor(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return Color::CYAN;
        case LogLevel::INFO:  return Color::GREEN;
        case LogLevel::WARN:  return Color::YELLOW;
        case LogLevel::ERROR: return Color::RED;
        case LogLevel::FATAL: return Color::BOLD_RED;
        default:              return Color::RESET;
    }
}

void ConsoleSink::write(LogEntry& message, bool isPreempted) {
    auto color = getSeverityColor(message.severityEnum);

    println("{}[{:%Y-%m-%dT%H:%M:%S}][{}][{}][{}:{}]{}: {}{}", 
        color,
        chrono::floor<chrono::microseconds>(message.time),
        message.severity,
        message.threadID,
        message.source.file_name(), message.source.line(),
        (isPreempted ? "[PREEMPTED]" : ""),
        message.message,
        Color::RESET
    );
}

