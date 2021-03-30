
#include "drivers/stm32f407vg_discovery/audio.h"
#include "audio_processor.h"

AudioProcessor::AudioProcessor()
        : audioDriver(AudioDriver::getInstance()) {}
