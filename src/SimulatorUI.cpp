#include "../include/SimulatorUI.h"
#include "../include/CPU.h"
#include "../include/MainMemory.h"

#include <string>


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

SimulatorUI::SimulatorUI(CPU* cpu, MainMemory* mem)
{
    this->cpu = cpu;
    this->memory = mem;
}

int SimulatorUI::initWindow()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

 
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


    //window creations
    monitor= glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
   
   // window = glfwCreateWindow(mode->width, mode->height, "Simulator Window", monitor, NULL);
    window = glfwCreateWindow(1580, 900, "Simulator Window", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize OpenGL loader (GLAD)!\n");
        return 1;
    }



    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

}

void SimulatorUI::mainRun()
{
    while (!glfwWindowShouldClose(window))
    {
       
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        {
            glfwSetWindowShouldClose(window, true);
        }



        if (!inputWindowVisible)
        {
            if (ImGui::IsKeyPressed(ImGuiKey_Space, false))
            {
                cpu->step();

            }
        }

       

        if (inputWindowVisible)
        {   
            ImGuiIO io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(0,0));
            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x , io.DisplaySize.y));
            ImGui::Begin("Input Commands", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

            //components 

            drawInputPanel();

            
         




            ImGui::End();
        }

        if (!inputWindowVisible)
        {
            ImGuiIO io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
            ImGui::Begin("8086 Simulator", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            //components 
    
            this->drawSimulatorPanel();


            ImGui::End();
        }

     





        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void SimulatorUI::destroyWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void SimulatorUI::drawInputPanel()
{
    
   

    ImVec2 windowSize = ImGui::GetWindowSize();
    float widgetWidth = 600.0f;
    float centerwidget = (windowSize.x - widgetWidth) * 0.5f;


    ImGui::SetWindowFontScale(2.0f);
    ImGui::SetCursorPosY(50);
    ImVec2 size = ImGui::CalcTextSize("In this window, enter the commands you want to be executed, line by line:");

    ImGui::SetCursorPosX((windowSize.x-size.x)/2);
    ImGui::Text("In this window, enter the commands you want to be executed, line by line:");
    ImGui::SetWindowFontScale(1.0f);


    static char inputBuffer[1024 * 4] = "";

    ImGui::SetCursorPosX(centerwidget);
    ImGui::SetCursorPosY(110);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
  //  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
    if (ImGui::InputTextMultiline("##input", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImVec2(widgetWidth, 500)))
    {

        commandsCorrect = false;
    }
   ImGui::PopStyleColor(1);

    ImGui::SetCursorPosX((windowSize.x-610)/2);
    ImGui::SetCursorPosY(650);
    if (ImGui::Button("Verify Commands", ImVec2(300, 50)))
    {
       
        //call bool cpu.verifycommands
        //based on the result, make the save and begin visible

        commandsCorrect = true;
    }
    if (commandsCorrect)
    {
        ImGui::SameLine();
        if (ImGui::Button("Save and Begin" , ImVec2(300, 50)))
        {
            inputWindowVisible = false;
            //cpu encodeIntoMachineCode
        }
    }

    //can have another panel in which the encoded instructions are visible (the 001etc format)


}

void SimulatorUI::drawSimulatorPanel()
{
    ImVec2 windowSize = ImGui::GetWindowSize();

    //top text
    ImGui::SetWindowFontScale(1.5f);
    ImGui::SetCursorPosY(20);
    ImVec2 size = ImGui::CalcTextSize("Press SPACEBAR to step");
    ImGui::SetCursorPosX((windowSize.x - size.x) / 2);
    ImGui::Text("Press SPACEBAR to step");
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Separator();
    //main content
    ImGuiIO& io = ImGui::GetIO();
    ImGui::BeginChild("CommandArea", ImVec2(0, io.DisplaySize.y - 100), true); 
     

    ImGui::Columns(4, nullptr, false);
    //first column

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    //Data Registers
    ImGui::BeginChild("FirstColumn", ImVec2(0, 0), true);
    ImGui::Text("Data Registers");

    ImGui::BeginChild("DataRegs", ImVec2(0, 200), true);
    ImGui::Text("AX: %04x", cpu->euunit.ax);
    ImGui::Text("BX: %04x", cpu->euunit.bx);
    ImGui::Text("CX: %04x", cpu->euunit.cx);
    ImGui::Text("DX: %04x", cpu->euunit.dx);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("SP: %04x", cpu->euunit.sp);
    ImGui::Text("BP: %04x", cpu->euunit.bp);
    ImGui::Text("SI: %04x", cpu->euunit.si);
    ImGui::Text("DI: %04x", cpu->euunit.di);

    ImGui::EndChild();
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    //Segment Registers
    ImGui::Text("Segment Registers");  
    ImGui::BeginChild("SegRegs", ImVec2(0, 200), true);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("DS: %04x", cpu->biuunit.segreg.dsreg);
    ImGui::Text("SS: %04x", cpu->biuunit.segreg.ssreg);
    ImGui::Text("ES: %04x", cpu->biuunit.segreg.esreg);
    ImGui::Text("CS: %04x", cpu->biuunit.segreg.csreg);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("IP: %04x", cpu->biuunit.segreg.ip);
    ImGui::EndChild();
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    //Internal BIU Registers
    ImGui::Text("Internal BIU Registers");
    ImGui::BeginChild("BIURegs", ImVec2(0, 200), true);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("DataReg1: %04x", cpu->biuunit.internalregs.regForData);
    ImGui::Text("OffsetReg1: %04x", cpu->biuunit.internalregs.regForOffset);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("DataReg1: %04x", cpu->biuunit.internalregs.regForData2);
    ImGui::Text("OffsetReg1: %04x", cpu->biuunit.internalregs.regForOffset2);
    ImGui::EndChild();


    ImGui::EndChild();




    ImGui::NextColumn();
    //second column
    ImGui::BeginChild("SecondColumn", ImVec2(0, 0), true);
    ImGui::Text("BIU Control Unit");
   
    ImGui::BeginChild("BIUcontrol", ImVec2(0, 200), true);
    ImGui::Text("%s", cpu->biuunit.biucontrol.returnCurrentState());

    ImGui::EndChild();
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("EU Control Unit");

    ImGui::BeginChild("Eucontrol", ImVec2(0, 200), true);
    ImGui::Text("%s", cpu->euunit.eucontrol.returnCurrentState());
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Current Decoded Instruction:\n%s", cpu->euunit.eucontrol.returnCurrentDecodedInst());
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("\nTemporary register 1: %04x", cpu->euunit.tempreg1);
    ImGui::Text("\nTemporary register 2: %04x", cpu->euunit.tempreg2);
    ImGui::EndChild();
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Instructions Queue");

    ImGui::BeginChild("InstrQueue", ImVec2(0, 100), true);
   uint8_t* queueReff = cpu->biuunit.instrqueue.returnFullQueue();
    for (int i = 0; i < 6; i++)
    {
        ImGui::Text("%02x", queueReff[i]);
        ImGui::SameLine();
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Instructions in queue:%d", this->cpu->biuunit.instrqueue.numOfInstr);

    ImGui::EndChild();
    ImGui::Text("Flags");
    ImGui::BeginChild("Flags", ImVec2(0, 150), true);

    ImGui::EndChild();
   

    ImGui::EndChild();


    ImGui::NextColumn();
    //third column
    ImGui::Text("ALU");
    ImGui::BeginChild("ALU", ImVec2(0, 150), true);
    ImGui::Text("Opperand 1: %04x\n", cpu->euunit.alu.operand1);
    ImGui::Text("Opperand 2: %04x\n", cpu->euunit.alu.operand2);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Current state: %s", cpu->euunit.alu.returnState());
    ImGui::Text("Current operation: %s", cpu->euunit.alu.returnOperation());
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Result: %04x\n", cpu->euunit.alu.result);
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Main Data Bus");
    ImGui::BeginChild("Main Data Bus", ImVec2(0, 150), true);
    ImGui::Text("Current data: %04x\n", cpu->euunit.maindatabus.data);
    ImGui::Text("Current state: %s", cpu->euunit.maindatabus.returnState());
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("BIU Address Bus");
    ImGui::BeginChild("BIU Address Bus", ImVec2(0, 150), true);
    ImGui::Text("Current data: %04x\n", cpu->biuunit.incomingaddress.addressbus);
    ImGui::Text("Current state: %s", cpu->biuunit.incomingaddress.returnState());
    ImGui::EndChild();  

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("BIU Data Bus");
    ImGui::BeginChild("BIU Data Bus", ImVec2(0, 150), true);
    ImGui::Text("Current data: %04x\n", cpu->biuunit.incomingbiudata.databus);
    ImGui::Text("Current state: %s", cpu->biuunit.incomingbiudata.returnState());
    ImGui::EndChild();


    ImGui::NextColumn();
    //4th column
    ImGui::Text("Main Memory");
    ImGui::BeginChild("Memory", ImVec2(0, 250), true);
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Exterior Memory BUS");
    ImGui::BeginChild("Memorybus", ImVec2(0, 250), true);
    ImGui::Text("Current data: %04x\n", cpu->biuunit.membus.databus);
    ImGui::Text("Current address used: %08x\n", cpu->biuunit.membus.addressbus);
    ImGui::Text("Current state: %s", cpu->biuunit.membus.returnState());

    ImGui::EndChild();

    ImGui::EndChild();


    //maybe add a command being executed right now: 

}
