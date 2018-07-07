#ifndef PIXEL_RENDERER_GROUP_H
#define PIXEL_RENDERER_GROUP_H

#include <memory>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

namespace pixel::renderers
{

using namespace std;

class Renderer
{
public:
    virtual ~Renderer() = default;
};

class RendererGroup
{
public:
    template<class RendererClass>
    RendererClass& get()
    {
        auto key = type_index(typeid(RendererClass));

        if (auto r = renderers_.find(key);
            r != end(renderers_)) {
            return *static_cast<RendererClass*>(r->second.get());
        }

        renderers_[key] = make_unique<RendererClass>();

        return *static_cast<RendererClass*>(renderers_[key].get());
    }

private:
    unordered_map<type_index, unique_ptr<Renderer>> renderers_;
};

}

#endif //PIXEL_RENDERER_GROUP_H
