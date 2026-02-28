#ifndef ISINK_H
#define ISINK_H

class ISink {
    public:
        virtual ~ISink() = default;
        virtual void write(struct LogEntry& message, bool isPreempted = false) = 0;
};

#endif