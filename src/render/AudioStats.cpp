#include "render/AudioStats.h"
#include <cmath>

namespace serum {

void AudioStats::updateBlock(const juce::AudioBuffer<float>& block)
{
    int numChannels = block.getNumChannels();
    int numSamples = block.getNumSamples();
    
    if (numChannels == 0 || numSamples == 0)
        return;
    
    // Update left channel (or mono)
    const float* leftData = block.getReadPointer(0);
    for (int i = 0; i < numSamples; ++i)
    {
        float sample = std::abs(leftData[i]);
        peakL = std::max(peakL, sample);
        sumSquaresL += sample * sample;
    }
    
    // Update right channel if stereo
    if (numChannels >= 2)
    {
        const float* rightData = block.getReadPointer(1);
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = std::abs(rightData[i]);
            peakR = std::max(peakR, sample);
            sumSquaresR += sample * sample;
        }
    }
    else
    {
        // Mono - use same values for right
        peakR = peakL;
        sumSquaresR = sumSquaresL;
    }
    
    totalSamples += numSamples;
}

void AudioStats::finalize()
{
    if (totalSamples > 0)
    {
        rmsL = static_cast<float>(std::sqrt(sumSquaresL / totalSamples));
        rmsR = static_cast<float>(std::sqrt(sumSquaresR / totalSamples));
    }
}

void AudioStats::reset()
{
    peakL = peakR = 0.0f;
    rmsL = rmsR = 0.0f;
    sumSquaresL = sumSquaresR = 0.0;
    totalSamples = 0;
}

} // namespace serum
