
#ifndef MIOSIX_AUDIO_MIDI_H
#define MIOSIX_AUDIO_MIDI_H

#include "midiXparser.h"
#include <array>
#include <list>
#include "../containers/circular_buffer.h"

/**
 * Maximum size of the MidiMessage buffer.
 */
#define BUFFER_MAX_SIZE 32

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
         * @param midiNote note number
         * @param velocity note intensity
         */
        Note(uint8_t midiNote = 48, uint8_t velocity = 120) : midiNote(midiNote), velocity(velocity) {};

        /**
         * Constructor.
         *
         * @param midiNoteMsg data bytes array
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
    enum class MidiMessageType {
        NOTE_ON,
        NOTE_OFF,
        CONTROL_CHANGE,
        PITCH_BEND,
        MASKED, // for a message read from a masked channel
        NOT_SUPPORTED // TODO: implement all midi messages
    };

    /**
     * Class used to represent a message of the MIDI protocol.
     */
    class MidiMessage {
    public:
        /**
         * Constructor.
         *
         * @param data message data bytes
         * @param type midi message type
         */
        MidiMessage(std::array<uint8_t, 2> data, MidiMessageType type) :
                dataBytes(data),
                midiType(type) {};

        /**
         * Default constructor.
         * A default message fallbacks to the NOT_SUPPORTED type.
         */
        MidiMessage() : dataBytes({0x0, 0x0}), midiType (MidiMessageType::NOT_SUPPORTED) {}

        /**
         * Getter for the type attribute.
         *
         * @return type attribute.
         */
        inline MidiMessageType getType() const { return midiType; };

        /**
         * Getter for the data bytes.
         * It can be used to interpret the data bytes externally.
         *
         * @return dataBytes attribute
         */
        inline std::array<uint8_t, 2> getDataBytes() const { return dataBytes; };

        /**
         * Gets the note of the midi message.
         * Use this getter only after verifying that the
         * type of the MidiMessage is a NOTE_ON or NOTE_OFF.
         *
         * @return the Note of the message
         */
        Note getNote();

        /**
         * Gets the pitch bend value of the message.
         * Use this getter only after verifying that the
         * type of the MidiMessage is a PITCH_BEND.
         *
         * @return pitch bend value
         */
        uint16_t getPitchBendValue();

        /**
         * Gets the controller number of the message.
         * Use this getter only after verifying that the
         * type of the MidiMessage is a CONTROL_CHANGE.
         *
         * @return controller number value
         */
        inline uint8_t getControllerNumber() { return dataBytes[0]; };

        /**
         * Gets the controller value of the message.
         * Use this getter only after verifying that the
         * type of the MidiMessage is a CONTROL_CHANGE.
         *
         * @return controller value
         */
        inline uint8_t getControllerValue() { return dataBytes[1]; };

    private:
        /**
         * Data bytes of the midi message.
         */
        std::array<uint8_t, 2> dataBytes;

        /**
         * Type of the midi message.
         */
        MidiMessageType midiType;
    };

    /**
     * A MidiParser parses a raw uint8_t Queue and stores the
     * parsed MidiMessages in an internal CircularBuffer.
     * The class it's designed to work with different Queue input types.
     */
    class MidiParser {
    public:

        /**
         * Constructor.
         */
        MidiParser();

        /**
         * Parses an input queue containing single bytes composing
         * multiple midi messages. The parsed messages are stored in an
         * internal CircularBuffer.
         *
         * @tparam Queue type of the queue
         * @param rawMidiQueue reference to the input raw midi queue
         */
        template<class Queue>
        void parse(Queue &rawMidiQueue);

        /**
         * Pop a midi message from the internal CircularBuffer.
         *
         * @return the oldest midi message in the CircularBuffer
         */
        MidiMessage popMidiMessage();

        /**
         * Getter for the messageQueue.
         *
         * @return the CircularBuffer midi messageQueue
         */
        inline CircularBuffer<MidiMessage, BUFFER_MAX_SIZE> getMessageQueue() { return messageQueue; };

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

        /**
         * Queue of midi messages implemented as a CircularBuffer.
         */
        CircularBuffer<MidiMessage, BUFFER_MAX_SIZE> messageQueue;

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

template<class Queue>
void Midi::MidiParser::parse(Queue &rawMidiQueue) {
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

#endif //MIOSIX_AUDIO_MIDI_H
