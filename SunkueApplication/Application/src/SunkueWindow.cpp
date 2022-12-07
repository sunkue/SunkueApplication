
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
		if (interactionWithCamera()) {
			constexpr double speed = 1.;
			auto& camera = this->renderer()->mainCamera();
			camera.zoom(scroll.yOffset * speed);
		}
		else {
			auto& agent = this->renderer()->selectedItem();
			if (!agent._Ptr)return;
			constexpr double speed = 1.005;
			agent->second.get().transform().scale(0 < scroll.yOffset ? speed : 1. / speed);
		}
		});
	MouseEventManager::Get().BindDefaultPosFunc([this](const MouseEventManager::PosEvent& pos) {
		auto xDiff = pos.xPos - MouseEventManager::Get().GetPrevX();
		auto yDiff = pos.yPos - MouseEventManager::Get().GetPrevY();
		if (MouseEventManager::Get().GetLeftClick() && MouseEventManager::Get().GetRightClick()) {
			if (interactionWithCamera()) {
				constexpr double xSpeed = -1.;
				constexpr double ySpeed = +1.;
				auto& camera = this->renderer()->mainCamera();
				camera.localTranslate(camera.right() * xDiff * xSpeed);
				camera.localTranslate(camera.up() * yDiff * ySpeed);
			}
			else {
				auto& agent = this->renderer()->selectedItem();
				if (!agent._Ptr)return;
				constexpr double xSpeed = +1.;
				constexpr double ySpeed = -1.;
				auto& camera = this->renderer()->mainCamera();
				auto rotate = agent->second.get().transform().rotation();
				agent->second.get().transform()
					.translate(Eigen::Translation3f{ (rotate.inverse() * camera.right()) * xDiff * xSpeed }.translation())
					.translate(Eigen::Translation3f{ (rotate.inverse() * camera.up()) * yDiff * ySpeed }.translation());
			}
		}
		else if (MouseEventManager::Get().GetWheelClick() && MouseEventManager::Get().GetRightClick()) {
			if (interactionWithCamera()) {
				constexpr double xSpeed = 1.;
				auto& camera = this->renderer()->mainCamera();
				camera.rotateAroundTarget(Eigen::Quaternionf(Eigen::AngleAxisf(radians(xDiff * xSpeed), camera.direction())));
			}
			else {
				auto& agent = this->renderer()->selectedItem();
				if (!agent._Ptr)return;
				constexpr double xSpeed = 1.;
				auto& camera = this->renderer()->mainCamera();
				auto center = agent->second.get().bound().center();
				agent->second.get().transform()
					.rotateAroundCenter(Eigen::Quaternionf(Eigen::AngleAxisf(radians(xDiff * xSpeed), camera.direction())), center);
			}
		}
		else if (MouseEventManager::Get().GetRightClick()) {
			if (interactionWithCamera()) {
				constexpr double xSpeed = -0.1;
				constexpr double ySpeed = -0.1;
				auto& camera = this->renderer()->mainCamera();
				camera.rotateAroundTarget(Eigen::Quaternionf(Eigen::AngleAxisf(radians(xDiff * xSpeed), camera.up())));
				camera.rotateAroundTarget(Eigen::Quaternionf(Eigen::AngleAxisf(radians(yDiff * ySpeed), camera.right())));
			}
			else {
				auto& agent = this->renderer()->selectedItem();
				if (!agent._Ptr)return;
				constexpr double xSpeed = -0.1;
				constexpr double ySpeed = -0.1;
				auto& camera = this->renderer()->mainCamera();
				auto center = agent->second.get().bound().center();
				agent->second.get().transform()
					.rotateAroundCenter(Eigen::Quaternionf(Eigen::AngleAxisf(radians(xDiff * xSpeed), camera.up())), center)
					.rotateAroundCenter(Eigen::Quaternionf(Eigen::AngleAxisf(radians(yDiff * ySpeed), camera.right())), center);
			}
		}
		else if (MouseEventManager::Get().GetWheelClick()) {
			if (interactionWithCamera()) {
				constexpr double ySpeed = -1.;
				auto& camera = this->renderer()->mainCamera();
				camera.zoom(yDiff * ySpeed);
			}
			else {
				auto& agent = this->renderer()->selectedItem();
				if (!agent._Ptr)return;
				constexpr double ySpeed = 1.005;
				agent->second.get().transform().scale(yDiff < 0 ? ySpeed : 1. / ySpeed);
			}
		}
		});
}

void SunkueWindow::DrawGui()
{
	if (ImGui::Begin("##window")) {
		TreeNode("built-in obj", [&] {
			ImGui::Checkbox("sun", &renderer()->sun().enable());
			ImGui::Checkbox("camera", &renderer()->mainCamera().enable());
			});
		TreeNode("Interaction With", [&] {
			if (ImGui::RadioButton("Camera", interactionWithCamera() == true)) {
				interactionWithCamera() = true;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Selected Item", interactionWithCamera() == false)) {
				interactionWithCamera() = false;
			}
			});
		TreeNode("registed items", [&]() {
			ImGui::SetNextItemWidth(200);
			if (ImGui::BeginCombo("Seleted Item##combo", renderer()->selectedItem()->first.c_str())) {
				for (const auto& item : renderer()->drawAbleItems()) {
					bool isSelected = (renderer()->selectedItem()->first == item.first);
					if (ImGui::Selectable(item.first.c_str(), isSelected)) {
						renderer()->selectedItem() = renderer()->drawAbleItems().find(item.first);
					}
					if (isSelected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			});
		ImGui::End();
	}
}
