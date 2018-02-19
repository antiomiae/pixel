#include <pixel/pixel.h>
#include <unistd.h>

using namespace std;


int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
    }

    pixel::print_version_information();

    pixel::App app;
    app.init();

    auto render_context = app.render_context();
    render_context.set_base_scale_from_virtual_resolution({320, 224});

    pixel::TileMapRenderer renderer{};

    tmx::Map tmx_map;
    pixel::TileMap tile_map;

    error_if(!tmx_map.load("assets/map.tmx"), "Unable to load map file");
    tile_map.load(tmx_map);
    tile_map.atlas().debugSave("atlas");

    app.setTickCallback(
        [&] {
            renderer.render(tile_map, render_context);
        }
    );

    app.run();

    return 0;
}

