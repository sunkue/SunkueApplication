#pragma once

#include "../Base"
#include "../Renderer"
#include "../Gui"
#include "../Interaction"
#include "../System"

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
	SunkueMakeGetSet(std::unique_ptr<System>, system);
private:
	void Init();
	void Destroy();
	void DoNextFrame();
	void MainLoop() { DoNextFrame(); }
};

