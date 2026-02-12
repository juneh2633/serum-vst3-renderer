#pragma once

#include <JuceHeader.h>
#include "midi/SyntheticMidiGenerator.h"
#include "render/WavWriter.h"
#include "render/AudioStats.h"

namespace serum {

/**
 * Streaming offline renderer
 * Renders audio block-by-block directly to disk with constant memory usage
 */
class OfflineRenderer
{
public:
    /**
     * Constructor
     * @param plugin Plugin instance to render
     * @param midiGenerator MIDI source (SyntheticMidiGenerator)
     * @param sampleRate Sample rate
     * @param blockSize Block size
     * @param renderLengthSec Main render length in seconds
     * @param tailSec Tail length in seconds (for reverb/delay)
     * @param warmupSec Warmup length in seconds (to stabilize synth)
     */
    OfflineRenderer(
        juce::AudioPluginInstance& plugin,
        SyntheticMidiGenerator& midiGenerator,
        double sampleRate,
        int blockSize,
        double renderLengthSec,
        double tailSec,
        double warmupSec
    );
    
    /**
     * Render to file with streaming
     * @param outputFile Output WAV file
     * @param outStats Output statistics
     * @return true if successful
     */
    bool renderToFile(const juce::File& outputFile, AudioStats& outStats);
    
private:
    juce::AudioPluginInstance& plugin;
    SyntheticMidiGenerator& midiGenerator;
    double sampleRate;
    int blockSize;
    double renderLengthSec;
    double tailSec;
    double warmupSec;
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi);
};

} // namespace serum
