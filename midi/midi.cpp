
#include "midi.h"
#include "midiXparser.h"
#include "../audio/audio_math.h"
#include <array>
#include <algorithm>
#include <functional>
#include <cmath>

#define TEST_MESSAGE noteTest


// TODO: modify midi class

static AudioMath::LookupTable<128> exp2LUT([](float x) { return std::pow(2.0, x); }, -5.75, 4.83333);

float Midi::midi2freq(uint8_t midiNote) {
    return exp2LUT((static_cast<float>(midiNote) - 69.0) / 12.0) * 440.0;
}

Midi::Parser::Parser() : channelMask(0xFFFF) {
    midiParser.setMidiMsgFilter(midiXparser::channelVoiceMsgTypeMsk);
}

Midi::MidiMessage Midi::Parser::parse(uint8_t *&message) {
    bool messageReadComplete = false;
    uint8_t currentByte;
    uint8_t statusByte;
    midiMessage = MidiMessage::NOT_SUPPORTED; // default fallback

    while (!messageReadComplete) {
        currentByte = *message;
        message++;
        if (midiParser.parse(currentByte)) {
            messageReadComplete = true; // a message has been parsed
            messagePtr = midiParser.getMidiMsg();
            statusByte = messagePtr[0];
            dataBytes[0] = messagePtr[1];
            dataBytes[1] = messagePtr[2];

            // checking the channel
            if (!channelIsEnabled(statusByte & 0xf)) {
                // channel is masked, breaking out the while
                midiMessage = MidiMessage::MASKED;
                break;
            }

            // note message
            if (midiParser.isMidiStatus(midiXparser::noteOnStatus) ||
                midiParser.isMidiStatus(midiXparser::noteOffStatus)) {
                note = Note(messagePtr);

                if (midiParser.isMidiStatus(midiXparser::noteOffStatus) || // noteOn with 0 velocity is a noteOff
                    (midiParser.isMidiStatus(midiXparser::noteOnStatus) && note.getVelocity() == 0)) {
                    // note off
                    midiMessage = MidiMessage::NOTE_OFF;
                } else {
                    // note on
                    midiMessage = MidiMessage::NOTE_ON;
                }
                break;
            }

            // control change
            if (midiParser.isMidiStatus(midiXparser::controlChangeStatus)) {
                controllerNumber = dataBytes[0];
                controllerValue = dataBytes[1];
                midiMessage = MidiMessage::CONTROL_CHANGE;
                break;
            }

            // pitch bend
            if (midiParser.isMidiStatus(midiXparser::pitchBendStatus)) {
                pitchBendValue = (dataBytes[0] << 7) | dataBytes[1];
                midiMessage = MidiMessage::PITCH_BEND;
                break;
            }
        }
    }
    return midiMessage;
}