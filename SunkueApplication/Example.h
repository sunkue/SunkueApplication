#pragma once

#include "Module/Application";

class TestActor : public Actor, public DrawAble, public GuiObject
{
private:
	GLfloat pointSize{ 0.25 };
	SunkueMakeGetSet(float, movingSpeed) { 5 };
public:
	TestActor(Shader& shader) : DrawAble{ shader } {}

	virtual void Draw()override {
		shader.Use();
		shader.Set("u_model_mat", modelMatrix());
		shader.Set("u_radius", pointSize);
		shader.Set("u_shininess", shininess);
		glBindVertexArray(vao);
		glDrawArrays(drawMode, 0, vertexNum);
	}

	virtual void DrawGui()override {
		ImPlot::ShowDemoWindow();
		ImGui::Begin("Actor A");
		ImGui::DragFloat("pointSize", &pointSize, 0.1, 0.1, 2, "%.3f", 1);
		ImGui::DragFloat("shininess", &shininess, 0.1, 0, 200, "%.1f", 1);
		ImGui::DragFloat("speed", &_movingSpeed, 0.1, 0, 25, "%.1f", 1);
		ImGui::End();
	}
};

