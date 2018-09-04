#include "game_object.h"

void pixel::GameObject::update_children(float dt)
{
    for (const auto& child_ptr : children()) {
        child_ptr->update(dt);
    }
}

void pixel::GameObject::update(float dt)
{
    update_children(dt);
}

void pixel::GameObject::init()
{
}
