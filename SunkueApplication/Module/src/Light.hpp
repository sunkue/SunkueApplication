#pragma once

#include "../Base"
#include "GuiManager.h"

class Light {
protected:
	Eigen::Array4f color{ 1, 1, 1, 0 };
	Eigen::Array4f power{ 0.5, 1, 1, 0 }; // ambient, diffuse, specular
public:
	const float* data()const { return color.data(); }
};

class DirectionalLight : public Light {
protected:
	Eigen::Vector4f direction{ 100, 100, 33, 0 };
};

class Sun : public DirectionalLight, public GuiObject {
public:
	virtual void DrawGui() override {
		ImGui::Begin("Sun");
		ImGui::DragFloat3("color", color.data(), 0.01, 0, 1, "%.2f", 1);
		ImGui::DragFloat("ambient", &power.x(), 0.01, 0, 1, "%.2f", 1);
		ImGui::DragFloat("diffuse", &power.y(), 0.01, 0, 1, "%.2f", 1);
		ImGui::DragFloat("specular", &power.z(), 0.01, 0, 1, "%.2f", 1);
		ImGui::DragFloat3("direction", direction.data(), 1, -100, 100, "%.f", 1);
		ImGui::End();
	}
};