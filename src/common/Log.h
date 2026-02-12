#pragma once

#include <JuceHeader.h>

namespace serum {

/**
 * Log levels
 */
enum class LogLevel
{
    Info,
    Warning,
    Error
};

/**
 * Log a message with Info level
 */
void logInfo(const juce::String& message);

/**
 * Log a message with Warning level
 */
void logWarning(const juce::String& message);

/**
 * Log a message with Error level
 */
void logError(const juce::String& message);

/**
 * Generic log function
 */
void log(LogLevel level, const juce::String& message);

} // namespace serum
