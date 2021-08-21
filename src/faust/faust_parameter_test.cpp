#include <iostream>
#include "../../include/faust/faust_synth.h"

int main() {
    FaustSynth synth;
    MapUI control;
    synth.buildUserInterface(&control);

    miosix_midi_handler midi_handler;
    MidiUI midiUI(&midi_handler);
    synth.buildUserInterface(&midiUI);

    for (int i = 0; i < control.getParamsCount(); i++)
        std::cout << control.getParamAddress(i) << "\n";
}