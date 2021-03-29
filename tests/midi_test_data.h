uint8_t noteTest[] = {
        //  || note on/off || midi note || velocity ||

        0x90, 61, 19, // CH0
        0x80, 61, 28,
        0x90, 30, 0x78,
        0x90, 30, 0x00,

        0x92, 61, 0x78, //  CH2
        0x82, 61, 0x78,
        0x92, 30, 0x78,
        0x92, 30, 0x00,
};

uint8_t noteOnDouble[] = {
        0x90, 61, 19,
        0x90, 60, 20
};

uint8_t noteOnCh0[] = {
        0x90, 61, 11,
};

uint8_t noteOffCh5[] = {
        0x85, 61, 0,
};

uint8_t noteOffCh8[] = {
        0x98, 61, 0,
};

uint8_t controlChangeCh7[] = {
        0xB7, 1, 78,
};

uint8_t pitchBendCh3[] = {
        0xE3, 0, 50,
};

uint8_t midiNotesValuesInAEolian[] = {
        57, 53, 77, 57, 48, 83, 77, 47
};