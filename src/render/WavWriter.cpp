#include "render/WavWriter.h"
#include "common/Log.h"

namespace serum {

WavWriter::WavWriter()
    : numChannels(2)
{
}

WavWriter::~WavWriter()
{
    close();
}

bool WavWriter::open(const juce::File& outputFile, double sampleRate, int numChannels)
{
    close();  // Ensure any previous file is closed
    
    this->numChannels = numChannels;
    
    logInfo("Opening WAV file: " + outputFile.getFullPathName());
    logInfo("Sample rate: " + juce::String(sampleRate) + " Hz, Channels: " + juce::String(numChannels));
    
    // Delete existing file
    if (outputFile.existsAsFile())
        outputFile.deleteFile();
    
    // Create parent directory if needed
    outputFile.getParentDirectory().createDirectory();
    
    // Create file stream
    fileStream = std::make_unique<juce::FileOutputStream>(outputFile);
    if (!fileStream->openedOk())
    {
        logError("Failed to open output file");
        fileStream.reset();
        return false;
    }
    
    // Create WAV format writer (24-bit PCM)
    juce::WavAudioFormat wavFormat;
    writer.reset(wavFormat.createWriterFor(
        fileStream.get(),
        sampleRate,
        static_cast<unsigned int>(numChannels),
        24,  // 24-bit
        {},  // metadata
        0    // quality option
    ));
    
    if (writer == nullptr)
    {
        logError("Failed to create WAV writer");
        fileStream.reset();
        return false;
    }
    
    // Release ownership of stream to writer
    fileStream.release();
    
    return true;
}

bool WavWriter::writeBlock(const juce::AudioBuffer<float>& block)
{
    if (writer == nullptr)
    {
        logError("WAV writer not open");
        return false;
    }
    
    int blockChannels = block.getNumChannels();
    int blockSamples = block.getNumSamples();
    
    if (blockSamples == 0)
        return true;  // Nothing to write
    
    // Handle mono-to-stereo conversion if needed
    if (blockChannels == 1 && numChannels == 2)
    {
        // Create temporary stereo buffer
        juce::AudioBuffer<float> stereoBuffer(2, blockSamples);
        stereoBuffer.copyFrom(0, 0, block, 0, 0, blockSamples);
        stereoBuffer.copyFrom(1, 0, block, 0, 0, blockSamples);
        
        return writer->writeFromAudioSampleBuffer(stereoBuffer, 0, blockSamples);
    }
    
    // Direct write
    return writer->writeFromAudioSampleBuffer(block, 0, blockSamples);
}

void WavWriter::close()
{
    if (writer != nullptr)
    {
        writer->flush();
        writer.reset();
        logInfo("Closed WAV file");
    }
    
    fileStream.reset();
}

} // namespace serum
