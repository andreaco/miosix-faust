
#ifndef MIOSIX_AUDIO_CIRCULAR_BUFFER_H
#define MIOSIX_AUDIO_CIRCULAR_BUFFER_H

#include <array>



/**
 * This class abstract class implements the basic functionalities
 * of a circular buffer, the put method can be overridden for
 * different overflow policies.
 *
 * @tparam T type to hold, it must implement the copy and default constructors
 * @tparam BUFFER_LEN length of the buffer
 */
 // TODO: add iterator
template<typename T, size_t BUFFER_LEN>
class CircularBuffer {
public:

    /**
     * Constructor.
     */
    CircularBuffer() :
            head(0),
            tail(0),
            currentCapacity(0){};

    /**
     * This method must be implemented in a subclass
     * to put a new element in the buffer handling
     * a possible overflow.
     *
     * @param item new element
     */
    virtual inline void put(T item) = 0;

    /**
     * Gets the oldest element available from the buffer
     * increasing the head pointer.
     * Getting an element from an empty buffer
     * has an undefined behaviours.
     *
     * @return oldest element
     */
    inline T get() {
        T item = bufferContainer[head];
        if (isEmpty()) return item; // undefined return behaviour in this case
        head++;
        head %= BUFFER_LEN;
        currentCapacity--;
        return item;
    };

    /**
     * Gets the oldest element available from the buffer
     * without increasing the head pointer.
     *
     * @return oldest element
     */
    inline T front() { return bufferContainer[head]; }

    /**
     * Resets the state of the buffer.
     */
    inline void clear() {
        head = 0;
        tail = 0;
        currentCapacity = 0;
    };

    /**
     * Checks if the buffer is empty.
     *
     * @return true if the buffer is empty
     */
    bool isEmpty() const { return currentCapacity == 0; };

    /**
     * Checks if the buffer is full.
     *
     * @return true if the buffer is full
     */
    bool isFull() const {
        return currentCapacity == BUFFER_LEN;
    };

    /**
     * Gets the number of elements currently inserted
     * in the buffer.
     *
     * @return number of inserted elements
     */
    size_t getCurrentCapacity() const {
        return currentCapacity;
    };

    /**
     * Gets the overall size of the buffer.
     *
     * @return BUFFER_LEN template parameter
     */
    size_t size() const { return BUFFER_LEN; };

protected:
    std::array <T, BUFFER_LEN> bufferContainer;
    size_t head;
    size_t tail;
    size_t currentCapacity;

};

/**
 * Circular buffer implementation with an override policy
 * on overflow. If the put method is called when the buffer
 * is full, the oldest element is overridden.
 *
 * @tparam T type to hold, it must implement the copy constructor
 * @tparam BUFFER_LEN length of the buffer
 */
template<typename T, size_t BUFFER_LEN>
class OverridingCircularBuffer : public CircularBuffer<T, BUFFER_LEN> {
public:

    /**
     * Puts a new element in the buffer.
     * If the buffer overflows the new element
     * it overwrites the oldest element.
     *
     * @param item new element
     */
    inline void put(T item) override {
        typedef CircularBuffer<T, BUFFER_LEN> P; // parent namespace
        if (P::isFull()) {
            // overwriting the oldest element
            P::head++;
            P::head %= BUFFER_LEN;
            P::currentCapacity--;
        }
        P::bufferContainer[P::tail] = item;
        P::tail++;
        P::tail %= BUFFER_LEN;
        P::currentCapacity++;

    };
};

/**
 * Circular buffer implementation with discarding policy
 * on overflow. If the put method is called when the buffer
 * is full, the new element is discarded.
 *
 * @tparam T type to hold, it must implement the copy constructor
 * @tparam BUFFER_LEN length of the buffer
 */
template<typename T, size_t BUFFER_LEN>
class DiscardingCircularBuffer : public CircularBuffer<T, BUFFER_LEN> {
public:

    /**
     * Puts a new element in the buffer.
     * If the buffer overflows the new element
     * is discarded.
     *
     * @param item new element
     */
    inline void put(T item) override {
        typedef CircularBuffer<T, BUFFER_LEN> P; // parent namespace
        if (P::isFull()) return; // overflow
        P::bufferContainer[P::tail] = item;
        P::tail++;
        P::tail %= BUFFER_LEN;
        P::currentCapacity++;
    };
};


#endif //MIOSIX_AUDIO_CIRCULAR_BUFFER_H
