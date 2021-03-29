
#ifndef MIOSIX_AUDIO_MIDI_H
#define MIOSIX_AUDIO_MIDI_H

#include "midiXparser.h"
#include <array>
#include <list>
#include <queue>

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
    enum class MidiMessageType {
        NOTE_ON,
        NOTE_OFF,
        CONTROL_CHANGE,
        PITCH_BEND,
        MASKED, // for a message read from a masked channel
        NOT_SUPPORTED // TODO: implement all midi messages
    };

    class MidiMessage {
    public:
        MidiMessage(std::array<uint8_t, 2> data, MidiMessageType type) :
                dataBytes(data),
                midiType(type) {};


        MidiMessage() = delete;

//        MidiMessage(const MidiMessage &copy) :
//                dataBytes(copy.getDataBytes()),
//                midiType(copy.getType()) {};
//
//        MidiMessage &operator=(const MidiMessage &copy) {
//            dataBytes = copy.dataBytes;
//            midiType = copy.midiType;
//            return *this;
//        };

        inline MidiMessageType getType() const { return midiType; };

        inline std::array<uint8_t, 2> getDataBytes() const { return dataBytes; };

        Note getNote();

        uint16_t getPitchBendValue();

        inline uint8_t getControllerNumber() { return dataBytes[0]; };

        inline uint8_t getControllerValue() { return dataBytes[1]; };

    private:
        std::array<uint8_t, 2> dataBytes;
        MidiMessageType midiType;
    };

    /**
     * Midi parser, based on midiXparser class.
     */
    class MidiParser {
    public:

        /**
         * Constructor.
         */
        MidiParser();

        void parse(std::queue <uint8_t> &rawMidiQueue);

        MidiMessage popMidiMessage();

        inline std::queue <MidiMessage> getMessageQueue() { return messageQueue; };

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


    private:

        std::queue <MidiMessage> messageQueue;

        /**
         * Bitmask used to enable the channels.
         */
        uint16_t channelMask;

        /**
         * Instance of midiXparser, used to
         * separate the messages in the stream.
         */
        midiXparser midiParser;

    };
}

#endif //MIOSIX_AUDIO_MIDI_H
