#include <pixel/pixel.h>
#include <unistd.h>
#include <pixel/error.h>

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
        {1000, 1000},
        {0.1, 0.1, 0.8, 1.0},
        4
    };

    app.init();

    pixel::TileMapRenderer renderer{};

    tmx::Map tmx_map;
    error_if(!tmx_map.load(map_file), "Unable to load map file");

    auto tile_map = make_unique<pixel::TileMap>();
    tile_map->load(tmx_map);

    int i = 0;

    app.set_tick_callback(
        [&] {
            ++i;
            if (i == 200) {
                {
                    tmx::Map tmx_map;
                    error_if(!tmx_map.load("assets/traps_2.tmx"), "Unable to load map file");
                    /* replace map */
                    tile_map = make_unique<pixel::TileMap>();
                    tile_map->load(tmx_map);
                }
            }

            tile_map->update(1 / 60.0);

            auto rc = app.render_context();
            //rc.pixel_scale = 1;
            renderer.render(*tile_map, rc);
        }
    );

    app.run();

    return 0;
}
