#pragma once

#include "Shader.h"
#include "DrawAble.h"
#include "Camera.hpp"
#include "Light.hpp"

class Renderer
{
public:
	Renderer(GLFWwindow* w) : window{ w } { Init(); }
private:
	using DrawAbleList = std::map<std::string, std::reference_wrapper<DrawAble>>;

	SunkueMakeGetSet(DrawAbleList, drawAbleItems);
	SunkueMakeGetSet(Camera, mainCamera);
	SunkueMakeGetSet(Sun, sun);
	SunkueMakeGetSet(DrawAbleList::iterator, selectedItem);
	GLFWwindow* window;
public:
	void Render() {
		Clear();
		Draw();
	}
	void Regist(const std::string& name, DrawAble& item) {
		_selectedItem = drawAbleItems().insert({ name, { item } }).first;
	};
	void UnRegist(const DrawAble& item) {
		if (&_selectedItem->second.get() == &item)_selectedItem = DrawAbleList::iterator();
		for (auto it = drawAbleItems().begin(); it != drawAbleItems().end();) {
			if (&it->second.get() == &item) it = drawAbleItems().erase(it);
			else ++it;
		}
	};
	void UnRegist(const std::string& name) {
		if (_selectedItem->first == name)_selectedItem = DrawAbleList::iterator();
		drawAbleItems().erase(name);
	};
	void ResetCamera();
private:
	void Clear()
	{
		glClearColor(0.9, 0.9, 0.9, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ReadyDraw()
	{
		Clear();
		/* 필요할 때만 변경 하도록.. */
		_mainCamera.Update(window);
		Eigen::Array4i Resoulution;
		_mainCamera.GetGLCoord(window, Resoulution.x(), Resoulution.y(), Resoulution.z(), Resoulution.w());
		glViewport(Resoulution.x(), Resoulution.y(), Resoulution.z(), Resoulution.w());
		Shader::SetUbo("V_MAT", _mainCamera.viewMatrix().data());
		Shader::SetUbo("P_MAT", _mainCamera.projectionMatrix().data());
		Shader::SetUbo("RESOLUTION", Resoulution.data());
		Shader::SetUbo("CAMERA", _mainCamera.position().data());
		Shader::SetUbo("LIGHT", _sun.data());
	}

	void Draw() {
		ReadyDraw();
		for (auto& item : drawAbleItems()) {
			item.second.get().Draw();
		}
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
		glEnable(GL_PROGRAM_POINT_SIZE);

		mainCamera().enable() = false;
		sun().enable() = false;
	}

};

