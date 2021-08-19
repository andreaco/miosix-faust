
#include "midi.h"
#include "midiXparser.h"
#include "audio/audio_math.h"
#include <array>
#include <algorithm>
#include <functional>
#include <cmath>


static AudioMath::LookupTable<128> exp2LUT([](float x) { return std::pow(2.0, x); }, -5.75, 4.83333);

float Midi::midi2freq(uint8_t midiNote) {
    return exp2LUT((static_cast<float>(midiNote) - 69.0) / 12.0) * 440.0;
}

Midi::MidiParser::MidiParser() : channelMask(0xFFFF) {
    midiParser.setMidiMsgFilter(midiXparser::channelVoiceMsgTypeMsk);
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
