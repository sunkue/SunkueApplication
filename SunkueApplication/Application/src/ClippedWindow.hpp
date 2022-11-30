#pragma once
#include "../Base"

class ClippedWindow {
protected:
	Eigen::Array2f center{ 0,0 };
	Eigen::Array2f size{ 2,2 };
public:
	// [0,2]
	void SetSize(float x, float y) {
		size.x() = std::min(2.f, std::max(0.f, x));
		size.y() = std::min(2.f, std::max(0.f, y));
	}

	// [-1,1]
	void SetCenter(float x, float y) {
		center.x() = std::min(1.f, std::max(-1.f, x));
		center.y() = std::min(1.f, std::max(-1.f, y));
	}

	void GetGLCoord(GLFWwindow* window, int& x, int& y, int& w, int& h) {
		glfwGetWindowSize(window, &w, &h);
		int hw = w / 2;
		int hh = h / 2;
		int cx = hw + hw * center.x();
		int cy = hh + hh * center.y();
		int sx = size.x() * hw;
		int sy = size.y() * hh;
		x = cx - sx / 2;
		y = cy - sy / 2;
		w = sx;
		h = sy;
	}

	float aspect() {
		return size.x() / size.y();
	}

	float aspect(GLFWwindow* window) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		int hw = w / 2;
		int hh = h / 2;
		float sx = size.x() * hw;
		float sy = size.y() * hh;
		return sx / sy;
	}

	void DrawChildGui(const std::string& n) {
		ImGui::BeginChild(n.c_str());
		ImGui::Text(n.c_str());
		ImGui::DragFloat2("Center", center.data(), 0.01, -1, 1, "%.2f", 1);
		ImGui::DragFloat2("Size", size.data(), 0.01, 0, 2, "%.2f", 1);
		ImGui::EndChild();
	}
};
