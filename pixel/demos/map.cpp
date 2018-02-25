#include <pixel/pixel.h>
#include <unistd.h>

using namespace std;


int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
        argv = &argv[2];
        argc -= 2;
    }

    string map_file = "assets/map.tmx";

    if (argc >= 1) {
        map_file = argv[0];
        argv = &argv[1];
        argc -= 1;
    }

    pixel::print_version_information();

    pixel::App app{
        {10 * 8 * 5, 10 * 8 * 5},
        {1.0, 1.0, 1.0, 1.0},
        5.0
    };
    app.init();

    pixel::TileMapRenderer renderer{};

    tmx::Map tmx_map;
    pixel::TileMap tile_map;

    error_if(!tmx_map.load(map_file), "Unable to load map file");
    tile_map.load(tmx_map);
    tile_map.atlas().debug_save("atlas");

    app.set_tick_callback(
        [&] {
            auto rc = app.render_context();
            //rc.pixel_scale = 1;
            renderer.render(tile_map, rc);
        }
    );

    app.run();

    return 0;
}
