#ifndef MICROAUDIO_MIDIPARSER_H
#define MICROAUDIO_MIDIPARSER_H

#include "miosix.h"
#include "../containers/circular_buffer.h"
#include "../config/hw_config.h"

/**
 * Definition of MIDI Standard Statuses
 */
#define NOTE_ON_MASK 0x90
#define NOTE_OFF_MASK 0x80
#define CC_MASK 0xB0

/**
 * Simple Struct representing a MIDI Note message
 */
struct MidiNote {
    enum NOTE_TYPE {
        NOTE_ON,
        NOTE_OFF
    };

    uint8_t channel;
    NOTE_TYPE msgType;
    uint8_t note;
    uint8_t velocity;
    uint8_t rawData[3];
};

/**
 * Simple Struct representing a MIDI Control Change message
 */
struct ControlChange {
    uint8_t channel;
    uint8_t controlNumber;
    uint8_t value;
    uint8_t rawData[3];
};

/**
 * MidiParser class
 * Each instance of this class has a Buffer for each type of supported
 * messages (Notes and CC as of now), when done parsing the message will be inserted
 * in its relative queue and can be obtained with the relative pop method
 */
class MidiParser {
public:
    /**
     * Constructor
     */
    MidiParser() : state(STATUS), lastState(STATUS) {}

    /**
     * Get the oldest note from the Note Message Buffer
     * and removes it from the buffer
     * @return Oldest note from the Note Message Buffer
     */
    MidiNote popNote();

    /**
     * Get the oldest Control Change from the CC Message Buffer
     * and removes it from the buffer
     * @return Oldest CC message from CC Message Buffer
     */
    ControlChange popCC();

    /**
     * Check whether a new note is available
     * @return true if there are notes in the Note Message Buffer, false otherwise
     */
    bool isNoteAvaiable();

    /**
     * Check whether a new CC is available
     * @return
     */
    bool isCCAvaiable();

    /**
     * Parse a byte at a time and construct the messages to be inserted
     * in their relative queue
     * TODO: Fault management when some byte is dropped
     * @param byte Byte extracted from the stream to be parsed
     */
    void parseByte(uint8_t byte);

private:
    /**
     * Circular Buffer containing the parsed MIDI notes
     */
    CircularBuffer<MidiNote, MIDI_NOTE_MESSAGE_QUEUE_SIZE> noteMessageBuffer;

    /**
     * Attribute containing the MidiNote being currently parsed
     */
    MidiNote currentNote;

    /**
     * Circular Buffer containing the parsed CC messages
     */
    CircularBuffer<ControlChange, CC_MESSAGE_QUEUE_SIZE> ccMessageBuffer;

    /**
     * Attributet containing the CC message being currently parsed
     */
    ControlChange currentCC;

    /**
     * Enum containing the possible parsing states
     */
    enum ParseState {
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

    /**
     * Last saved message for running status
     */
    ParseState lastState;

    /**
     * Buffer Mutex
     */
     miosix::Mutex bufferMutex;
};

#endif //MICROAUDIO_MIDIPARSER_H
