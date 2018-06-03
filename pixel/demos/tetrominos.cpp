#include <pixel/pixel.h>
#include <array>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;


namespace tetrominos
{

class Level;
class Board;
class Tetromino;


class Board
{
public:
    static const int BOARD_WIDTH = 10;
    static const int BOARD_HEIGHT = 20;

    struct Piece
    {
        int color{0};
    };

    using PieceArray = array<array<Piece, BOARD_WIDTH>, BOARD_HEIGHT>;

private:

     PieceArray pieces_;
};


class Tetromino
{
public:
    Tetromino() = default;

    Tetromino(const vector<vector<Board::Piece>>& shape, int color)
        :color_(color)
    {
        set_shape(shape);
    }

    Tetromino(const vector<vector<int>>& shape, int color)
        :color_(color)
    {
        set_shape(shape);
    }

    vector<vector<Board::Piece>>& pieces()
    {
        return pieces_;
    }

    int color() {
        return color_;
    }

    void set_color(int new_color)
    {
        color_ = new_color;
    }

    void set_shape(const vector<vector<Board::Piece>>& shape)
    {
        pieces_ = shape;
    }

    void set_shape(const vector<vector<int>>& shape)
    {
        assert(!shape.empty());
        assert(!shape[0].empty());
        pieces_.resize(shape.size(), vector<Board::Piece>{shape[0].size()});

        for (auto row = 0u; row < shape.size(); ++row) {
            for (auto col = 0u; col < shape[0].size(); ++col) {
                if (shape[row][col] > 0) {
                    pieces_[row][col].color = color_;
                }
            }
        }
    }

private:
    int color_;
    vector<vector<Board::Piece>> pieces_;
};

class Level
{
public:
    void update()
    {

    }

    void render()
    {

    }

private:
    pixel::graphics::SpriteBatch sprite_batch_;
    pixel::graphics::renderers::SpriteRenderer sprite_renderer_;
    pixel::graphics::TextureAtlas texture_atlas_;

    Board board_;
    Tetromino current_piece_;
};


}


int main(int argc, char* argv[])
{
    glm::ivec2 virtual_window_size = {320, 224};
    glm::ivec2 actual_window_size = virtual_window_size * 3;

    pixel::init(actual_window_size, virtual_window_size, argc, argv);

    Camera camera({0, 0}, {0, 0, 2000, 2000});

    OffscreenRenderTarget render_target{};

    render_target.set_window_size(virtual_window_size);
    camera.set_window_size(render_target.window_size());

    tetrominos::Level level{};

    pixel::app().set_tick_callback(
        [&] {
            level.update();

            render_target.activate();

            glClear(GL_COLOR_BUFFER_BIT);

            level.render();

            render_target.deactivate();

            glClear(GL_COLOR_BUFFER_BIT);

            auto rc = app().render_context();
            /* blit virtual window to actual window */
            render_target.draw(
                glm::ivec4(
                    0,
                    0,
                    rc.window_size.x,
                    rc.window_size.y
                )
            );
        }
    );

    pixel::app().run();

    return 0;
}
