#include "catch.hpp"
#include "../containers/circular_buffer.h"

TEST_CASE("CircularBuffer", "[containers]") {

    SECTION("CircularBuffer STL style") {
        CircularBuffer<int, 4> buffer;

        SECTION("putting elements in the buffer") {
            buffer.push_back(1);
            REQUIRE(buffer.front() == 1);

            buffer.push_back(2);
            REQUIRE(buffer.front() == 1);


            SECTION("getting elements") {
                REQUIRE(buffer.front() == 1);
                buffer.pop_front();
                REQUIRE(buffer.front() == 2);
                buffer.pop_front();
                REQUIRE(buffer.empty() == true);
            }


            SECTION("checking isEmpty") {
                REQUIRE(buffer.empty() == false);

                // clearing the buffer
                buffer.clear();
                REQUIRE(buffer.empty() == true);
            }

            SECTION("checking the buffer length") {
                REQUIRE(buffer.max_size() == 4);
            }

            SECTION("checking the current capacity") {
                REQUIRE(buffer.size() == 2);
                buffer.push_back(45);
                REQUIRE(buffer.size() == 3);
            }
        }
    }

    SECTION("Overriding CL") {
        CircularBuffer<float, 4> buffer;
        SECTION("wrapping") {
            REQUIRE(buffer.empty() == true);

            buffer.push_back(1);
            buffer.push_back(2);
            buffer.push_back(2.5);


            buffer.pop_front();
            buffer.pop_front();
            buffer.push_back(3.5);

            REQUIRE(buffer.size() == 2);

            buffer.push_back(3.6);

            buffer.push_back(3.9);

            buffer.pop_front();
            buffer.pop_front();

            REQUIRE(buffer.front() == Approx(3.6));
            buffer.pop_front();
            buffer.clear();
            REQUIRE(buffer.empty() == true);
            REQUIRE(buffer.size() == 0);
        }

        SECTION("overriding") {
            buffer.push_back(1);
            buffer.push_back(2);
            buffer.push_back(2.5);
            buffer.push_back(3.5);
            buffer.push_back(4.5);
            REQUIRE(buffer.front() == Approx(2));
        }

    }

    SECTION("Discarding CircularBuffer test") {
        CircularBuffer<double, 4, CircularBufferType::Discard> buffer;
        SECTION("wrapping") {
            buffer.push_back(1);
            buffer.push_back(2);
            buffer.push_back(2.5);
            buffer.pop_front();
            buffer.pop_front();
            buffer.push_back(3.5);
            REQUIRE(buffer.size() == 2);
            buffer.push_back(3.6);
            buffer.push_back(3.9);
            buffer.pop_front();
            buffer.pop_front();
            REQUIRE(buffer.front() == 3.6);
            buffer.pop_front();
            REQUIRE(buffer.size() == 1);
            buffer.clear();
            REQUIRE(buffer.empty() == true);
            REQUIRE(buffer.size() == 0);
        }

        SECTION("overriding") {
            buffer.push_back(1);
            buffer.push_back(2);
            buffer.push_back(2.5);
            buffer.push_back(3.5);
            buffer.push_back(4.5);
            REQUIRE(buffer.front() == 1);
            buffer.pop_front();
            REQUIRE(buffer.size() == 3);
        }
    }
}
