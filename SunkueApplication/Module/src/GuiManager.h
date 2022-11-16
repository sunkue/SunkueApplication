#pragma once

#include "../Base"
#include "ClippedWindow.hpp"

class GuiObject {
public:
	ClippedWindow area;
	SunkueMakeVar(mutable bool, enable) = false;
	SunkueMakeGet(bool, enable);
public:
	~GuiObject() { DisableGui(); }
	virtual void DrawGui() {};
	void EnableGui()const { _enable = true; };
	void DisableGui()const { _enable = false; };
};

class GuiManager
{
public:
	GuiManager(GLFWwindow* w) :window{ w } {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
		auto& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("Resource/Font/³ª´®¼Õ±Û¾¾ ´ÞÀÇ±Ëµµ.ttf", 15.f, nullptr, io.Fonts->GetGlyphRangesKorean());
	}
	~GuiManager() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
private:
	using GuiObjectList = std::list<std::reference_wrapper<GuiObject>>;
	GuiObjectList guiObjects;
	GLFWwindow* window;
public:
	void Regist(GuiObject& obj) {
		guiObjects.push_back({ obj });
	};
	void UnRegist(const GuiObject& obj) {
		guiObjects.remove_if([&](const auto& a) { return &a.get() == &obj; });
	};
	void RenderGui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (auto& o : guiObjects) {
			if (!o.get().enable())continue;
			o.get().DrawGui();
		}
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};

