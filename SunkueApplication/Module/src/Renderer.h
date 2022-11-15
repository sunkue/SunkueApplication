#pragma once

#include "Shader.h"
#include "Actor.h"
#include "Camera.hpp"

class Renderer
{
public:
	Renderer(GLFWwindow* w) : window{ w } { test(); }
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
private:
	void RegisterActor(Actor& actor) {
		actors.push_back({ actor });
	};
	void UnRegisterActor(const Actor& actor) {
		actors.remove_if([&](const auto& a) { return &a.get() == &actor; });
	};

	void Clear()
	{
		glClearColor(0.9, 0.9, 0.9, 1);
		glClear(GL_COLOR_BUFFER_BIT);
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

	void test()
	{
		mainCamera.SetCenter(0, 0.5);
		mainCamera.SetSize(1.75, 1);
		static Actor a; a.InitVao();
		RegisterActor(a);
	}

};

