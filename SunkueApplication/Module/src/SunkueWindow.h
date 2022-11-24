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
	SunkueWindow(int w = 500, int h = 500) { Init(w, h); }
	~SunkueWindow() { Destroy(); }
public:
	void run();
private:
	SunkueMakeGetSet(std::unique_ptr<Renderer>, renderer);
	SunkueMakeGetSet(std::unique_ptr<GuiManager>, gui);
	SunkueMakeGetSet(std::unique_ptr<System>, system);
private:
	void Init(int w = 500, int h = 500);
	void Destroy();
	void DoNextFrame();
	void MainLoop() { DoNextFrame(); }
	void InitInteration();
	void BindEventFuncs();
};

