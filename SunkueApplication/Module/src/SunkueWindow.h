#pragma once

#include "../Base"
#include "../Renderer"
#include "../Gui"
#include "../Interaction"

class SunkueWindow
{
private:
	GLFWwindow* window;
public:
	SunkueWindow() { Init(); }
	~SunkueWindow() { Destroy(); }
public:
	void run();
private:
	SunkueMakeGetSet(std::unique_ptr<Renderer>, renderer);
	SunkueMakeGetSet(std::unique_ptr<Interaction>, interactor);
	SunkueMakeGetSet(std::unique_ptr<GuiManager>, gui);
private:
	void Init();
	void Destroy();
	void DoNextFrame();
	void MainLoop() { DoNextFrame(); }
};

