#include <stdio.h>
#include "glad/glad.h" 

#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>
class CPU;
class MainMemory;
class SimulatorUI {


public:
	SimulatorUI(CPU* cpu,MainMemory* mem );



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


	void passInstructions();
	char inputBuffer[1024 * 4];

	bool inputWindowVisible=true;
	bool commandsCorrect = false;

	bool verifyIfLabel(std::string labelCouldBe);

};