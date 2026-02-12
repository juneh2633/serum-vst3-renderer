#include "vst/PluginScanner.h"
#include "common/Log.h"

namespace serum {

PluginScanner::PluginScanner()
{
}

PluginScanner::~PluginScanner()
{
}

juce::File PluginScanner::getCacheFile()
{
    return juce::File::getCurrentWorkingDirectory().getChildFile("plugin_cache.xml");
}

bool PluginScanner::loadOrScan()
{
    auto cacheFile = getCacheFile();
    
    if (cacheFile.existsAsFile())
    {
        logInfo("Loading plugin cache from: " + cacheFile.getFullPathName());
        auto xml = juce::parseXML(cacheFile);
        if (xml != nullptr)
        {
            pluginList.recreateFromXml(*xml);
            logInfo("Loaded " + juce::String(pluginList.getNumTypes()) + " plugins from cache");
            return pluginList.getNumTypes() > 0;
        }
        logWarning("Failed to parse plugin cache, will rescan");
    }
    
    logInfo("No valid cache found, scanning VST3 plugins...");
    scanDefaultLocations();
    saveCache();
    
    return pluginList.getNumTypes() > 0;
}

void PluginScanner::scanDefaultLocations()
{
    juce::VST3PluginFormat vst3Format;
    
    // Default Windows VST3 path
    juce::String defaultPath = "C:\\Program Files\\Common Files\\VST3";
    
    logInfo("Scanning VST3 directory: " + defaultPath);
    
    juce::PluginDirectoryScanner scanner(
        pluginList,
        vst3Format,
        juce::FileSearchPath(defaultPath),
        true,  // recursive
        juce::File()  // dead mans pedal file (unused)
    );
    
    juce::String pluginBeingScanned;
    while (scanner.scanNextFile(true, pluginBeingScanned))
    {
        logInfo("Scanning: " + pluginBeingScanned);
    }
    
    logInfo("Scan complete. Found " + juce::String(pluginList.getNumTypes()) + " plugins");
}

void PluginScanner::saveCache()
{
    auto cacheFile = getCacheFile();
    auto xml = pluginList.createXml();
    
    if (xml != nullptr)
    {
        if (xml->writeToFile(cacheFile, {}))
        {
            logInfo("Saved plugin cache to: " + cacheFile.getFullPathName());
        }
        else
        {
            logError("Failed to save plugin cache");
        }
    }
}

juce::PluginDescription* PluginScanner::findSerum2()
{
    for (int i = 0; i < pluginList.getNumTypes(); ++i)
    {
        auto* desc = pluginList.getType(i);
        if (desc->name.containsIgnoreCase("Serum"))
        {
            logInfo("Found Serum plugin: " + desc->name + " by " + desc->manufacturerName);
            return desc;
        }
    }
    
    logError("Serum plugin not found in plugin list");
    return nullptr;
}

} // namespace serum
