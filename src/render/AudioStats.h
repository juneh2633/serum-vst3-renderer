#pragma once

#include <JuceHeader.h>

namespace serum {

/**
 * Online audio statistics computed during streaming
 */
struct AudioStats
{
    float peakL = 0.0f;
    float peakR = 0.0f;
    float rmsL = 0.0f;
    float rmsR = 0.0f;
    int64 totalSamples = 0;
    
    double sumSquaresL = 0.0;
    double sumSquaresR = 0.0;
    
    /**
     * Update statistics with a new block
     */
    void updateBlock(const juce::AudioBuffer<float>& block);
    
    /**
     * Finalize RMS calculation after all blocks processed
     */
    void finalize();
    
    /**
     * Reset statistics
     */
    void reset();
};

} // namespace serum
