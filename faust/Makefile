CC=g++

faustsynth: faust_synth.dsp
	faust -I embedded -a arch.cpp -i -cn FaustSynth faust_synth.dsp -o faust_synth.h
	$(info Avaiable Parameters:)
	@$(CC) faust_parameter_test.cpp -o FaustParameterTest && ./FaustParameterTest && rm FaustParameterTest

.PHONY: clean
