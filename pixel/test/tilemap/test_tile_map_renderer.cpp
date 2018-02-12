#include "test/setup.h"

namespace
{

using pixel::TileMapRenderer;

TEST(TileMapRenderer, DefaultConstructor)
{
    pixel::TileMapRenderer r2;
};

TEST(TileMapRenderer, ConstructWithShader)
{
    pixel::TileMapRenderer r1{
        {
            "assets/shaders/tilemap.vert",
            "assets/shaders/tilemap.frag"
        }
    };
}

};

