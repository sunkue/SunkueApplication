#pragma once

#include "../Base"
#include "Transform.hpp"
#include "Shader.h"
#include "GuiManager.h"



class Actor 
{
	using UpdateFunc = std::function<void(float)>;
	using PriorityUpdateFunc = std::pair<size_t, UpdateFunc>;
	struct PriorityUpdateFuncLess {
		_NODISCARD constexpr bool operator()(const PriorityUpdateFunc& _Left, const PriorityUpdateFunc& _Right) const {
			return _Left.first < _Right.first;
		}
	};
	using UpdateFuncs = std::set<PriorityUpdateFunc, PriorityUpdateFuncLess>;
protected:
	UpdateFuncs updateFuncs;
public:
	void Update(float elapsed) {
		for (auto& f : updateFuncs) f.second(elapsed);
	}
	void AddUpdateFunc(UpdateFunc f, size_t priority = std::numeric_limits<size_t>::max()) {
		updateFuncs.insert(std::make_pair(priority, f));
	}
};

