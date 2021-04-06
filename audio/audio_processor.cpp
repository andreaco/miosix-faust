#include "drivers/common/audio.h"
#include "audio_processor.h"

AudioProcessor::AudioProcessor()
        : audioDriver(AudioDriver::getInstance()) {}
