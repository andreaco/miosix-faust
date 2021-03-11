
#include "midi.h"
#include "midiXparser.h"
#include "../tests/midi_test_data.h"
#include "audio_math.h"
#include <array>
#include <algorithm>
#include <functional>
#include <cmath>

#define TEST_MESSAGE noteTest

static std::function<float(float)> pow2 = [](float x) { return std::pow(2.0, x); };
static AudioMath::LookupTable<128> exp2LUT(pow2, 0, 127);

float Midi::midi2freq(uint8_t midiNote) {
    return exp2LUT((midiNote - 69.0) / 12.0) * 440.0;
}

void Midi::testParser() {
    midiXparser midiParser;
    uint8_t msg;
    uint8_t len;
    uint8_t type;
    uint8_t note;
    uint8_t velocity;
    int tmp = 0;
    float frequency;

    std::array<uint8_t, 16> readBuffer;
    uint8_t *readPtr;
    size_t msgLen = sizeof(TEST_MESSAGE) / sizeof(*TEST_MESSAGE);

    midiParser.setMidiMsgFilter(midiXparser::channelVoiceMsgTypeMsk);
    for (size_t i = 0; i < msgLen; i++) {
        msg = TEST_MESSAGE[i];
        if (midiParser.parse(msg)) {
            len = midiParser.getMidiMsgLen();
            type = midiParser.getMidiMsgType();
            readPtr = midiParser.getMidiMsg();
            std::copy(readPtr, readPtr + len, std::begin(readBuffer));

            // note message
            if (midiParser.isMidiStatus(midiXparser::noteOnStatus) ||
                midiParser.isMidiStatus(midiXparser::noteOffStatus)) {

                note = readPtr[1];
                velocity = readPtr[2];
                MidiNote midiNote(readPtr);
                frequency = Midi::midi2freq(midiNote.getNote());

                if (midiParser.isMidiStatus(midiXparser::noteOffStatus) || // noteOn with 0 velocity is a noteOff
                     (midiParser.isMidiStatus(midiXparser::noteOnStatus) && midiNote.getVelocity() == 0)) {
                    // note off
                    tmp--;
                } else {
                    // note on
                    tmp++;
                }

            }

            // note off
        }
    }
}