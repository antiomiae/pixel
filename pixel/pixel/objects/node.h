

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

private:
    std::vector<std::unique_ptr<T>> children_;

};

};

#endif //PIXEL_MAIN_NODE_H
