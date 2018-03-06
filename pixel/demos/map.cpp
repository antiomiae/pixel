#include <pixel/pixel.h>
#include <unistd.h>
#include <pixel/error.h>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;

void update_camera(Camera& camera, const RenderContext& rc)
{
    camera.set_window_size(glm::vec2(rc.window_size) / rc.pixel_scale);
}

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
        {512, 448},
        {0.1, 0.1, 0.8, 1.0},
        2
    };

    app.init();

    pixel::TileMapRenderer renderer{};

    tmx::Map tmx_map;
    error_if(!tmx_map.load(map_file), "Unable to load map file");

    auto tile_map = make_unique<pixel::TileMap>();
    tile_map->load(tmx_map);

    Camera camera({0, 0}, {0, 0, 2000, 2000});

    app.set_tick_callback(
        [&] {
            update_camera(camera, app.render_context());

            tile_map->update(1 / 60.0);

            renderer.render(*tile_map, camera);
        }
    );

    app.run();

    return 0;
}
