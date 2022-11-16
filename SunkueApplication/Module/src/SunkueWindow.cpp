
#include "SunkueWindow.h"


void SunkueWindow::run() {
	while (!glfwWindowShouldClose(window)) {
		MainLoop();
	}
	Destroy();
}

void SunkueWindow::Init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(500, 500, "SUNKUE", NULL, NULL);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::exception();
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	system() = std::make_unique<System>(window);
	renderer() = std::make_unique<Renderer>(window);
	gui() = std::make_unique<GuiManager>(window);
	interactor() = std::make_unique<Interaction>(window);
	interactor()->BindEventFuncs();
}

void SunkueWindow::Destroy() {
	if (nullptr==window) return;
	gui().release();
	renderer().release();
	interactor().release();
	glfwDestroyWindow(window);
	glfwTerminate();
	window = nullptr;
}

void SunkueWindow::DoNextFrame() {
	std::this_thread::yield();
	
	system()->Update();
	renderer()->Render();
	gui()->RenderGui();
	glfwSwapBuffers(window);

	interactor()->PollEvents();
}
