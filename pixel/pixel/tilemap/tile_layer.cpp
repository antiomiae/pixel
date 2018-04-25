#include <algorithm>
#include <pixel/error.h>
#include "tile_map.h"

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


TileLayer::TileLayer(TileMap* parent)
    : parent_(parent),
      width_(parent->tile_count().x),
      height_(parent->tile_count().y)
{
    tiles_.resize(width_ * height_);
}


bool TileLayer::load(const tmx::TileLayer& t)
{
    auto tile_count = parent_->tile_count();

    if (width_ > 0 || height_ > 0) {
        argument_error_if(
            tile_count.x != width_ || tile_count.y != height_,
            "TileLayer was initialized with different map size than parent TileMap"
        );

    } else {
        width_ = tile_count.x;
        height_ = tile_count.y;
        tiles_.resize(width_ * height_);
    }

    auto& tileset = parent_->tileset();

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
                tiles_[tile_idx].tile_id = anim.current_tile_id();
            }
        }
    }
}

TileLayer::Tile& TileLayer::at(uint x, uint y)
{
    return tiles_.at(x + y * height_);
}

void TileLayer::TileAnimation::update(float dt)
{
    timer += dt * 1000;
    update_frame();
}

void TileLayer::TileAnimation::update_frame()
{
    while (timer >= animation_definition.frames[frame].duration) {
        timer -= animation_definition.frames[frame].duration;
        frame++;
        if (frame >= animation_definition.frames.size()) {
            frame = 0;
        }
    }
}

uint32_t TileLayer::TileAnimation::current_tile_id()
{
    return animation_definition.frames[frame].tile_id;
}

};
