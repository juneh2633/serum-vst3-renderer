#include "common/Hash.h"

namespace serum {

std::string computeSHA256(const void* data, size_t size)
{
    juce::SHA256 sha256(data, size);
    return sha256.toHexString().toStdString();
}

std::string computeSHA256FromFile(const juce::File& file)
{
    if (!file.existsAsFile())
        return "";
    
    juce::FileInputStream stream(file);
    if (!stream.openedOk())
        return "";
    
    juce::SHA256 sha256(stream);
    return sha256.toHexString().toStdString();
}

} // namespace serum
