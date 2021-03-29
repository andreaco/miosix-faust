
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

Midi::MidiParser::MidiParser() : channelMask(0xFFFF) {
    midiParser.setMidiMsgFilter(midiXparser::channelVoiceMsgTypeMsk);
}

void Midi::MidiParser::parse(std::queue <uint8_t> &rawMidiQueue) {
    uint8_t currentByte;
    uint8_t statusByte;
    std::array<uint8_t, 2> dataBytes;
    uint8_t *messagePtr;
    Midi::MidiMessageType midiMessageType = Midi::MidiMessageType::NOT_SUPPORTED; // default fallback

    while (rawMidiQueue.empty() == false) {
        currentByte = rawMidiQueue.front(); // input read
        rawMidiQueue.pop();
        if (midiParser.parse(currentByte)) {
            messagePtr = midiParser.getMidiMsg();
            statusByte = messagePtr[0];
            dataBytes[0] = messagePtr[1];
            dataBytes[1] = messagePtr[2];

            // checking the channel first
            if (!channelIsEnabled(statusByte & 0xf)) {
                // channel is masked, breaking out the while
                midiMessageType = Midi::MidiMessageType::MASKED;

                // note message -------
            } else if (midiParser.isMidiStatus(midiXparser::noteOnStatus) ||
                       midiParser.isMidiStatus(midiXparser::noteOffStatus)) {
                Note note(dataBytes[0], dataBytes[1]);
                if (midiParser.isMidiStatus(midiXparser::noteOffStatus) || // noteOn with 0 velocity is a noteOff
                    (midiParser.isMidiStatus(midiXparser::noteOnStatus) && note.getVelocity() == 0)) {
                    // note off
                    midiMessageType = Midi::MidiMessageType::NOTE_OFF;
                } else {
                    // note on
                    midiMessageType = Midi::MidiMessageType::NOTE_ON;
                }

                // control change -------
            } else if (midiParser.isMidiStatus(midiXparser::controlChangeStatus)) {
                midiMessageType = Midi::MidiMessageType::CONTROL_CHANGE;

                // pitch bend -------
            } else if (midiParser.isMidiStatus(midiXparser::pitchBendStatus)) {
                midiMessageType = Midi::MidiMessageType::PITCH_BEND;

            } // end of message parsing

            // constructing the new MidiMessage
            Midi::MidiMessage midiMessage = {dataBytes, midiMessageType};
            messageQueue.push(midiMessage); // the element is pushed into the queue
        }
    }

}


Midi::Note Midi::MidiMessage::getNote() {
    return Midi::Note(dataBytes[0], dataBytes[1]);
}

uint16_t Midi::MidiMessage::getPitchBendValue() {
    return (dataBytes[0] << 7) | dataBytes[1];
}

Midi::MidiMessage Midi::MidiParser::popMidiMessage() {
    Midi::MidiMessage message = messageQueue.front();
    messageQueue.pop();
    return message;
}
