#pragma once

#include "../Base"
#include "Actor.h"

class System
{
public:
	System(GLFWwindow* w) : window{ w } {}
private:
	using ActorsList = std::list<std::reference_wrapper<Actor>>;
	SunkueMakeGetSet(ActorsList, actorList);
	GLFWwindow* window;
	clk::time_point timeStamp{};
public:
	void Regist(Actor& item) {
		actorList().push_back({ item });
	};
	void UnRegist(const Actor& item) {
		actorList().remove_if([&](const auto& a) { return &a.get() == &item; });
	};

	void Update() {
		auto prev = timeStamp; timeStamp = clk::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timeStamp - prev);
		double elapsedd = elapsed.count() / 1000.;
		for (auto& item : actorList()) {
			item.get().Update(elapsedd);
		}
	}
};
