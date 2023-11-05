#pragma once
#include "ElapsedTimer.h"

class TimerSystem {
public:
	static void Update() {
		Grobal::s_registry.view<ElapsedTimer>().each([](ElapsedTimer& timer) {
			timer.elapsed_sum += Scene::DeltaTime();
			});
	}
};
