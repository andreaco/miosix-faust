//
// Created by mailf on 06/03/2021.
//

#ifndef MIOSIX_AUDIO_AUDIO_BUFFER_H
#define MIOSIX_AUDIO_AUDIO_BUFFER_H

#include <array>


template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
class AudioBuffer {
public:
    AudioBuffer() {
        static_assert((BUFFER_LEN % 2) == 0, "The AudioBuffer BUFFER_LEN must be even");
    };

    inline size_t getNumChannels() { return bufferContainer.size(); };

    inline size_t getBufferLength() { return bufferContainer[0].size(); };

    inline const T *getReadPointer(unsigned int channelNumber) { return bufferContainer[channelNumber].data(); };

    inline T *getWritePointer(unsigned int channelNumber) { return bufferContainer[channelNumber].data(); };
    
    void applyGain(float) {};

private:
    std::array <std::array<T, BUFFER_LEN>, CHANNEL_NUM> bufferContainer;
    AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>(const AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>&);
    AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN> &operator=(const AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>&);
};

// TODO: SIMD implementation
//namespace AudioBufferSIMD {
//    template<size_t CHANNEL_NUM, size_t BUFFER_LEN>
//    void add_uint16_t(AudioBuffer<uint16_t, CHANNEL_NUM, BUFFER_LEN> buffer1,
//                      AudioBuffer<uint16_t, CHANNEL_NUM, BUFFER_LEN> buffer2) {
//        for (int i = 0; i < CHANNEL_NUM; i++) { // iterating for each channel
//            // these pointers contain two uint16_t values
//            uint32_t *p1 = static_cast<uint32_t *>(buffer1.getWritePointer(i));
//            uint32_t *p2 = static_cast<uint32_t *>(buffer2.getWritePointer(i));
//
//            // uint32_t  [0:BUFFER_LEN/2-1]: one value = two uint16_t values
//            // uint16_t [0:BUFFER_LEN-1]
//            for (unsigned int j = 0; j < BUFFER_LEN / 2; j++) {
//                p1[j] = __QADD16(p1[j], p2[j]);
//            }
//        }
//    };
//}

#endif //MIOSIX_AUDIO_AUDIO_BUFFER_H
