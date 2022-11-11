#pragma once

#include "../Base"
#include "KeyBoardEvent.h"
#include "MouseEvent.h"

class Interaction {
public:
	Interaction(GLFWwindow* w) :window{ w } {}
private:
	GLFWwindow* window;
	KeyboardEventManager keyboardManager;
	MouseEventManager mmouseManager;
public:
	void PollEvents() { glfwPollEvents(); };

	void BindEventFuncs() {
		glfwSetFramebufferSizeCallback(window,
			[](GLFWwindow* window, int w, int h)
			{});
		glfwSetKeyCallback(window,
			[](GLFWwindow* window, int key, int code, int action, int modifiers)
			{});
		glfwSetScrollCallback(window,
			[](GLFWwindow* window, double xoffset, double yoffset)
			{});
		glfwSetMouseButtonCallback(window,
			[](GLFWwindow* window, int key, int action, int modifiers)
			{std::cerr << "!"; });
		glfwSetCursorPosCallback(window,
			[](GLFWwindow* window, double xpos, double ypos)
			{});
	}
};