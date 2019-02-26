#include "hook_stage.h"
#include "triangle_batch.h"
#include <iostream>
#include <vector>
#include <string>

const vector<pair<string,string>> SPRITES = {
    {"demos/hook/assets/body.png", "body"}
};

void HookStage::update()
{
    auto& renderer = this->renderer_group().get<pixel::renderers::TexturedTriangleRenderer>();

    TriangleBatch batch{};
    batch.add_sprite(body_sprite_);

    renderer.render(batch.triangles(), sprite_texture(), camera());
}

void HookStage::on_load()
{
    load_sprites(SPRITES);
}

void HookStage::on_enter()
{
    body_sprite_ = get_sprite("body");
}

