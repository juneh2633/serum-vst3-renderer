#pragma once

#include <JuceHeader.h>

namespace serum {

/**
 * Save and load plugin state to/from .bin files
 */
class PresetStateIO
{
public:
    /**
     * Save plugin state to file
     * @param plugin Plugin instance
     * @param outputFile Output .bin file
     * @return true if successful
     */
    static bool saveState(juce::AudioPluginInstance& plugin, const juce::File& outputFile);
    
    /**
     * Load plugin state from file
     * @param plugin Plugin instance
     * @param inputFile Input .bin file
     * @return true if successful
     */
    static bool loadState(juce::AudioPluginInstance& plugin, const juce::File& inputFile);
};

} // namespace serum
