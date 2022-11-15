#pragma once
#include "../Base"

class ClippedWindow {
	double centerX{ 0 }, centerY{ 0 };
	double sizeX{ 2 }, sizeY{ 2 };
public:
	// [0,2]
	void SetSize(double x, double y) {
		sizeX = min(2, max(0, x));
		sizeY = min(2, max(0, y));
	}

	// [-1,1]
	void SetCenter(double x, double y) {
		centerX = min(1, max(-1, x));
		centerY = min(1, max(-1, y));
	}

	void SetGlViewport(GLFWwindow* window) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		int hw = w / 2;
		int hh = h / 2;
		int cx = hw + hw * centerX;
		int cy = hh + hh * centerY;
		int sx = sizeX * hw;
		int sy = sizeY * hh;
		glViewport(cx - sx / 2, cy - sy / 2, sx, sy);
	}
	double aspect(GLFWwindow* window) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		int hw = w / 2;
		int hh = h / 2;
		double sx = sizeX * hw;
		double sy = sizeY * hh;
		return sx / sy;
	}
};
