#ifndef PIXEL_MAIN_RANDOM_H
#define PIXEL_MAIN_RANDOM_H

#include <random>

namespace pixel::random
{
    using random_engine_type = std::default_random_engine;

    random_engine_type& default_engine()
    {
        static random_engine_type engine{};

        return engine;
    }
}

#endif //PIXEL_MAIN_RANDOM_H
