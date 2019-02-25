#include <pixel/pixel.h>
#include <unistd.h>
#include <pixel/error.h>
#include <pixel/input/input.h>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;

void update_camera(Camera& camera, const RenderContext& rc)
{
    camera.set_window_size(glm::vec2(rc.virtual_window_size));
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

    glm::ivec2 virtual_window_size = {320, 224};
    glm::ivec2 actual_window_size = virtual_window_size * 3;

    pixel::init(actual_window_size, virtual_window_size);

    renderers::RendererGroup rg;
    auto& renderer = rg.get<renderers::TileMapRenderer>();

    tmx::Map tmx_map;
    error_if(!tmx_map.load(map_file), "Unable to load map file");

    auto tile_map = make_unique<pixel::TileMap>(
        glm::uvec2{tmx_map.getTileCount().x, tmx_map.getTileCount().y},
        glm::uvec2{tmx_map.getTileSize().x, tmx_map.getTileSize().y}
    );

    tile_map->load(tmx_map);

    Camera camera({0, 0}, {0, 0, 2000, 2000});

    camera.set_window_size(virtual_window_size);

    pixel::app().set_tick_callback(
        [&] {
            tile_map->update(1 / 60.0);

            renderer.render(*tile_map, camera);

        }
    );

    pixel::app().run();

    return 0;
}
