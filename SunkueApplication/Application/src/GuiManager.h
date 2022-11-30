#pragma once

#include "../Base"
#include "ClippedWindow.hpp"

class GuiObject {
	friend class GuiManager;
protected:
	ClippedWindow area;
	SunkueMakeVar(mutable bool, enable) = true;
	SunkueMakeGet(bool, enable);
protected:
	~GuiObject() { DisableGui(); }
	virtual void DrawGui() {};
	void EnableGui()const { _enable = true; };
	void DisableGui()const { _enable = false; };

	static void TabBar(const char* label, std::function<void()> tabBar) {
		if (ImGui::BeginTabBar(label)) {
			tabBar();
			ImGui::EndTabBar();
		}
	}
	static void TabItem(const char* label, std::function<void()> tabItem) {
		if (ImGui::BeginTabItem(label)) {
			tabItem();
			ImGui::EndTabItem();
		}
	}
	static void TreeNode(const char* label, std::function<void()> node) {
		if (ImGui::TreeNodeEx(label)) {
			node();
			ImGui::TreePop();
		}
	}
};

class GuiManager
{
public:
	GuiManager(GLFWwindow* w) :window{ w } {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
		auto& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("Resource/Font/³ª´®¼Õ±Û¾¾ ´ÞÀÇ±Ëµµ.ttf", 15.f, nullptr, io.Fonts->GetGlyphRangesKorean());
	}
	~GuiManager() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}
private:
	using GuiObjectList = std::list<std::reference_wrapper<GuiObject>>;
	GuiObjectList guiObjects;
	GLFWwindow* window;
public:
	void Regist(GuiObject& obj) {
		guiObjects.push_front({ obj });
	};
	void UnRegist(const GuiObject& obj) {
		guiObjects.remove_if([&](const auto& a) { return &a.get() == &obj; });
	};
	void RenderGui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImPlot::ShowDemoWindow();
		for (auto& o : guiObjects) {
			if (!o.get().enable())continue;
			o.get().DrawGui();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};

