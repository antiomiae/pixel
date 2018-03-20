#include <algorithm>
#include <pixel/pixel.h>

namespace pixel
{
using namespace std;

class PropertyCollection
{
public:
    PropertyCollection(const vector<tmx::Property>& props)
    {
        for (const auto& prop : props) {
            props_[prop.getName()] = prop;
        }
    }

    float getFloat(const string& k)
    {
        auto prop = props_.at(k);
        error_unless(prop.getType() == tmx::Property::Type::Float,
                     "Property attempting to be read as float type is not float type");
        return prop.getFloatValue();
    }

    float getFloat(const string& k, float default_val)
    {

        auto item = props_.find(k);
        if (item != end(props_)) {
            auto prop = item->second;
            error_unless(prop.getType() == tmx::Property::Type::Float,
                         "Property attempting to be read as float type is not float type");
            return prop.getFloatValue();
        } else {
            return default_val;
        }

    }

private:
    unordered_map<string, tmx::Property> props_;
};

TileLayer::TileLayer(unsigned width, unsigned height)
    : width_(width),
      height_(height),
      tiles_(width * height)
{
}

bool TileLayer::load(const tmx::Map& m, const tmx::TileLayer& t, const pixel::Tileset& tileset)
{
    auto tile_count = m.getTileCount();

    if (width_ > 0 || height_ > 0) {
        argument_error_if(
            tile_count.x != width_ || tile_count.y != height_,
            "TileLayer was initialized with different map size than supplied tmx::Map"
        );

    } else {
        width_ = tile_count.x;
        height_ = tile_count.y;
        tiles_.resize(width_ * height_);
    }

    auto tmx_tiles = t.getTiles();
    error_if(tmx_tiles.size() != tiles_.size(), "tmx::TileLayer t contains different number of tiles than expected");

    PropertyCollection props(t.getProperties());

    parallax_ = {
        props.getFloat("parallax_x", 1.0),
        props.getFloat("parallax_y", 1.0)
    };

    transform(
        begin(tmx_tiles),
        end(tmx_tiles),
        begin(tiles_),
        [&](auto tmx_tile) {
            return Tile{
                tmx_tile.ID,
                tmx_tile.flipFlags
            };
        }
    );

    for (auto i = 0u; i < tiles_.size(); ++i) {
        auto& tile = tiles_[i];
        if (tileset.tile_has_animation(tile.tile_id)) {
            animations_[tile.tile_id].base_tile_id = tile.tile_id;
            animations_[tile.tile_id].animation_definition = tileset.tile(tile.tile_id).animation;
            animations_[tile.tile_id].tiles.push_back(i);
        }
    }

    return true;
}

const vector<TileLayer::Tile>& TileLayer::tiles() const
{
    return tiles_;
}


unordered_map<uint32_t, TileLayer::TileAnimation> TileLayer::animations()
{
    return animations_;
}


unsigned TileLayer::width() const
{
    return width_;
}


unsigned TileLayer::height() const
{
    return height_;
}


glm::vec2 TileLayer::parallax() const
{
    return parallax_;
}


void TileLayer::update(float dt)
{
    for (auto & [id, anim] : animations_) {
        anim.update(dt);

        if (!anim.tiles.empty()) {
            for (auto tile_idx : anim.tiles) {
                tiles_[tile_idx].tile_id = anim.tile_id();
            }
        }
    }
}

};
