#include <stdio.h>
#include "glad/glad.h" 

#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class CPU;
class MainMemory;
class SimulatorUI {


public:
	SimulatorUI(CPU* cpu,MainMemory* mem );

	void takeUserInput();
	void makeMachineCode();

	int initWindow();
	void mainRun();

	void destroyWindow();

private:
	CPU* cpu;
	MainMemory* memory;

	//window components
	GLFWwindow* window;
	GLFWmonitor* monitor;

	//input panel
	void drawInputPanel(); //input text, 2 buttons

	//simulator panel
	void drawSimulatorPanel();


	bool inputWindowVisible=true;
	bool commandsCorrect = false;

};