#pragma once

#include "../Base"
#include "Transform.hpp"
#include "ClippedWindow.hpp"

// vp matrix = viewport + sight
class Camera : public TransformComponnent, public ClippedWindow
{
	Eigen::Vector3d position{ 0.,0.,10. };
	Eigen::Vector3d target{ 0., 0., 0. };
	Eigen::Vector3d up{ 0., 1., 0. };
	double fovy{ 45.0 };
	double n{ 0.1 }, f{ 2000. };
private:
	Eigen::Matrix4d viewMatrix{};
	Eigen::Matrix4d projectionMatrix{};
public:
	void SetFovy(double d) { fovy = d; }
	void SetF(double d) { f = d; }
public:
	void Update(GLFWwindow* window) {
		projectionMatrix = perspective(radians(fovy), aspect(window), n, f);
		viewMatrix = lookAt(position, target, up);
	}
	Eigen::Matrix4d vpMatrix() const {
		return  projectionMatrix * viewMatrix;
	}
};

