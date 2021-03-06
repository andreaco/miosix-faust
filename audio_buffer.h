//
// Created by mailf on 06/03/2021.
//

#ifndef MIOSIX_AUDIO_AUDIO_BUFFER_H
#define MIOSIX_AUDIO_AUDIO_BUFFER_H

#include <array>

template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
class AudioBuffer {
public:
    AudioBuffer() {};

    inline size_t getNumChannels() { return bufferContainer.size(); };

    inline size_t getBufferLength() { return bufferContainer[0].size(); };

    inline const T *getReadPointer(unsigned int channelNumber) { return bufferContainer[channelNumber].data(); };
    inline T *getWritePointer(unsigned int channelNumber) { return bufferContainer[channelNumber].data(); };

    void addBuffer(AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>) {}; // TODO: SIMD implementation
    void applyGain(float) {}; // TODO: SIMD implementation

private:
    std::array <std::array<T, BUFFER_LEN>, CHANNEL_NUM> bufferContainer;
};

#endif //MIOSIX_AUDIO_AUDIO_BUFFER_H
