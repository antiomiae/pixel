#ifndef PIXEL_MAIN_HOOK_STAGE_H
#define PIXEL_MAIN_HOOK_STAGE_H

#include "stage.h"

class HookStage : public Stage
{
public:
    virtual void update() override;

    virtual void on_load() override;

    virtual void on_enter() override;

private:
    pixel::Sprite body_sprite_;
};


#endif //PIXEL_MAIN_HOOK_STAGE_H
