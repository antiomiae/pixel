#include <unistd.h>
#include <pixel/pixel.h>
#include <pixel/lua/lua_binding.h>

using namespace std;


int main(int argc, char* argv[])
{
    if (argc >= 2) {
        chdir(argv[1]);
    }

    sol::state lua;
    lua.open_libraries();
    pixel::bind_pixel(lua);
    lua.script("package.path = package.path .. ';pixel/lua/?.lua'");
    lua.script_file("demos/lua.lua");

    return 0;
}
