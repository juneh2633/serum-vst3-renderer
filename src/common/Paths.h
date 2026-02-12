#pragma once

#include <JuceHeader.h>

namespace serum {

/**
 * Get the preset states directory (data/preset_states/)
 */
juce::File getPresetStatesDir();

/**
 * Get the MIDI files directory (data/midis/)
 */
juce::File getMidisDir();

/**
 * Get the output WAV directory (data/outwav/)
 */
juce::File getOutputWavDir();

/**
 * Get the output metadata directory (data/outmeta/)
 */
juce::File getOutputMetaDir();

/**
 * Ensure directory exists, create if missing
 * @param dir Directory to ensure exists
 * @return true if directory exists or was created successfully
 */
bool ensureDirectoryExists(const juce::File& dir);

} // namespace serum
