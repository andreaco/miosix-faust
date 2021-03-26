
#ifndef MIOSIX_AUDIO_MIDI_H
#define MIOSIX_AUDIO_MIDI_H

#include "midiXparser.h"
#include <array>

/**
 * This namespace defines classes and functions to handle
 * the midi protocol.
 */
namespace Midi {

    /**
     * Converts a midi note to a frequency.
     *
     * @param midiNote
     * @return converted frequency
     */
    float midi2freq(uint8_t midiNote);

    /**
     * Class used to represent a note.
     */
    class Note {
    public:
        /**
         * Constructor.
         *
         * @param midiNote
         * @param velocity
         */
        Note(uint8_t midiNote = 48, uint8_t velocity = 120) : midiNote(midiNote), velocity(velocity) {};

        /**
         * Constructor.
         *
         * @param midiNoteMsg
         */
        Note(uint8_t *midiNoteMsg) : midiNote(midiNoteMsg[1]), velocity(midiNoteMsg[2]) {};

        /**
         * Returns the midi note.
         *
         * @return midi note
         */
        inline uint8_t getMidiNote() { return midiNote; };

        /**
         * Returns the velocity of the note.
         *
         * @return velocity
         */
        inline uint8_t getVelocity() { return velocity; };

    private:
        /**
         * Numerical value of the note.
         */
        uint8_t midiNote;

        /**
         * Velocity of the note.
         */
        uint8_t velocity;
    };

    /**
     * Enumeration for the different midi messages.
     */
    enum class MidiMessage {
        NOTE_ON,
        NOTE_OFF,
        CONTROL_CHANGE,
        PITCH_BEND,
        MASKED, // for a message read from a masked channel
        NOT_SUPPORTED // TODO: implement all midi messages
    };

    /**
     * Midi parser, based on midiXparser class.
     * Parses one message at the time with the parse(message) function;
     * after a message is read, the getter methods returns its information.
     */
    class Parser {
    public:
        // TODO: remove it
        static void testParser() {};

        /**
         * Constructor.
         */
        Parser();

        /**
         * Parse a message from a pointer.
         * The pointer is increased to the first byte
         * of the next message.
         *
         * @param message pointer reference to a midi buffer
         * @return midi message type
         */
        MidiMessage parse(uint8_t *&message);

        /**
         * Masks a certain channel.
         * The parsing of a masked channel message
         * generates a MASKED MidiMessage.
         *
         * @param channelNumber
         */
        inline void maskChannel(uint8_t channelNumber) {
            channelMask &= ~(1 << channelNumber);
        }

        /**
         * Enable a channel.
         *
         * @param channelNumber
         */
        inline void enableChannel(uint8_t channelNumber) {
            channelMask |= 1 << channelNumber;
        }

        /**
         * Checks if a channel is enabled.
         *
         * @param channelNumber
         * @return true if the channel is enabled
         */
        inline bool channelIsEnabled(uint8_t channelNumber) {
            return channelMask & (1 << channelNumber);
        }

        /**
         * Getter for the midiMessage attribute.
         *
         * @return midi message
         */
        inline MidiMessage getMidiMessage() { return midiMessage; };

        /**
         * Getter for the note
         *
         * @return note
         */
        inline Note getNote() { return note; };

        inline uint16_t getPitchBendValue() { return pitchBendValue; };

        inline uint8_t getControllerNumber() { return controllerNumber; };

        inline uint8_t getControllerValue() { return controllerValue; };

        /**
         * Getter for the array dataByte
         * @return
         */
        inline std::array<uint8_t, 2> getDataBytes() { return dataBytes; };

    private:
        /**
         * Bitmask used to enable the channels.
         */
         // TODO: set in the constructor
        uint16_t channelMask;

        /**
         * Instance of midiXparser, used to
         * separate the messages in the stream.
         */
        midiXparser midiParser;

        /**
         * Last parsed note.
         */
        Note note;

        /**
         * Last pitch bend value.
         */
        uint16_t pitchBendValue;

        /**
         * Last controller number from control change.
         */
        uint8_t controllerNumber;

        /**
        * Last controller value from control change
        */
        uint8_t controllerValue;

        /**
         * Pointer to the last parsed message.
         */
        uint8_t *messagePtr;

        /**
         * Data bytes of the last parsed message.
         */
        std::array<uint8_t, 2> dataBytes;

        /**
         * Last midi message.
         */
        MidiMessage midiMessage;
    };
}

#endif //MIOSIX_AUDIO_MIDI_H
