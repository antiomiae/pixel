

#ifndef PIXEL_MAIN_NODE_H
#define PIXEL_MAIN_NODE_H

#include <vector>
#include <memory>

namespace pixel
{

template <typename T>
    class Node
{
public:
    std::vector<std::unique_ptr<T>>& children()
    {
        return children_;
    }

    T* parent() const
    {
        return parent_;
    }

    void add_child(std::unique_ptr<T> new_child)
    {
        new_child->set_parent(static_cast<T*>(this));
        children().push_back(std::move(new_child));
    }

private:
    std::vector<std::unique_ptr<T>> children_;
    T* parent_ = nullptr;
};

};

#endif //PIXEL_MAIN_NODE_H
