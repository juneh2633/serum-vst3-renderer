#include "vst/PluginFactory.h"
#include "common/Log.h"

namespace serum {

PluginFactory::PluginFactory()
{
    // Register VST3 format
    formatManager.addDefaultFormats();
}

PluginFactory::~PluginFactory()
{
}

std::unique_ptr<juce::AudioPluginInstance> PluginFactory::createPlugin(
    const juce::PluginDescription& desc,
    juce::String& errorMsg)
{
    logInfo("Creating plugin instance: " + desc.name);
    
    // Synchronous plugin loading for deterministic behavior
    errorMsg = "";
    
    auto* format = formatManager.findFormatForDescription(desc, errorMsg);
    if (format == nullptr)
    {
        errorMsg = "No format found for plugin: " + desc.name;
        logError(errorMsg);
        return nullptr;
    }
    
    std::unique_ptr<juce::AudioPluginInstance> instance;
    instance.reset(format->createInstanceFromDescription(desc, 44100.0, 512));
    
    if (instance == nullptr)
    {
        errorMsg = "Failed to create plugin instance: " + desc.name;
        logError(errorMsg);
        return nullptr;
    }
    
    logInfo("Successfully created plugin instance");
    return instance;
}

} // namespace serum
