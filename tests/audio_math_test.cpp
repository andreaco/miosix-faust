#include "catch.hpp"
#include "../audio/audio_math.h"
#include <functional>

TEST_CASE("linearInterpolation", "[audio]") {
    float a = -10;
    float b = 10;

    SECTION("mix = 0") {
        REQUIRE(AudioMath::linearInterpolation(a, b, 0) == Approx(-10));
    }

    SECTION("mix = 0.5") {
        REQUIRE(AudioMath::linearInterpolation(a, b, 0.5) == Approx(0));
    }

    SECTION("mix = 1") {
        REQUIRE(AudioMath::linearInterpolation(a, b, 1) == Approx(10));
    }
}

TEST_CASE("linearMap", "[audio]") {
    float x = 5;
    float y;

    SECTION("mapping back to the original value") {
        y = AudioMath::linearMap(x, 0, 10, 30, 50);
        y = AudioMath::linearMap(y, 30, 50, 0, 10);
        REQUIRE(x == Approx(y));
    }

    SECTION("inverting a value") {
        y = AudioMath::linearMap(x, 0, 1, 0, -1);
        REQUIRE(x == Approx(-y));
    }
}

TEST_CASE("LookupTable", "[audio]") {
    float testValue;
    SECTION("sine LUT") {
        auto f = [](float x) -> float { return std::sin(x); };
        AudioMath::LookupTable<4096> sineLUT(f, 0, 2 * M_PI, AudioMath::LookupTableEdges::PERIODIC);

        SECTION("values inside the table") {
            testValue = GENERATE(0.0, M_PI, M_PI / 4);
            REQUIRE(sineLUT(testValue) - std::sin(testValue) < 0.0001);
        }

        SECTION("periodicity") {
            testValue = GENERATE(-M_PI, 0.0, M_PI);
            REQUIRE(sineLUT(testValue) - sineLUT(2 * M_PI + testValue) < 0.0001);
        }
    }

    SECTION("tanh LUT") {
        auto f = [](float x) -> float { return std::tanh(x); };
        AudioMath::LookupTable<4096> tanhLUT(f, -3, 3 * M_PI, AudioMath::LookupTableEdges::EXTENDED);

        SECTION("values inside the table") {
            testValue = GENERATE(-2.5, -1, 0, 1.2, 2.7);
            REQUIRE(tanhLUT(testValue) - std::tanh(testValue) < 0.0001);
        }
    }
}