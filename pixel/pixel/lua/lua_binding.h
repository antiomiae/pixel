#ifndef PIXEL_LUA_H
#define PIXEL_LUA_H

#define SOL_CHECK_ARGUMENTS 1

#include <sol.hpp>
#include <pixel/pixel.h>
#include <memory>

namespace pixel
{

sol::table bind_pixel(sol::state& lua);
void bind_app(sol::state& lua, sol::table& binding, const string& type_name = "App");
void bind_glm(sol::state& lua, sol::table& binding, const string& module_name = "glm");
void bind_tile_map(sol::state& lua, sol::table& binding, const string& type_name = "TileMap");
void bind_tile_map_renderer(sol::state& lua, sol::table& binding, const string& type_name = "TileMapRenderer");
void bind_sprite_renderer(sol::state& lua, sol::table& binding, const string& type_name = "SpriteRenderer");
void bind_camera(sol::state& lua, sol::table& binding, const string& type_name = "Camera");
void bind_texture_atlas(sol::state& lua, sol::table& binding, const string& type_name = "TextureAtlas");
void bind_sprite_animation(sol::state& lua, sol::table& binding, const string& type_name = "SpriteAnimation");
void bind_sprite(sol::state& lua, sol::table& binding, const string& type_name = "Sprite");
void bind_sprite_batch(sol::state& lua, sol::table& binding, const string& type_name = "SpriteBatch");
void bind_texture_region(sol::state& lua, sol::table& binding, const string& type_name = "TextureRegion");

};

#endif //PIXEL_LUA_H
