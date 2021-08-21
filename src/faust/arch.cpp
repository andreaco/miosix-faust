// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)

// needed by any faust arch file
#include "faust/misc.h"


#include "faust/gui/MidiUI.h"
#include "../../include/miosix-midi.h"

// allows to control a faust DSP module in a simple manner by using parameter's path
#include "faust/gui/MapUI.h"

// needed by any faust arch file
#include "faust/dsp/dsp.h"

// tags used by the faust compiler to paste the generated c++ code
<<includeIntrinsic>>
<<includeclass>>