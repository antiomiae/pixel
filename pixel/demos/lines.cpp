

#include <pixel/pixel.h>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;


template<class Vec>
auto vec_min(const Vec& vec)
{
    return min(vec.x, vec.y);
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

    renderers::RendererGroup rg{};

    auto& line_renderer = rg.get<renderers::LineRenderer>();

    vector<LineSegment> lines;
    vector<pair<LineSegment, glm::vec4>> colored_lines;

    auto center = glm::vec2(virtual_window_size / 2);
    auto radius = vec_min(virtual_window_size) / 4.0;

    vector<glm::vec2> circle_points{};

    for (auto theta = 0.0; theta < 2.0 * M_PI; theta += M_PI / 8.0) {
        circle_points.emplace_back(center.x + radius * cos(theta), center.y + radius * sin(theta));
    }

    auto last_point = circle_points.back();

    for (const auto& point : circle_points) {
        lines.emplace_back(last_point.x, last_point.y, point.x, point.y);

        colored_lines.emplace_back(
            LineSegment{last_point.x + 1, last_point.y, point.x + 1, point.y},
            glm::vec4{1.0, 0.0, 0.0, 1.0}
        );

        last_point = point;
    }

    for_each(begin(lines), end(lines), [](auto line) { cout << line << endl; });

    pixel::app().set_tick_callback(
        [&] {
            glClear(GL_COLOR_BUFFER_BIT);

            render_target.activate();

            glClear(GL_COLOR_BUFFER_BIT);

            line_renderer.render(lines, camera);

            line_renderer.render(colored_lines, camera);

            render_target.deactivate();

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
