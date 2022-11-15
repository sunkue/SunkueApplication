#pragma once

#include "Shader.h"
#include "Actor.h"
#include "Camera.hpp"

class Renderer
{
public:
	Renderer(GLFWwindow* w) : window{ w } { Init(); }
private:
	using ActorsList = std::list<std::reference_wrapper<Actor>>;
	ActorsList actors;
	Camera mainCamera;
	GLFWwindow* window;
public:
	void Render() {
		Clear();
		Draw();
	}
	void Regist(Actor& actor) {
		actors.push_back({ actor });
	};
	void UnRegist(const Actor& actor) {
		actors.remove_if([&](const auto& a) { return &a.get() == &actor; });
	};
private:
	void Clear()
	{
		glClearColor(0.9, 0.9, 0.9, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ReadyDraw()
	{
		mainCamera.Update(window);
		mainCamera.SetGlViewport(window);
		Clear();
		Shader::SetUbo("VP_MAT", mainCamera.vpMatrix().data());
	}

	void Draw() {
		ReadyDraw();
		for (auto& a : actors) { a.get().Draw(); }
	}

	void Init()
	{
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_3D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POINT_SMOOTH);
		
		mainCamera.SetCenter(0, 0.5);
		mainCamera.SetSize(1.75, 1);
	}

};

