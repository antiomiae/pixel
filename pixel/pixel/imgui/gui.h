#ifndef PIXEL_MAIN_GUI_H
#define PIXEL_MAIN_GUI_H

#include <GLFW/glfw3.h>

namespace pixel {

void imgui_setup(GLFWwindow* window);
void imgui_shutdown();

void imgui_frame_start();
void imgui_render();

}

#endif //PIXEL_MAIN_GUI_H
