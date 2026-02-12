#include "vst/PresetStateIO.h"
#include "common/Log.h"

namespace serum {

bool PresetStateIO::saveState(juce::AudioPluginInstance& plugin, const juce::File& outputFile)
{
    logInfo("Saving plugin state to: " + outputFile.getFullPathName());
    
    juce::MemoryBlock stateData;
    plugin.getStateInformation(stateData);
    
    if (stateData.getSize() == 0)
    {
        logError("Plugin state is empty");
        return false;
    }
    
    if (!outputFile.replaceWithData(stateData.getData(), stateData.getSize()))
    {
        logError("Failed to write state file");
        return false;
    }
    
    logInfo("Saved " + juce::String(stateData.getSize()) + " bytes");
    return true;
}

bool PresetStateIO::loadState(juce::AudioPluginInstance& plugin, const juce::File& inputFile)
{
    logInfo("Loading plugin state from: " + inputFile.getFullPathName());
    
    if (!inputFile.existsAsFile())
    {
        logError("State file does not exist");
        return false;
    }
    
    juce::MemoryBlock stateData;
    if (!inputFile.loadFileAsData(stateData))
    {
        logError("Failed to read state file");
        return false;
    }
    
    if (stateData.getSize() == 0)
    {
        logError("State file is empty");
        return false;
    }
    
    plugin.setStateInformation(stateData.getData(), static_cast<int>(stateData.getSize()));
    
    logInfo("Loaded " + juce::String(stateData.getSize()) + " bytes");
    return true;
}

} // namespace serum
