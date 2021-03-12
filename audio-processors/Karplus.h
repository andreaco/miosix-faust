#pragma once


class Karplus
{
public:
    void setFrequency(float frequency, float sampleRate) {
        buffer.resize((int)(float(sampleRate) / frequency));
        index = 0;
    }
    
    void setFeedback(float feedback) {
        this->feedback = feedback;
        
    }
    
    float process (float sample)
    {
 
        // low pass filter (average) some samples
        float value = sample + (buffer[index] + buffer[(index + 1) % buffer.size()]) * 0.5f * feedback;
        buffer[index] = value;
 
        // move to the next sample
        index = (index + 1) % buffer.size();
 
        // return the sample from the buffer
        return value;
    }
 
private:
    std::vector<float>  buffer;
    size_t              index;
    float               feedback;
};
