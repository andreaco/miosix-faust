
#ifndef MIOSIX_AUDIO_AUDIO_BUFFER_H
#define MIOSIX_AUDIO_AUDIO_BUFFER_H

#include <array>

/**
 * This template class define a multi channel buffer that can be used to
 * store and process audio.
 *
 * @tparam T type stored in the buffer
 * @tparam CHANNEL_NUM number of channels of the buffer
 * @tparam BUFFER_LEN length of each channel of the buffer
 */
template<typename T, size_t CHANNEL_NUM, size_t BUFFER_LEN>
class AudioBuffer {
public:
    /**
     * Constructor, statically asserts that the length of the
     * AudioBuffer is even.
     */
    AudioBuffer() {
        static_assert((BUFFER_LEN % 2) == 0, "The AudioBuffer BUFFER_LEN must be even");
        clear();
    };

    /**
     * Returns the channel count.
     *
     * @return number of channels of the buffer
     */
    inline size_t getNumChannels() const { return bufferContainer.size(); };

    /**
     * Returns the length of the buffers (that is the same for
     * each channel).
     *
     * @return buffer length
     */
    inline size_t getBufferLength() const { return bufferContainer[0].size(); };

    /**
     * Returns a raw pointer to the data array of a certain channel
     * of the buffer. The access is read only.
     *
     * @param channelNumber target channel
     * @return read pointer to the channel data
     */
    inline const T *getReadPointer(const unsigned int channelNumber) const { return bufferContainer[channelNumber].data(); };

    /**
     * Returns a raw pointer to the data array of a certain channel
     * of the buffer. The pointer grants both read and write accesses.
     *
     * @param channelNumber target channel
     * @return write pointer to the channel data
     */
    inline T *getWritePointer(const unsigned int channelNumber) { return bufferContainer[channelNumber].data(); };

    /**
     * Returns the std::array that contains the channels data.
     *
     * @return array containing arrays of data
     */
    inline std::array <std::array<T, BUFFER_LEN>, CHANNEL_NUM> &getBufferContainer() const { return bufferContainer; };

    /**
     * Applies a constant gain to the AudioBuffer.
     *
     * @param gain multiplicative factor
     */
    void applyGain(float gain);

    /**
     * Sums a second AudioBuffer to this AudioBuffer.
     *
     * @param buffer AudioBuffer to sum to this instance
     */
    void add(const AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN> &buffer);

    /**
     * Performs a copy from another buffer of the same dimensions.
     *
     * @param audioBuffer buffer to copy from
     */
    void copyFrom(const AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN> &audioBuffer);

    /**
     * Copy from a mono buffer on a certain channel.
     *
     * @param audioBuffer target buffer to copy
     * @param channelNumber destination channel
     */
    void copyOnChannel(const AudioBuffer<T, 1, BUFFER_LEN> &audioBuffer, size_t channelNumber);

    /**
     * Clear the buffer by filling it with zeroes
     */
    void clear();

private:
    /**
     * Data structure containing the buffer data.
     */
    std::array <std::array<T, BUFFER_LEN>, CHANNEL_NUM> bufferContainer;

    /**
     * Disabling copy constructor.
     */
    AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN>(const AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN> &);

    /**
     * Disabling move operator.
     */
    AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN> &operator=(const AudioBuffer<T, CHANNEL_NUM, BUFFER_LEN> &);
};

#endif //MIOSIX_AUDIO_AUDIO_BUFFER_H
