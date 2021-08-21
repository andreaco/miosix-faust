/************************** BEGIN miosix-midi.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef MIOSIX_DRUM_MIOSIX_MIDI_H
#define MIOSIX_DRUM_MIOSIX_MIDI_H

#include "midi.h"
#include "midi/midi_parser.h"

class MapUI;

/**
 *  MIDI input/output handling using miosix RTOS: https://miosix.org/
 */
class miosix_midi_handler : public midi_handler {
protected:
    void decodeMessage(MidiNote &midiNote)
    {
        const uint8_t *data = midiNote.rawData;
        int channel = midiNote.channel; // which MIDI channel, 0-15
        if (midiNote.msgType == MidiNote::NOTE_OFF) {
            handleKeyOff(0, channel, data[1], data[2]);
        } else if (midiNote.msgType == MidiNote::NOTE_ON) {
            handleKeyOn(0, channel, data[1], data[2]);
        }
    }
public:

    miosix_midi_handler() : midi_handler("miosix") {}

    virtual ~miosix_midi_handler() {}
};

class miosix_midi : public miosix_midi_handler {

public:
    void handleIncomingMidiMessage(MidiNote &message) {
        decodeMessage(message);
    }

public:

    miosix_midi() : miosix_midi_handler() {}

};

#endif //MIOSIX_DRUM_MIOSIX_MIDI_H

/**************************  END  miosix-midi.h **************************/