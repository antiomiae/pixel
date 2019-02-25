#include <pixel/pixel.h>
#include <pixel/level.h>
#include "src/game.h"
#include "src/hook_stage.h"

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;

int main(int argc, char* argv[])
{
    Game game(argc, argv);
    game.add_stage(make_unique<HookStage>());
    game.run();

    return 0;
}
