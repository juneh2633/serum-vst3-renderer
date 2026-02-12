#pragma once

#include <JuceHeader.h>

namespace serum {

/**
 * Streaming WAV file writer
 * Writes audio blocks directly to disk with no buffering
 */
class WavWriter
{
public:
    WavWriter();
    ~WavWriter();
    
    /**
     * Open WAV file for writing
     * @param outputFile Output file path
     * @param sampleRate Sample rate
     * @param numChannels Number of channels (1 or 2)
     * @return true if opened successfully
     */
    bool open(const juce::File& outputFile, double sampleRate, int numChannels);
    
    /**
     * Write audio block to file
     * @param block Audio data to write
     * @return true if written successfully
     */
    bool writeBlock(const juce::AudioBuffer<float>& block);
    
    /**
     * Close the file
     */
    void close();
    
    /**
     * Check if file is open
     */
    bool isOpen() const { return writer != nullptr; }
    
private:
    std::unique_ptr<juce::FileOutputStream> fileStream;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    int numChannels;
};

} // namespace serum
