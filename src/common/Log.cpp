#include "common/Log.h"
#include <iostream>

namespace serum {

static juce::String getLevelPrefix(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Info:    return "[INFO] ";
        case LogLevel::Warning: return "[WARN] ";
        case LogLevel::Error:   return "[ERROR]";
        default:                return "[????] ";
    }
}

void log(LogLevel level, const juce::String& message)
{
    auto timestamp = juce::Time::getCurrentTime().toString(true, true, true, true);
    auto fullMessage = timestamp + " " + getLevelPrefix(level) + " " + message;
    
    std::cout << fullMessage << std::endl;
}

void logInfo(const juce::String& message)
{
    log(LogLevel::Info, message);
}

void logWarning(const juce::String& message)
{
    log(LogLevel::Warning, message);
}

void logError(const juce::String& message)
{
    log(LogLevel::Error, message);
}

} // namespace serum
