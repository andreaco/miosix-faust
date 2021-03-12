
#pragma once

#include <array>
#include <cmath>
#include <math.h> // sin
#include <JuceHeader.h>

class Oscillator {
public:
    Oscillator() { }
    
    void prepare()
    {
        auto size = waveTable.size()-1;
        
        for (auto i = 0; i < size; ++i)
        {
            waveTable[i] = sin(2 * M_PI * i / size);
        }
        waveTable[waveTable.size()-1] = waveTable[0];
        
        
        phase = 0;
    }
    
    void setFrequency (float frequency, float sampleRate)
    {
        auto tableSizeOverSampleRate = (float) tableSize / sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }
    
    
    
    inline float getNextSample() noexcept
    {
        auto index0 = (unsigned int) tableIndex;
        auto index1 = index0 + 1;

        auto frac = tableIndex - (float) index0;

        
        auto value0 = waveTable[index0];
        auto value1 = waveTable[index1];

        
        auto currentSample = value0 + frac * (value1 - value0);

        if ((tableIndex += tableDelta) > (float) tableSize)
            tableIndex -= (float) tableSize;

        return currentSample;
    }
    
    
    
private:
    
    float phase;
    
    std::array<float, 1025> waveTable;
    const int tableSize = 1024;
    
    float tableIndex = 0.0f, tableDelta = 0.0f;
    float freq;
};
