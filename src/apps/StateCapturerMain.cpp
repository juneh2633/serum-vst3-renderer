/*
    StateCapturer - GUI application for capturing Serum2 plugin state
    Milestone B implementation (placeholder for now)
*/

#include <JuceHeader.h>
#include "common/Log.h"

class StateCapturerApplication : public juce::JUCEApplication
{
public:
    StateCapturerApplication() {}
    
    const juce::String getApplicationName() override { return "Serum State Capturer"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    
    void initialise(const juce::String& commandLine) override
    {
        serum::logInfo("StateCapturer starting (Milestone B - not yet implemented)");
        serum::logInfo("Will be implemented after Milestone A validation");
        quit();
    }
    
    void shutdown() override {}
    void systemRequestedQuit() override { quit(); }
};

START_JUCE_APPLICATION(StateCapturerApplication)
