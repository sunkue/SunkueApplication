
#include "SunkueWindow.h"


void SunkueWindow::run() {
	while (!glfwWindowShouldClose(window)) {
		MainLoop();
	}
	Destroy();
}

void SunkueWindow::Init(int w, int h) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(w, h, "SUNKUE", NULL, NULL);
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
	InitInteration();
	BindEventFuncs();
}

void SunkueWindow::Destroy() {
	if (nullptr == window) return;
	gui().release();
	renderer().release();
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

	glfwPollEvents();
	MouseEventManager::Get().ProcessInput();
	KeyboardEventManager::Get().ProcessInput();
}

void SunkueWindow::InitInteration()
{
	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow* window, int w, int h)
		{});
	glfwSetScrollCallback(window,
		[](GLFWwindow* window, double xoffset, double yoffset)
		{ MouseEventManager::Get().Scroll(window, xoffset, yoffset);  });
	glfwSetMouseButtonCallback(window,
		[](GLFWwindow* window, int key, int action, int modifiers)
		{ MouseEventManager::Get().MouseButton(window, key, action, modifiers);  });
	glfwSetCursorPosCallback(window,
		[](GLFWwindow* window, double xpos, double ypos)
		{ MouseEventManager::Get().CursorPosition(window, xpos, ypos);  });
	glfwSetKeyCallback(window,
		[](GLFWwindow* window, int key, int code, int action, int modifiers)
		{ KeyboardEventManager::Get().Keyboard(window, key, code, action, modifiers);  });
}

void SunkueWindow::BindEventFuncs()
{
	KeyboardEventManager::Get().BindKeyFunc(GLFW_KEY_R, [this](const KeyboardEventManager::KeyEvent& key) {
		if (key.action == GLFW_PRESS) { this->renderer()->ResetCamera(); }
		});
	MouseEventManager::Get().BindDefaultScrollFunc([this](const MouseEventManager::ScrollEvent& scroll) {
		constexpr double speed = 1.;
		auto& camera = this->renderer()->mainCamera();
		camera.zoom(scroll.yOffset * speed);
		});
	MouseEventManager::Get().BindDefaultPosFunc([this](const MouseEventManager::PosEvent& pos) {
		auto xDiff = pos.xPos - MouseEventManager::Get().GetPrevX();
		auto yDiff = pos.yPos - MouseEventManager::Get().GetPrevY();
		if (MouseEventManager::Get().GetLeftClick() && MouseEventManager::Get().GetRightClick()) {
			constexpr double xSpeed = 1.;
			constexpr double ySpeed = -1.;
			auto& camera = this->renderer()->mainCamera();
			camera.localTranslate(camera.right() * xDiff * xSpeed);
			camera.localTranslate(camera.up() * yDiff * ySpeed);
		}
		else if (MouseEventManager::Get().GetWheelClick() && MouseEventManager::Get().GetRightClick()) {
			constexpr double xSpeed = 1.;
			auto& camera = this->renderer()->mainCamera();
			camera.rotateAroundTarget(Eigen::Quaternionf(Eigen::AngleAxisf(radians(xDiff * xSpeed), camera.direction())));
		}
		else if (MouseEventManager::Get().GetRightClick()) {
			constexpr double xSpeed = -0.1;
			constexpr double ySpeed = -0.1;
			auto& camera = this->renderer()->mainCamera();
			camera.rotateAroundTarget(Eigen::Quaternionf(Eigen::AngleAxisf(radians(xDiff * xSpeed), camera.up())));
			camera.rotateAroundTarget(Eigen::Quaternionf(Eigen::AngleAxisf(radians(yDiff * ySpeed), camera.right())));
		}
		else if (MouseEventManager::Get().GetWheelClick()) {
			constexpr double ySpeed = -1.;
			auto& camera = this->renderer()->mainCamera();
			camera.zoom(yDiff * ySpeed);
		}
		});
}
