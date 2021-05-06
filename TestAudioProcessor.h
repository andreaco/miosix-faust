#ifndef MIOSIX_DRUM_TESTAUDIOPROCESSOR_H
#define MIOSIX_DRUM_TESTAUDIOPROCESSOR_H

// testing an implementation of an AudioProcessor
class AudioProcessorTest : public AudioProcessor {
public:
    AudioProcessorTest(AudioDriver &audioDriver) :
            AudioProcessor(audioDriver),
            sineLUT([](float x) { return std::sin(x); }, 0, 2 * M_PI,
                    AudioMath::LookupTableEdges::PERIODIC) {}

    void process() override {
        auto &buffer = getBuffer();
        float *left = buffer.getWritePointer(0);
        float *right = buffer.getWritePointer(1);

        for (uint32_t i = 0; i < getBufferSize(); i += 1) {
            left[i] = 0.8 * sineLUT(phase);
            right[i] = left[i];
            phase += phaseDelta;
            if (phase >= 2 * M_PI) phase -= 2 * M_PI;
        }

    }

    AudioMath::LookupTable<128> sineLUT;

    float phase = 0;
    float phaseDelta = 440 * 2 * M_PI / 44100.0;
};

#endif //MIOSIX_DRUM_TESTAUDIOPROCESSOR_H
