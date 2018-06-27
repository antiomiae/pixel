#include <pixel/time/simple_timer.h>
#include <test/setup.h>

namespace
{

using namespace pixel::time;

TEST_CASE("SimpleTimer")
{
    SimpleTimer timer{};

    SECTION("update_float") {
        timer.update(1.0f);

        REQUIRE(timer.time().count() == Approx(1.0f));
    }


    SECTION("update_duration") {
        timer.update(1s);
        timer.update(1000ms);

        REQUIRE(timer.time() == float_seconds(2.0f));
    }

    SECTION("reset") {
        timer.update(1s);
        timer.reset();
        timer.update(1s);

        REQUIRE(timer.time() == 1s);
    }
}

};
