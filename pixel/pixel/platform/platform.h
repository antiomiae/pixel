#ifndef PIXEL_MAIN_PLATFORM_H
#define PIXEL_MAIN_PLATFORM_H

namespace pixel
{

class Window;

/**
 * The base layer API for basic system initialization and services, allowing
 * adapters for different platform APIs (like glfw, SDL, iOS)
 */
class Platform
{
public:
    ~Platform() = default;

    /**
     * Entrypoint to initialize platform
     */
    virtual void init() {};
    /**
     * Entrypoint to teardown platform
     */
    virtual void de_init() {};

    /**
     * Return platform specific pixel::Window subclass
     * \return pointer to window
     */
    virtual Window* get_window() = 0;

    /**
     * Sets OpenGL context tied to platform window to be current
     */
    virtual void make_context_current() = 0;
};

};

#endif //PIXEL_MAIN_PLATFORM_H
