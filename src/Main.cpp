#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include "../include/Simulator.h"

int main() {
 
    Simulator  s;
    s.run();


    return 0;
}