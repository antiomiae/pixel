#include <unistd.h>
#include <boost/filesystem.hpp>
#include <pixel/pixel.h>
#include <pixel/lua/lua_binding.h>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        pixel_error("Missing required arguments");
    }

    chdir(argv[1]);

    cout << "cwd = " << boost::filesystem::current_path().string() << endl;

    sol::state lua;
    /* open all standard libraries */
    lua.open_libraries();
    pixel::bind_pixel(lua);

    /* Set package path */
    lua.script("package.path = package.path .. ';pixel/lua/?.lua'");

    boost::filesystem::path script_path(argv[2]);

    lua.script("package.path = package.path .. ';" + script_path.remove_filename().string() + "/?.lua'");

    lua.script_file(argv[2]);

    return 0;
}


