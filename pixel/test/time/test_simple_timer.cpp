#include <pixel/time/simple_timer.h>
#include <test/setup.h>

namespace
{

using namespace pixel::time;

class SimpleTimerTest : public ::testing::Test
{
public:

};

TEST_F(SimpleTimerTest, update_float)
{
    SimpleTimer timer{};

    timer.update(1.0f);

    EXPECT_FLOAT_EQ(timer.time().count(), 1.0f);
}


TEST_F(SimpleTimerTest, update_duration)
{
    SimpleTimer timer{};

    timer.update(1s);

    timer.update(1000ms);

    EXPECT_EQ(timer.time(), float_seconds(2.0f));
}

TEST_F(SimpleTimerTest, reset)
{
    SimpleTimer timer{};

    timer.update(1s);
    timer.reset();
    timer.update(1s);

    EXPECT_EQ(timer.time(), 1s);
}


}
