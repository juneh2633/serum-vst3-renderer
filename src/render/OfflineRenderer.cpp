#include "render/OfflineRenderer.h"
#include "common/Log.h"

namespace serum {

OfflineRenderer::OfflineRenderer(
    juce::AudioPluginInstance& plugin,
    SyntheticMidiGenerator& midiGenerator,
    double sampleRate,
    int blockSize,
    double renderLengthSec,
    double tailSec,
    double warmupSec)
    : plugin(plugin)
    , midiGenerator(midiGenerator)
    , sampleRate(sampleRate)
    , blockSize(blockSize)
    , renderLengthSec(renderLengthSec)
    , tailSec(tailSec)
    , warmupSec(warmupSec)
{
}

bool OfflineRenderer::renderToFile(const juce::File& outputFile, AudioStats& outStats)
{
    logInfo("Starting offline render");
    logInfo("Sample rate: " + juce::String(sampleRate) + " Hz");
    logInfo("Block size: " + juce::String(blockSize));
    logInfo("Warmup: " + juce::String(warmupSec) + "s");
    logInfo("Render: " + juce::String(renderLengthSec) + "s");
    logInfo("Tail: " + juce::String(tailSec) + "s");
    
    // Reset statistics
    outStats.reset();
    
    // Prepare plugin
    logInfo("Preparing plugin for playback");
    plugin.prepareToPlay(sampleRate, blockSize);
    plugin.setNonRealtime(true);
    
    // Open WAV writer
    WavWriter wavWriter;
    int numChannels = std::max(2, plugin.getTotalNumOutputChannels());
    if (!wavWriter.open(outputFile, sampleRate, numChannels))
    {
        logError("Failed to open WAV writer");
        return false;
    }
    
    // Allocate single block buffer (constant memory usage)
    juce::AudioBuffer<float> buffer(numChannels, blockSize);
    juce::MidiBuffer emptyMidi;
    
    int64 currentSample = 0;
    
    // Phase 1: Warmup (discard output)
    int64 warmupSamples = static_cast<int64>(warmupSec * sampleRate);
    int64 warmupBlocks = (warmupSamples + blockSize - 1) / blockSize;
    
    if (warmupBlocks > 0)
    {
        logInfo("Warmup phase: " + juce::String(warmupBlocks) + " blocks");
        for (int64 i = 0; i < warmupBlocks; ++i)
        {
            buffer.clear();
            processBlock(buffer, emptyMidi);
            // Discard output during warmup
        }
    }
    
    // Reset MIDI generator
    midiGenerator.reset();
    currentSample = 0;
    
    // Phase 2: Main render with MIDI
    int64 renderSamples = static_cast<int64>(renderLengthSec * sampleRate);
    int64 renderBlocks = (renderSamples + blockSize - 1) / blockSize;
    
    logInfo("Main render phase: " + juce::String(renderBlocks) + " blocks");
    for (int64 i = 0; i < renderBlocks; ++i)
    {
        buffer.clear();
        
        // Get MIDI events for this block
        auto midi = midiGenerator.popEvents(currentSample, blockSize);
        
        // Process block
        processBlock(buffer, midi);
        
        // Stream to file
        if (!wavWriter.writeBlock(buffer))
        {
            logError("Failed to write audio block");
            wavWriter.close();
            return false;
        }
        
        // Update statistics online
        outStats.updateBlock(buffer);
        
        currentSample += blockSize;
        
        // Progress logging
        if (i % 100 == 0 && i > 0)
        {
            logInfo("Rendered " + juce::String(i) + " / " + juce::String(renderBlocks) + " blocks");
        }
    }
    
    // Phase 3: Tail (empty MIDI, continue rendering)
    int64 tailSamples = static_cast<int64>(tailSec * sampleRate);
    int64 tailBlocks = (tailSamples + blockSize - 1) / blockSize;
    
    if (tailBlocks > 0)
    {
        logInfo("Tail phase: " + juce::String(tailBlocks) + " blocks");
        for (int64 i = 0; i < tailBlocks; ++i)
        {
            buffer.clear();
            processBlock(buffer, emptyMidi);
            
            if (!wavWriter.writeBlock(buffer))
            {
                logError("Failed to write tail block");
                wavWriter.close();
                return false;
            }
            
            outStats.updateBlock(buffer);
        }
    }
    
    // Finalize statistics
    outStats.finalize();
    
    // Close WAV file
    wavWriter.close();
    
    logInfo("Render complete!");
    logInfo("Peak L/R: " + juce::String(outStats.peakL, 3) + " / " + juce::String(outStats.peakR, 3));
    logInfo("RMS L/R: " + juce::String(outStats.rmsL, 3) + " / " + juce::String(outStats.rmsR, 3));
    
    // Release plugin
    plugin.releaseResources();
    
    return true;
}

void OfflineRenderer::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    plugin.processBlock(buffer, midi);
}

} // namespace serum
