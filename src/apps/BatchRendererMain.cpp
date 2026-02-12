/*
    Milestone A Test Application
    Tests basic plugin loading and synthetic MIDI rendering
*/

#include <JuceHeader.h>
#include "vst/PluginScanner.h"
#include "vst/PluginFactory.h"
#include "midi/SyntheticMidiGenerator.h"
#include "render/OfflineRenderer.h"
#include "render/AudioStats.h"
#include "common/Log.h"
#include "common/Paths.h"

using namespace serum;

int main(int argc, char* argv[])
{
    juce::ScopedJuceInitialiser_GUI juceInit;
    
    logInfo("=== Milestone A: Plugin Load & Basic Rendering Test ===");
    
    // Test configuration
    const double sampleRate = 44100.0;
    const int blockSize = 512;
    const double warmupSec = 0.2;
    const double renderSec = 2.0;
    const double tailSec = 1.0;
    const auto noteName = juce::String("C4");
    const int velocity = 100;
    
    // Step 1: Scan for plugins
    logInfo("Step 1: Scanning for VST3 plugins");
    PluginScanner scanner;
    if (!scanner.loadOrScan())
    {
        logError("Failed to scan plugins");
        return 1;
    }
    
    // Step 2: Find Serum2
    logInfo("Step 2: Finding Serum2 plugin");
    auto* desc = scanner.findSerum2();
    if (desc == nullptr)
    {
        logError("Serum2 not found!");
        logError("Please ensure Serum2 is installed in: C:\\Program Files\\Common Files\\VST3\\");
        return 1;
    }
    
    logInfo("Found: " + desc->name + " by " + desc->manufacturerName);
    
    // Step 3: Create plugin instance
    logInfo("Step 3: Creating plugin instance");
    PluginFactory factory;
    juce::String errorMsg;
    auto plugin = factory.createPlugin(*desc, errorMsg);
    
    if (plugin == nullptr)
    {
        logError("Failed to create plugin: " + errorMsg);
        return 1;
    }
    
    logInfo("Plugin created successfully");
    logInfo("Input channels: " + juce::String(plugin->getTotalNumInputChannels()));
    logInfo("Output channels: " + juce::String(plugin->getTotalNumOutputChannels()));
    
    // Step 4: Generate synthetic MIDI
    logInfo("Step 4: Generating synthetic MIDI");
    SyntheticMidiGenerator midiGen(noteName, velocity, renderSec, sampleRate);
    midiGen.generate();
    
    // Step 5: Setup output file
    logInfo("Step 5: Setting up output");
    auto outputDir = getOutputWavDir();
    ensureDirectoryExists(outputDir);
    auto outputFile = outputDir.getChildFile("milestone_a_test.wav");
    
    // Step 6: Render
    logInfo("Step 6: Rendering to WAV");
    OfflineRenderer renderer(
        *plugin,
        midiGen,
        sampleRate,
        blockSize,
        renderSec,
        tailSec,
        warmupSec
    );
    
    AudioStats stats;
    if (!renderer.renderToFile(outputFile, stats))
    {
        logError("Rendering failed");
        return 1;
    }
    
    // Step 7: Verify output
    logInfo("Step 7: Verifying output");
    if (!outputFile.existsAsFile())
    {
        logError("Output file was not created");
        return 1;
    }
    
    auto fileSizeKB = outputFile.getSize() / 1024;
    logInfo("Output file: " + outputFile.getFullPathName());
    logInfo("File size: " + juce::String(fileSizeKB) + " KB");
    logInfo("Peak L: " + juce::String(stats.peakL, 4) + ", Peak R: " + juce::String(stats.peakR, 4));
    logInfo("RMS L: " + juce::String(stats.rmsL, 4) + ", RMS R: " + juce::String(stats.rmsR, 4));
    
    // Check for silence
    if (stats.peakL < 0.001f && stats.peakR < 0.001f)
    {
        logWarning("WARNING: Audio appears to be silent! Check plugin state.");
        return 1;
    }
    
    logInfo("");
    logInfo("=== Milestone A PASSED ===");
    logInfo("Next steps:");
    logInfo("1. Listen to: " + outputFile.getFullPathName());
    logInfo("2. Verify audible tone");
    logInfo("3. Proceed to Milestone B (state capture)");
    
    return 0;
}
