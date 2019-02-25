#include "hook_stage.h"
#include <iostream>

void HookStage::update()
{
}

void HookStage::on_load()
{
    load_sprites({
        "demos/hook/assets/body.png"
    });
}
