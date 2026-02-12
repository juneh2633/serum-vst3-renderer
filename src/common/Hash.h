#pragma once

#include <JuceHeader.h>
#include <string>

namespace serum {

/**
 * Compute SHA256 hash from memory buffer
 * @param data Pointer to data
 * @param size Size of data in bytes
 * @return Hex string representation of SHA256 hash
 */
std::string computeSHA256(const void* data, size_t size);

/**
 * Compute SHA256 hash from file
 * @param file File to hash
 * @return Hex string representation of SHA256 hash, empty if file doesn't exist
 */
std::string computeSHA256FromFile(const juce::File& file);

} // namespace serum
