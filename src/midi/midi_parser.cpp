#include "../../include/midi/midi_parser.h"

MidiNote MidiParser::popNote()
{
    MidiNote note = noteMessageBuffer.front();
    noteMessageBuffer.pop();
    return note;
}

ControlChange MidiParser::popCC()
{
    ControlChange cc = ccMessageBuffer.front();
    ccMessageBuffer.pop();
    return cc;
}

bool MidiParser::isNoteAvaiable()
{
    return !noteMessageBuffer.empty();
}

bool MidiParser::isCCAvaiable()
{
    return !ccMessageBuffer.empty();
}

void MidiParser::parseByte(uint8_t byte)
{
    uint8_t channel = byte & 0x0f;

    uint8_t message = byte & 0xf0;

    // MIDI Running status Check
    if (state == STATUS && message < NOTE_OFF_MASK)
    {
        if (lastState == NOTE_DATA2)
            state = NOTE_DATA1;
        if (lastState == CC_DATA2)
            state = CC_DATA1;
    }

    switch(state)
    {
        case STATUS:
        {
            // Look for note on event on midi channel 0
            if (message == NOTE_ON_MASK)
            {
                state = NOTE_DATA1;
                currentNote.msgType = MidiNote::NOTE_ON;
                currentNote.channel = channel;
                break;
            }
            // Look for a note off event on midi channel 0
            if (message == NOTE_OFF_MASK)
            {
                state = NOTE_DATA1;
                currentNote.msgType = MidiNote::NOTE_OFF;
                currentNote.channel = channel;
                break;
            }

            if (message == CC_MASK)
            {
                state = CC_DATA1;
                currentCC.channel = channel;
                break;
            }

        }
        case NOTE_DATA1:
        {
            currentNote.note = byte;
            state = NOTE_DATA2;
            break;

        }
        case NOTE_DATA2:
        {
            currentNote.velocity = byte;
            state = STATUS;
            lastState = NOTE_DATA2;
            noteMessageBuffer.push(currentNote);
            break;
        }
        case CC_DATA1:
        {
            currentCC.controlNumber = byte;
            state = CC_DATA2;
            break;
        }
        case CC_DATA2:
        {
            currentCC.value = byte;
            state = STATUS;
            lastState = CC_DATA2;
            ccMessageBuffer.push(currentCC);
        }
    }
}
