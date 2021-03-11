
#ifndef MIOSIX_AUDIO_MIDI_H
#define MIOSIX_AUDIO_MIDI_H

#include "midiXparser.h"


class Midi {
public:
    static void testParser();

    static float midi2freq(uint8_t midiNote);
};

class MidiNote {
public:
    MidiNote(uint8_t midiNote, uint8_t velocity) : midiNote(midiNote), velocity(velocity) {};

    MidiNote(uint8_t *midiNoteMsg) : midiNote(midiNoteMsg[1]), velocity(midiNoteMsg[2]) {};

    inline uint8_t getNote() { return midiNote; };

    inline uint8_t getVelocity() { return velocity; };


private:
    uint8_t midiNote;
    uint8_t velocity;
};


#endif //MIOSIX_AUDIO_MIDI_H
