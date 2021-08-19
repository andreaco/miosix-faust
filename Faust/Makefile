CC=g++

faustsynth: FaustSynth.dsp
	@faust -I /usr/local/share/faust/embedded/ -a arch.cpp -i -cn FaustSynth FaustSynth.dsp -o FaustSynth.h
	$(info Avaiable Parameters:)
	@$(CC) FaustParameterTest.cpp -o FaustParameterTest && ./FaustParameterTest && rm FaustParameterTest

.PHONY: clean

clean:
	@rm FaustSynth.h
