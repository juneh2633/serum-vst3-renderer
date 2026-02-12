#pragma once

#include <JuceHeader.h>
#include <memory>

namespace serum {

/**
 * Factory for creating plugin instances
 */
class PluginFactory
{
public:
    PluginFactory();
    ~PluginFactory();
    
    /**
     * Create a plugin instance synchronously
     * @param desc Plugin description
     * @param errorMsg Output error message if creation fails
     * @return Plugin instance or nullptr on failure
     */
    std::unique_ptr<juce::AudioPluginInstance> createPlugin(
        const juce::PluginDescription& desc,
        juce::String& errorMsg
    );
    
private:
    juce::AudioPluginFormatManager formatManager;
};

} // namespace serum
