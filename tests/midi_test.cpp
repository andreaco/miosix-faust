#include "catch.hpp"
#include "midi_test_data.h"
#include "../include/midi/midi_parser.h"
#include "../include/containers/circular_buffer.h"
#include <algorithm>
#include <array>
#include <queue>

// TODO: Test new parser
#if 0
TEST_CASE("Midi std::queue", "[midi]") {
    uint8_t *p;
    Midi::MidiParser midiParser;
    std::queue <uint8_t> inputQueue;

    SECTION("note on test") {
        p = noteOnCh0;
        while (p != std::end(noteOnCh0)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        REQUIRE(midiParser.getMessageQueue().size() == 1);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(midiParser.getMessageQueue().size() == 0);
        REQUIRE(message.getType() == Midi::MidiMessageType::NOTE_ON);
        REQUIRE(message.getNote().getMidiNote() == 61);
        REQUIRE(message.getNote().getVelocity() == 11);
        REQUIRE(message.getDataBytes()[0] == message.getNote().getMidiNote());
        REQUIRE(message.getDataBytes()[1] == message.getNote().getVelocity());

    }

    SECTION("note off test 1") {
        p = noteOffCh5;
        while (p != std::end(noteOffCh5)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        REQUIRE(midiParser.getMessageQueue().size() == 1);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::NOTE_OFF);
        REQUIRE(message.getNote().getMidiNote() == 61);
        REQUIRE(message.getNote().getVelocity() == 0);
        REQUIRE(message.getDataBytes()[0] == message.getNote().getMidiNote());
        REQUIRE(message.getDataBytes()[1] == message.getNote().getVelocity());
    }

    SECTION("note off test 2") {
        p = noteOffCh8;
        while (p != std::end(noteOffCh8)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::NOTE_OFF);
        REQUIRE(message.getNote().getMidiNote() == 61);
        REQUIRE(message.getNote().getVelocity() == 0);
        REQUIRE(message.getDataBytes()[0] == message.getNote().getMidiNote());
        REQUIRE(message.getDataBytes()[1] == message.getNote().getVelocity());
    }

    SECTION("control change test") {
        p = controlChangeCh7;
        while (p != std::end(controlChangeCh7)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::CONTROL_CHANGE);
        REQUIRE(message.getControllerNumber() == 1);
        REQUIRE(message.getControllerValue() == 78);
        REQUIRE(message.getDataBytes()[0] == message.getControllerNumber());
        REQUIRE(message.getDataBytes()[1] == message.getControllerValue());
    }

    SECTION("pitch bend test") {
        p = pitchBendCh3;
        while (p != std::end(pitchBendCh3)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::PITCH_BEND);
        REQUIRE(message.getPitchBendValue() == 50);
        REQUIRE(message.getDataBytes()[0] == 0);
        REQUIRE(message.getDataBytes()[1] == 50);
    }

    SECTION("test masking") {
        p = pitchBendCh3;
        while (p != std::end(pitchBendCh3)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.maskChannel(3);
        midiParser.parse(inputQueue);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::MASKED);
    }

    SECTION("multiple notes") {
        p = noteTest;
        while (p != std::end(noteTest)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        REQUIRE(midiParser.getMessageQueue().size() == 8);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::NOTE_ON);
        REQUIRE(message.getNote().getMidiNote() == 61);
        REQUIRE(message.getNote().getVelocity() == 19);
        REQUIRE(midiParser.getMessageQueue().size() == 7);
        Midi::MidiMessage message2 = midiParser.popMidiMessage();
        REQUIRE(message2.getType() == Midi::MidiMessageType::NOTE_OFF);
        REQUIRE(message2.getNote().getMidiNote() == 61);
        REQUIRE(message2.getNote().getVelocity() == 28);
        REQUIRE(midiParser.getMessageQueue().size() == 6);

    }
}

TEST_CASE("Midi CircularBuffer", "[midi]") {
    uint8_t *p;
    Midi::MidiParser midiParser;
    CircularBuffer<uint8_t, 32> inputQueue;

    SECTION("note on test") {
        p = noteOnCh0;
        while (p != std::end(noteOnCh0)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        REQUIRE(midiParser.getMessageQueue().size() == 1);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(midiParser.getMessageQueue().size() == 0);
        REQUIRE(message.getType() == Midi::MidiMessageType::NOTE_ON);
        REQUIRE(message.getNote().getMidiNote() == 61);
        REQUIRE(message.getNote().getVelocity() == 11);
        REQUIRE(message.getDataBytes()[0] == message.getNote().getMidiNote());
        REQUIRE(message.getDataBytes()[1] == message.getNote().getVelocity());

    }

    SECTION("note off test 1") {
        p = noteOffCh5;
        while (p != std::end(noteOffCh5)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        REQUIRE(midiParser.getMessageQueue().size() == 1);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::NOTE_OFF);
        REQUIRE(message.getNote().getMidiNote() == 61);
        REQUIRE(message.getNote().getVelocity() == 0);
        REQUIRE(message.getDataBytes()[0] == message.getNote().getMidiNote());
        REQUIRE(message.getDataBytes()[1] == message.getNote().getVelocity());
    }

    SECTION("note off test 2") {
        p = noteOffCh8;
        while (p != std::end(noteOffCh8)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::NOTE_OFF);
        REQUIRE(message.getNote().getMidiNote() == 61);
        REQUIRE(message.getNote().getVelocity() == 0);
        REQUIRE(message.getDataBytes()[0] == message.getNote().getMidiNote());
        REQUIRE(message.getDataBytes()[1] == message.getNote().getVelocity());
    }

    SECTION("control change test") {
        p = controlChangeCh7;
        while (p != std::end(controlChangeCh7)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::CONTROL_CHANGE);
        REQUIRE(message.getControllerNumber() == 1);
        REQUIRE(message.getControllerValue() == 78);
        REQUIRE(message.getDataBytes()[0] == message.getControllerNumber());
        REQUIRE(message.getDataBytes()[1] == message.getControllerValue());
    }

    SECTION("pitch bend test") {
        p = pitchBendCh3;
        while (p != std::end(pitchBendCh3)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::PITCH_BEND);
        REQUIRE(message.getPitchBendValue() == 50);
        REQUIRE(message.getDataBytes()[0] == 0);
        REQUIRE(message.getDataBytes()[1] == 50);
    }

    SECTION("test masking") {
        p = pitchBendCh3;
        while (p != std::end(pitchBendCh3)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.maskChannel(3);
        midiParser.parse(inputQueue);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::MASKED);
    }

    SECTION("multiple notes") {
        p = noteTest;
        while (p != std::end(noteTest)) {
            inputQueue.push(*p);
            p++;
        }
        midiParser.parse(inputQueue);
        REQUIRE(midiParser.getMessageQueue().size() == 8);
        Midi::MidiMessage message(midiParser.popMidiMessage());
        REQUIRE(message.getType() == Midi::MidiMessageType::NOTE_ON);
        REQUIRE(message.getNote().getMidiNote() == 61);
        REQUIRE(message.getNote().getVelocity() == 19);
        REQUIRE(midiParser.getMessageQueue().size() == 7);
        Midi::MidiMessage message2 = midiParser.popMidiMessage();
        REQUIRE(message2.getType() == Midi::MidiMessageType::NOTE_OFF);
        REQUIRE(message2.getNote().getMidiNote() == 61);
        REQUIRE(message2.getNote().getVelocity() == 28);
        REQUIRE(midiParser.getMessageQueue().size() == 6);

    }
}

#endif