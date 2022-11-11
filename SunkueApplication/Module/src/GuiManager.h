#pragma once

#include "../Base"

class GuiObject {
public:
	virtual void DrawGui() {};
	virtual void Register() {};
	virtual void UnRegister() {}; // On destroy
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
	GLFWwindow* window;
public:
	using GuiObjectList = std::list<std::reference_wrapper<GuiObject>>;
	GuiObjectList GuiObjects;

	void RenderGui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (auto& o : GuiObjects) o.get().DrawGui();
		
		{
			ImGui::Begin("PlayerInfo", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
			static float x;
			ImGui::DragFloat("x", &x, 0.1, 0, 100, "%3f", 1);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};

