#ifndef PIXEL_MAIN_GAME_H
#define PIXEL_MAIN_GAME_H

#include <vector>
#include <memory>

using namespace std;

class Stage;

class Game
{
public:
    Game(int argc, char* argv[]);

    void add_stage(unique_ptr<Stage> s);
    void run();
private:
    vector<unique_ptr<Stage>> stages_;
    unique_ptr<Stage> current_stage_;

    void update();
};


#endif //PIXEL_MAIN_GAME_H
