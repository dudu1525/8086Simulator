#include <stdio.h>
#include "glad/glad.h" 

#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class CPU;
class SimulatorUI {


public:
	SimulatorUI(CPU* cpu);


	int initWindow();
	void mainRun();

	void destroyWindow();

private:
	CPU* cpu;
	GLFWwindow* window;

};