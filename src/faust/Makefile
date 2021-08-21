CC=gcc

faustsynth: faust_synth.dsp
	faust -I ../../include/faust/embedded -a arch.cpp -i -cn FaustSynth faust_synth.dsp -o ../../include/faust/faust_synth.h
	$(info Avaiable Parameters:)
	@$(CC) -fPIC faust_parameter_test.cpp -o FaustParameterTest && ./FaustParameterTest && rm FaustParameterTest

.PHONY: clean
