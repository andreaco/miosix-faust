

#ifndef STM32_MONOSYNTH_AUDIO_PARAMETER_H
#define STM32_MONOSYNTH_AUDIO_PARAMETER_H

#define AUDIO_PARAMETER_DEFAULT_TRANSITION_TIME 0.2

/**
 * This template class can be used as a wrapper for a parameter of
 * an AudioModule or similar classes.
 * It allows to store the current and last values for a parameter in order
 * to provide a smooth interpolation during the processing.
 * @tparam T
 */
template<typename T>
class AudioParameter {
public:

    /**
     * Constructor with an initializer for the parameter value.
     *
     * @param value
     */
    AudioParameter(T value) : transitionTime(AUDIO_PARAMETER_DEFAULT_TRANSITION_TIME),
                              currentValue(value),
                              lastValue(value) {};

    inline T getValue() { return currentValue; };

    inline T getLastValue() { return lastValue; };

    inline float getTransitionTime() { return transitionTime; };

    inline void setValue(T newValue) {
        lastValue = currentValue;
        currentValue = newValue;
    };

private:
    /**
     * This value indicates the expected transition time
     * in seconds from lastValue to currentValue.
     */
    float transitionTime;

    /**
     * Current value of the AudioParameter.
     */
    T currentValue;

    /**
     * Value of the AudioParameter from the last call
     * of setValue().
     */
    T lastValue;
};

#endif //STM32_MONOSYNTH_AUDIO_PARAMETER_H
