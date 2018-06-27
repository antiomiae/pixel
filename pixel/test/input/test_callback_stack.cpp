#include <pixel/input/input.h>
#include <test/setup.h>

namespace
{

using namespace pixel::input;

TEST_CASE("CallbackStack")
{
    using TestStack = CallbackStack<int>;

    TestStack stack{};

    auto canary = 0;
    auto parrot = 0;


    stack.push_callback([&](int i) -> auto {
        canary = i;
        return true;
    });

    REQUIRE(stack.invoke(10));
    REQUIRE(10 == canary);


    stack.push_callback([&](auto i) {
        canary = 2 * i;
        return true;
    });

    REQUIRE(stack.invoke(10));
    REQUIRE(20 == canary);

    stack.push_callback([&](auto i) {
        canary = 3 * i;
        parrot = canary;
        return false;
    });

    REQUIRE(stack.invoke(10));
    REQUIRE(20 == canary);
    REQUIRE(30 == parrot);
}

}
