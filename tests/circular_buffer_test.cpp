#include "catch.hpp"
#include "../containers/circular_buffer.h"

TEST_CASE("CircularBuffer", "[containers]") {

    SECTION("CircularBuffer shared functionalities") {
        OverridingCircularBuffer<int, 4> buffer;

        SECTION("putting elements in the buffer") {
            buffer.put(1);
            REQUIRE(buffer.front() == 1);
            buffer.put(2);
            REQUIRE(buffer.front() == 1);

            SECTION("getting elements") {
                REQUIRE(buffer.get() == 1);
                REQUIRE(buffer.get() == 2);
                REQUIRE(buffer.isEmpty() == true);

            }

            SECTION("checking isFull") {
                OverridingCircularBuffer<int, 2> buffer2;
                REQUIRE(buffer2.isFull() == false);
                // filling the buffer
                buffer2.put(3);
                buffer2.put(4);
                REQUIRE(buffer2.isFull() == true);
            }

            SECTION("checking isEmpty") {
                REQUIRE(buffer.isEmpty() == false);

                // clearing the buffer
                buffer.clear();
                REQUIRE(buffer.isEmpty() == true);
            }

            SECTION("checking the buffer length") {
                REQUIRE(buffer.size() == 4);
            }

            SECTION("checking the current capacity") {
                REQUIRE(buffer.getCurrentCapacity() == 2);
                buffer.put(45);
                REQUIRE(buffer.getCurrentCapacity() == 3);
            }
        }
    }

    SECTION("OverridingCircularBuffer test") {
        OverridingCircularBuffer<float, 4> buffer;
        SECTION("wrapping") {
            REQUIRE(buffer.isEmpty() == true);
            buffer.put(1);
            buffer.put(2);
            buffer.put(2.5);
            buffer.get();
            buffer.get();
            buffer.put(3.5);
            REQUIRE(buffer.getCurrentCapacity() == 2);
            buffer.put(3.6);
            buffer.put(3.9);
            buffer.get();
            buffer.get();
            REQUIRE(buffer.get() == Approx(3.6));
            buffer.clear();
            REQUIRE(buffer.isEmpty() == true);
            REQUIRE(buffer.getCurrentCapacity() == 0);
        }

        SECTION("overriding") {
            buffer.put(1);
            buffer.put(2);
            buffer.put(2.5);
            buffer.put(3.5);
            buffer.put(4.5);
            REQUIRE(buffer.get() == Approx(2));
        }
    }

    SECTION("Discarding CircularBuffer test") {
        DiscardingCircularBuffer<double, 4> buffer;
        SECTION("wrapping") {
            buffer.put(1);
            buffer.put(2);
            buffer.put(2.5);
            buffer.get();
            buffer.get();
            buffer.put(3.5);
            REQUIRE(buffer.getCurrentCapacity() == 2);
            buffer.put(3.6);
            buffer.put(3.9);
            buffer.get();
            buffer.get();
            REQUIRE(buffer.get() == 3.6);
            REQUIRE(buffer.getCurrentCapacity() == 1);
            buffer.clear();
            REQUIRE(buffer.isEmpty() == true);
            REQUIRE(buffer.getCurrentCapacity() == 0);
        }

        SECTION("overriding") {
            buffer.put(1);
            buffer.put(2);
            buffer.put(2.5);
            buffer.put(3.5);
            buffer.put(4.5);
            REQUIRE(buffer.get() == 1);
            REQUIRE(buffer.getCurrentCapacity() == 3);
        }

    }


//    SECTION()
}
