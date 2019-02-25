#include "game.h"
#include "stage.h"
#include <pixel/pixel.h>


void Game::add_stage(unique_ptr<Stage> s)
{
    if (!current_stage_) {
        current_stage_ = move(s);
        current_stage_->on_load();
        current_stage_->on_enter();
    } else {
        stages_.push_back(move(s));
    }
}

void Game::update()
{
    if (current_stage_) {
        current_stage_->update();
    }
}

Game::Game(int argc, char* argv[])
{
    glm::ivec2 virtual_window_size = glm::vec2{320, 240};
    glm::ivec2 actual_window_size = virtual_window_size * 2;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);
}

void Game::run()
{
    pixel::app().set_tick_callback(
        [&] {
            update();
        }
    );

    pixel::app().run();
}
