#include <pixel/input/input.h>
#include <test/setup.h>

namespace
{

using namespace pixel::input;

TEST(CallbackStack, interface)
{
    using TestStack = CallbackStack<int>;

    TestStack stack{};

    auto canary = 0;
    auto parrot = 0;


    stack.push_callback([&](int i) -> auto {
        canary = i;
        return true;
    });

    EXPECT_EQ(true, stack.invoke(10));
    EXPECT_EQ(10, canary);


    stack.push_callback([&](auto i) {
        canary = 2 * i;
        return true;
    });

    EXPECT_EQ(true, stack.invoke(10));
    EXPECT_EQ(20, canary);

    stack.push_callback([&](auto i) {
        canary = 3 * i;
        parrot = canary;
        return false;
    });

    EXPECT_EQ(true, stack.invoke(10));
    EXPECT_EQ(20, canary);
    EXPECT_EQ(30, parrot);
}

}
