#include <pixel/pixel.h>
#include <pixel/level.h>
//#include <pixel/input
#include <imgui.h>
#include <pixel/imgui/imgui_impl_glfw.h>
#include <pixel/imgui/imgui_impl_opengl3.h>

using namespace pixel;
using namespace pixel::graphics;


class SpriteDebugMenu
{
public:
    SpriteDebugMenu(Sprite* sprite)
    : sprite_(sprite)
    {
    }

    void update()
    {
        if (!is_active_ && should_activate()) {
             is_active_ = true;
        }

        render();
    }

private:
    bool is_active_ = true;
    Sprite* sprite_ = nullptr;

    bool should_activate()
    {
        auto v = input::Keyboard::keymap[GLFW_KEY_D];
        return v;
    }

    void render()
    {
        if (!is_active_) {
            return;
        }

        if (!ImGui::Begin("Sprite", &is_active_)) {
            ImGui::End();
            return;
        }

        ImGui::SliderFloat("x", &sprite_->position.x, 0, app().render_context().virtual_window_size.x);
        ImGui::SliderFloat("y", &sprite_->position.y, 0, app().render_context().virtual_window_size.y);

        ImGui::SliderFloat("angle", &sprite_->angle, 0, 2*M_PI);

        ImGui::End();
    }
};


void texture_atlas_tree_view(TextureAtlas& atlas)
{
    using TreeValue = std::tuple<TextureAtlas::RegionMapItem, std::string>;
    vector<vector<TreeValue>> data(atlas.atlas_size().z);

    for (const auto& item : atlas.region_map()) {

        string entry_name = "";
        const auto& nr = atlas.name_registry();

        const auto& it = find_if(begin(nr), end(nr), [&](const auto& p) { return p.second == item.first; });
        if (it != end(nr)) {
            entry_name = it->first;
        }

        data[item.second.layer].push_back(TreeValue(item, entry_name));
    }

    for (auto& values : data) {
        if (values.size() > 0) {
            sort(begin(values), end(values), [](auto& item1, auto& item2) { return get<1>(item1) < get<1>(item2); });
        }
    }

    for (auto i = 0u; i < data.size(); ++i) {
        if (data[i].size() > 0 && ImGui::TreeNode((string("Layer") + to_string(i)).c_str())) {
            for (auto &[item, str] : data[i]) {
                auto id = item.first;
                auto region = item.second;
                ImGui::Text("%s %i [%i, %i, %i, %i]", str.c_str(), id, region.x, region.y, region.w, region.h);
            }

            ImGui::TreePop();
        }
    }
}

void gui(Level& level)
{
    static bool show_demo_window = false;

    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("Debug");

    ImGui::Checkbox("Show demo window", &show_demo_window);

    if (ImGui::TreeNode("Texture Atlas")) {
        texture_atlas_tree_view(level.texture_atlas());
        ImGui::TreePop();
    }

    ImGui::End();


}

void update(Level& level)
{
    static Sprite sprite = level.get_sprite("assets/sonic.png");
    static SpriteDebugMenu debug_menu(&sprite);

    gui(level);

    debug_menu.update();

    level.begin_render();

    SpriteBatch batch;

    batch.add(sprite);

    level.renderer_group().get<pixel::renderers::SpriteRenderer>().render(batch.sprites(), level.sprite_texture(), level.camera());
}

void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{1400, 800};
    glm::ivec2 actual_window_size = virtual_window_size * 1;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    Level level(app().render_context().virtual_window_size);

    vector<string> sprites = {"assets/sonic.png"};

    for (auto i = 0u; i < 100; ++i) {
        sprites.push_back("assets/random_images/" + to_string(i) + ".png");
    }

    level.load_sprites(sprites);

    pixel::app().set_background_color({1, 1, 1, 1});

    pixel::app().set_tick_callback(
        [&] {
            update(level);
        }
    );

    pixel::app().run();
}

int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}

