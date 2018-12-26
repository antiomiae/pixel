#ifndef PIXEL_MAIN_GOLF_H
#define PIXEL_MAIN_GOLF_H

#include <cstdint>
#include <vector>

namespace golf
{
    /**
     * @brief A tile in the golf course map
    */
    struct GolfMapTile
    {
        int8_t corner_heights[4] = {0};
        uint32_t flags{0};
        // texture index?
    };

    class GolfMap
    {
    public:
        GolfMap(uint32_t w, uint32_t h);

        uint32_t width() const;
        uint32_t height() const;

    private:
        uint32_t width_;
        uint32_t height_;
        std::vector<GolfMapTile> tiles_;
    };

    class GolfMapRenderer
    {};

}

#endif //PIXEL_MAIN_GOLF_H
