

#ifndef PIXEL_MAIN_GAME_OBJECT_H
#define PIXEL_MAIN_GAME_OBJECT_H

#include "node.h"

namespace pixel
{

class GameObject : public Node<GameObject>
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void init();
    virtual void update(float dt);

protected:
    void update_children(float dt);

};

};

#endif //PIXEL_MAIN_GAME_OBJECT_H
