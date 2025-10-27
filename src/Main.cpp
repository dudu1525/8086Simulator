#include <stdio.h>
#include "glad/glad.h" 

#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../include/CPU.h"
#include "../include/SimulatorUI.h"


int main(int, char**)
{

    CPU cpu;
    SimulatorUI sim(&cpu);

    sim.initWindow();
    sim.mainRun();
    sim.destroyWindow();

  


    return 0;
}