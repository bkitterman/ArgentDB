#pragma once
#include <string_view>

#include <Logging/ISink.hpp>
#include <Logging/Logger.hpp>

class ConsoleSink : public ISink {
    public:
        ConsoleSink();
        ~ConsoleSink() override;
        void write(LogEntry& message, bool isPreempted = false) override;
    private:
        string_view getSeverityColor(LogLevel level);
};

#pragma endregion