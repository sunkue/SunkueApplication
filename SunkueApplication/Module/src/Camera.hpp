#pragma once

#include "../Base"
#include "Transform.hpp"
#include "ClippedWindow.hpp"

// vp matrix = viewport + sight
class Camera : public TransformComponnent, public ClippedWindow, public GuiObject
{
	SunkueMakeGetSet(Eigen::Vector3f, position) { 0., 0., 10. };
	SunkueMakeGetSet(Eigen::Vector3f, target) { 0., 0., 0. };
	SunkueMakeGetSet(Eigen::Vector3f, up) { 0., 1., 0. };
	SunkueMakeGetSet(float, fovy) { 45.0 };
	SunkueMakeGetSet(float, n) { 0.1 };
	SunkueMakeGetSet(float, f) { 2000. };
private:
	SunkueMakeVar(Eigen::Matrix4f, viewMatrix){};
	SunkueMakeGet(Eigen::Matrix4f, viewMatrix);
	SunkueMakeVar(Eigen::Matrix4f, projectionMatrix) {};
	SunkueMakeGet(Eigen::Matrix4f, projectionMatrix);
public:
	void Update(GLFWwindow* window) {
		_viewMatrix = lookAt(position(), target(), up());
		_projectionMatrix = perspective(radians(fovy()), aspect(window), n(), f());
	}
	virtual void DrawGui() {
		ImGui::Begin("Main Camera");
		ImGui::DragFloat("fovy", &_fovy, 1, 10, 180, "%.1f", 1);
		ImGui::DragFloat3("position", _position.data(), 0.1, -10, 10, "%.1f", 1);
		ImGui::DragFloat3("target", _target.data(), 0.1, -10, 10, "%.1f", 1);
		ImGui::DragFloat3("up", _up.data(), 0.1, -10, 10, "%.1f", 1);
		ClippedWindow::DrawChildGui("ViewPort");
		ImGui::End();
	}
};

