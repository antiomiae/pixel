#include <pixel/pixel.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
    }

    pixel::print_version_information();

    pixel::App app;

    app.init();

    tmx::Map map;

    if(!map.load("assets/map.tmx")) {
        cerr << "Unable to open map file" << endl;
        return 1;
    };

    auto &layers = map.getLayers();

    const auto &tilesets = map.getTilesets();

    pixel::TileAtlas atlas {16, 16, 2560};

    for (auto &tileset : tilesets) {
        atlas.addTileset(tileset);
    }

    atlas.debugSave("atlas");

    return 0;
}

