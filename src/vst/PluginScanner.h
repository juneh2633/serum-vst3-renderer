#pragma once

#include <JuceHeader.h>
#include <memory>

namespace serum {

/**
 * Scans for VST3 plugins and caches results
 */
class PluginScanner
{
public:
    PluginScanner();
    ~PluginScanner();
    
    /**
     * Load plugin list from cache if available, otherwise scan and save cache
     * @return true if plugins are available (from cache or scan)
     */
    bool loadOrScan();
    
    /**
     * Scan default VST3 directories
     */
    void scanDefaultLocations();
    
    /**
     * Save current plugin list to cache
     */
    void saveCache();
    
    /**
     * Find Serum2 plugin by name
     * @return PluginDescription if found, nullptr otherwise
     */
    juce::PluginDescription* findSerum2();
    
    /**
     * Get the known plugin list
     */
    juce::KnownPluginList& getPluginList() { return pluginList; }
    
private:
    juce::KnownPluginList pluginList;
    juce::File getCacheFile();
};

} // namespace serum
