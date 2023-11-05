#pragma once
#include "entt.hpp"

class Grobal {
public :
	static constexpr Point kWindowSize = { 1200, 900 };
	static inline entt::registry s_registry;
};
