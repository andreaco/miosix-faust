
#include "miosix.h"
#include "audio_buffer.h"
#include <algorithm>


template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
void AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>::applyGain(float gain) {
    for (uint32_t channelNumber = 0; channelNumber < CHANNEL_NUM; channelNumber++) {
        // iterating for each channel
        T* channel = getWritePointer(channelNumber);
        for (uint32_t i = 0; i < BUFFER_LEN; i++) {
            // applying the gain to each sample of the channel
            channel[i] *= gain;
        }
    }
}

template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
void AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>::add(AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN> &buffer) {
    for (uint32_t channelNumber = 0; channelNumber < CHANNEL_NUM; channelNumber++) {
        // iterating for each channelBuffer1
        T* channelBuffer1 = getWritePointer(channelNumber);
        T* channelBuffer2 = buffer.getWritePointer(channelNumber);
        for (uint32_t sampleIndex = 0; sampleIndex < BUFFER_LEN; sampleIndex++) {
            // summing AudioBuffer2 on AudioBuffer1
            channelBuffer1[sampleIndex] += channelBuffer2[sampleIndex];
        }
    }
}

template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
void AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>::copyOnChannel(AudioBuffer<T, 1, BUFFER_LEN> &audioBuffer,
                                                            size_t channelNumber) {

    T* buffer1 = getWritePointer(channelNumber);
    const T* buffer2 = audioBuffer.getReadPointer(0);
    std::copy(buffer2, buffer2 + BUFFER_LEN, buffer1);
}

template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
void AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>::clear() {
    for (uint32_t channelNumber = 0; channelNumber < CHANNEL_NUM; channelNumber++) {
       bufferContainer[channelNumber].fill(0);
    }
}