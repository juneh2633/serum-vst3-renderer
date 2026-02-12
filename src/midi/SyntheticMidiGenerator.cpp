#include "midi/SyntheticMidiGenerator.h"
#include "common/Log.h"

namespace serum {

SyntheticMidiGenerator::SyntheticMidiGenerator(
    const juce::String& noteName,
    int velocity,
    double durationSeconds,
    double sampleRate)
    : noteName(noteName)
    , velocity(velocity)
    , durationSeconds(durationSeconds)
    , sampleRate(sampleRate)
    , midiNoteNumber(60)  // Middle C default
    , noteOnSample(0)
    , noteOffSample(0)
    , currentPosition(0)
{
    midiNoteNumber = noteNameToMidiNumber(noteName);
}

void SyntheticMidiGenerator::generate()
{
    // Note starts at sample 0
    noteOnSample = 0;
    
    // Note ends after duration
    noteOffSample = static_cast<int64>(durationSeconds * sampleRate);
    
    logInfo("Generated synthetic MIDI: " + noteName + 
            " (MIDI " + juce::String(midiNoteNumber) + 
            ") vel=" + juce::String(velocity) + 
            " duration=" + juce::String(durationSeconds) + "s");
    
    reset();
}

juce::MidiBuffer SyntheticMidiGenerator::popEvents(int64 blockStartSample, int blockSize)
{
    juce::MidiBuffer buffer;
    
    int64 blockEndSample = blockStartSample + blockSize;
    
    // Check if note-on falls in this block
    if (noteOnSample >= blockStartSample && noteOnSample < blockEndSample)
    {
        int offsetInBlock = static_cast<int>(noteOnSample - blockStartSample);
        buffer.addEvent(juce::MidiMessage::noteOn(1, midiNoteNumber, static_cast<uint8>(velocity)), 
                       offsetInBlock);
    }
    
    // Check if note-off falls in this block
    if (noteOffSample >= blockStartSample && noteOffSample < blockEndSample)
    {
        int offsetInBlock = static_cast<int>(noteOffSample - blockStartSample);
        buffer.addEvent(juce::MidiMessage::noteOff(1, midiNoteNumber), 
                       offsetInBlock);
    }
    
    currentPosition = blockEndSample;
    return buffer;
}

void SyntheticMidiGenerator::reset()
{
    currentPosition = 0;
}

int SyntheticMidiGenerator::noteNameToMidiNumber(const juce::String& name)
{
    // Parse note name like "C4", "A#3", "Db5"
    if (name.isEmpty())
        return 60;  // Middle C
    
    // Extract note letter
    char noteLetter = name[0];
    int noteIndex = 0;
    
    switch (noteLetter)
    {
        case 'C': case 'c': noteIndex = 0; break;
        case 'D': case 'd': noteIndex = 2; break;
        case 'E': case 'e': noteIndex = 4; break;
        case 'F': case 'f': noteIndex = 5; break;
        case 'G': case 'g': noteIndex = 7; break;
        case 'A': case 'a': noteIndex = 9; break;
        case 'B': case 'b': noteIndex = 11; break;
        default: return 60;
    }
    
    // Check for sharp or flat
    int accidental = 0;
    int octaveStartIndex = 1;
    
    if (name.length() > 1)
    {
        if (name[1] == '#')
        {
            accidental = 1;
            octaveStartIndex = 2;
        }
        else if (name[1] == 'b')
        {
            accidental = -1;
            octaveStartIndex = 2;
        }
    }
    
    // Extract octave number
    int octave = 4;  // Default to octave 4
    if (name.length() > octaveStartIndex)
    {
        octave = name.substring(octaveStartIndex).getIntValue();
    }
    
    // MIDI note number = (octave + 1) * 12 + noteIndex + accidental
    // Middle C (C4) = 60
    int midiNote = (octave + 1) * 12 + noteIndex + accidental;
    
    // Clamp to valid MIDI range (0-127)
    return juce::jlimit(0, 127, midiNote);
}

} // namespace serum
