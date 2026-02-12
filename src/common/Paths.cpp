#include "common/Paths.h"

namespace serum {

static juce::File getDataDir()
{
    return juce::File::getCurrentWorkingDirectory().getChildFile("data");
}

juce::File getPresetStatesDir()
{
    return getDataDir().getChildFile("preset_states");
}

juce::File getMidisDir()
{
    return getDataDir().getChildFile("midis");
}

juce::File getOutputWavDir()
{
    return getDataDir().getChildFile("outwav");
}

juce::File getOutputMetaDir()
{
    return getDataDir().getChildFile("outmeta");
}

bool ensureDirectoryExists(const juce::File& dir)
{
    if (dir.isDirectory())
        return true;
    
    return dir.createDirectory().wasOk();
}

} // namespace serum
