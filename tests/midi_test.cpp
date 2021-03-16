#include "catch.hpp"
#include "midi_test_data.h"
#include "../midi/midi.h"
#include <algorithm>
#include <array>

TEST_CASE("Midi", "[midi]") {
    uint8_t *p;
    Midi::Parser midiParser;

    SECTION("note on test") {
        p = noteOnCh0;
        while (p != std::end(noteOnCh0)) midiParser.parse(p);
        REQUIRE(midiParser.getMidiMessage() == Midi::MidiMessage::NOTE_ON);
        REQUIRE(midiParser.getNote().getMidiNote() == 61);
        REQUIRE(midiParser.getNote().getVelocity() == 11);
        REQUIRE(midiParser.getDataBytes()[0] == midiParser.getNote().getMidiNote());
        REQUIRE(midiParser.getDataBytes()[1] == midiParser.getNote().getVelocity());

    }

    SECTION("note off test 1") {
        p = noteOffCh5;
        while (p != std::end(noteOffCh5)) midiParser.parse(p);
        REQUIRE(midiParser.getMidiMessage() == Midi::MidiMessage::NOTE_OFF);
        REQUIRE(midiParser.getNote().getMidiNote() == 61);
        REQUIRE(midiParser.getNote().getVelocity() == 0);
        REQUIRE(midiParser.getDataBytes()[0] == midiParser.getNote().getMidiNote());
        REQUIRE(midiParser.getDataBytes()[1] == midiParser.getNote().getVelocity());
    }

    SECTION("note off test 2") {
        p = noteOffCh8;
        while (p != std::end(noteOffCh8)) midiParser.parse(p);
        REQUIRE(midiParser.getMidiMessage() == Midi::MidiMessage::NOTE_OFF);
        REQUIRE(midiParser.getNote().getMidiNote() == 61);
        REQUIRE(midiParser.getNote().getVelocity() == 0);
        REQUIRE(midiParser.getDataBytes()[0] == midiParser.getNote().getMidiNote());
        REQUIRE(midiParser.getDataBytes()[1] == midiParser.getNote().getVelocity());
    }

    SECTION("control change test") {
        p = controlChangeCh7;
        while (p != std::end(controlChangeCh7)) midiParser.parse(p);
        REQUIRE(midiParser.getMidiMessage() == Midi::MidiMessage::CONTROL_CHANGE);
        REQUIRE(midiParser.getControllerNumber() == 1);
        REQUIRE(midiParser.getControllerValue() == 78);
        REQUIRE(midiParser.getDataBytes()[0] == midiParser.getControllerNumber());
        REQUIRE(midiParser.getDataBytes()[1] == midiParser.getControllerValue());
    }

    SECTION("pitch bend test") {
        p = pitchBendCh3;
        while (p != std::end(pitchBendCh3)) midiParser.parse(p);
        REQUIRE(midiParser.getMidiMessage() == Midi::MidiMessage::PITCH_BEND);
        REQUIRE(midiParser.getPitchBendValue() == 50);
        REQUIRE(midiParser.getDataBytes()[0] == 0);
        REQUIRE(midiParser.getDataBytes()[1] == 50);
    }

    SECTION("test masking") {
        p = pitchBendCh3;
        midiParser.maskChannel(3);
        while (p != std::end(pitchBendCh3)) midiParser.parse(p);
        REQUIRE(midiParser.getMidiMessage() == Midi::MidiMessage::MASKED);

    }
}