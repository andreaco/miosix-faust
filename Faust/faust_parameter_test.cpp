#include <iostream>
#include "faust_synth.h"

int main()
{
    FaustSynth synth;
    MapUI control;
    synth.buildUserInterface(&control);

    for(int i=0; i < control.getParamsCount(); i++)
	    std::cout << control.getParamAddress(i) << "\n";
}