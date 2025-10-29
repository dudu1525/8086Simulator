#include <stdio.h>
#include "glad/glad.h" 

#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../include/CPU.h"
#include "../include/SimulatorUI.h"
#include "../include/MainMemory.h"

int main(int, char**)
{
    MainMemory memory;
    CPU cpu(&memory);
    SimulatorUI sim(&cpu, &memory);

    sim.initWindow();
    //sim.transform userInput
    //give to mem
    sim.mainRun();
    sim.destroyWindow();

  


    return 0;
}