#ifndef MICROAUDIO_MIDIPARSER_H
#define MICROAUDIO_MIDIPARSER_H

#include "../containers/circular_buffer.h"

/**
 * Definition of MIDI Standard Statuses
 */
#define NOTE_ON_MASK 0x90
#define NOTE_OFF_MASK 0x80
#define CC_MASK 0xB0

/**
 * Simple Struct representing a MIDI Note message
 */
struct MidiNote
{
    enum NOTE_TYPE
    {
        NOTE_ON,
        NOTE_OFF
    };

    uint8_t channel;
    NOTE_TYPE msgType;
    uint8_t note;
    uint8_t velocity;
};

/**
 * Simple Struct representing a MIDI Control Change message
 */
struct ControlChange
{
    uint8_t channel;
    uint8_t controlNumber;
    uint8_t value;
};

/**
 * MidiParser class
 * Each instance of this class has a Buffer for each type of supported
 * messages (Notes and CC as of now), when done parsing the message will be inserted
 * in its relative queue and can be obtained with the relative pop method
 */
class MidiParser
{
public:
    /**
     * Constructor
     */
    MidiParser() : state(STATUS) {}

    /**
     * Get the oldest note from the Note Message Buffer
     * and removes it from the buffer
     * @return Oldest note from the Note Message Buffer
     */
    MidiNote popNote()
    {
        MidiNote note = noteMessageBuffer.front();
        noteMessageBuffer.pop();
        return note;
    }

    /**
     * Get the oldest Control Change from the CC Message Buffer
     * and removes it from the buffer
     * @return Oldest CC message from CC Message Buffer
     */
    ControlChange popCC()
    {
        ControlChange cc = ccMessageBuffer.front();
        ccMessageBuffer.pop();
        return cc;
    }

    /**
     * Check whether a new note is available
     * @return true if there are notes in the Note Message Buffer, false otherwise
     */
    bool isNoteAvaiable()
    {
        return !noteMessageBuffer.empty();
    }

    /**
     * Check whether a new CC is available
     * @return
     */
    bool isCCAvaiable()
    {
        return !ccMessageBuffer.empty();
    }

    /**
     * Parse a byte at a time and construct the messages to be inserted
     * in their relative queue
     * TODO: Fault management when some byte is dropped
     * TODO: Add Running Status parsing
     * @param byte Byte extracted from the stream to be parsed
     */
    void parseByte(uint8_t byte)
    {
        switch(state)
        {
            case STATUS:
            {
                uint8_t channel = byte & 0x0f;

                uint8_t message = byte & 0xf0;
                // Look for note on event on midi channel 0
                if (message == NOTE_ON_MASK)
                {
                    state = NOTE_DATA1;
                    currentNote.msgType = MidiNote::NOTE_ON;
                    currentNote.channel = channel;
                }
                // Look for a note off event on midi channel 0
                if (message == NOTE_OFF_MASK)
                {
                    state = NOTE_DATA1;
                    currentNote.msgType = MidiNote::NOTE_OFF;
                    currentNote.channel = channel;
                }

                if (message == CC_MASK)
                {
                    state = CC_DATA1;
                    currentCC.channel = channel;
                }
                break;
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
                ccMessageBuffer.push(currentCC);
            }
        }
    }

private:
    /**
     * Circular Buffer containing the parsed MIDI notes
     */
    CircularBuffer<MidiNote, 32> noteMessageBuffer;

    /**
     * Attribute containing the MidiNote being currently parsed
     */
    MidiNote currentNote;

    /**
     * Circular Buffer containing the parsed CC messages
     */
    CircularBuffer<ControlChange, 32> ccMessageBuffer;

    /**
     * Attributet containing the CC message being currently parsed
     */
    ControlChange currentCC;

    /**
     * Enum containing the possible parsing states
     */
    enum ParseState
    {
        STATUS = 0,
        NOTE_DATA1,
        NOTE_DATA2,
        CC_DATA1,
        CC_DATA2
    };

    /**
     * Current parsing state
     */
    ParseState state;
};

#endif //MICROAUDIO_MIDIPARSER_H
