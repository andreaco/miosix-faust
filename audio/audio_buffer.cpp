
#include "miosix.h"
#include "audio_buffer.h"


template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
void AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>::applyGain(float gain) {
    for (uint32_t channelNumber = 0; channelNumber < getNumChannels(); channelNumber++) {
        // iterating for each channel
        T* channel = getWritePointer(channelNumber);
        for (uint32_t sampleIndex = 0; sampleIndex < getBufferLength(); sampleIndex++) {
            // applying the gain to each sample of the channel
            channel[sampleIndex] *= gain;
        }
    }
}

template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
void AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>::add(AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN> buffer) {
    for (uint32_t channelNumber = 0; channelNumber < getNumChannels(); channelNumber++) {
        // iterating for each channelBuffer1
        T* channelBuffer1 = getWritePointer(channelNumber);
        T* channelBuffer2 = buffer.getWritePointer(channelNumber);
        for (uint32_t sampleIndex = 0; sampleIndex < getBufferLength(); sampleIndex++) {
            // summing AudioBuffer2 on AudioBuffer1
            channelBuffer1[sampleIndex] += channelBuffer2[sampleIndex];
        }
    }
}