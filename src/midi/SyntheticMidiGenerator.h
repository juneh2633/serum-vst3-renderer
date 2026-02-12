#pragma once

#include <JuceHeader.h>

namespace serum {

/**
 * Generates synthetic single-note MIDI sequences
 * Used for multi-view rendering (note × velocity combinations)
 */
class SyntheticMidiGenerator
{
public:
    /**
     * Constructor
     * @param noteName Note name (e.g., "C4", "A#3")
     * @param velocity MIDI velocity (0-127)
     * @param durationSeconds Note duration in seconds
     * @param sampleRate Sample rate for timing calculations
     */
    SyntheticMidiGenerator(
        const juce::String& noteName,
        int velocity,
        double durationSeconds,
        double sampleRate
    );
    
    /**
     * Generate the MIDI sequence
     */
    void generate();
    
    /**
     * Get MIDI events for a specific block
     * @param blockStartSample Start sample of the block
     * @param blockSize Size of the block
     * @return MIDI buffer for this block
     */
    juce::MidiBuffer popEvents(int64 blockStartSample, int blockSize);
    
    /**
     * Reset to beginning
     */
    void reset();
    
private:
    juce::String noteName;
    int velocity;
    double durationSeconds;
    double sampleRate;
    
    int midiNoteNumber;
    int64 noteOnSample;
    int64 noteOffSample;
    int64 currentPosition;
    
    int noteNameToMidiNumber(const juce::String& name);
};

} // namespace serum
