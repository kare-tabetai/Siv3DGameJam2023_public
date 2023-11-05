#pragma once
#include "Position.h"
#include "Velocity.h"

class PhysicsSystem {
public:
	static void Update() {
		Grobal::s_registry.view<Position, Velocity>().each([](Position& pos, Velocity& vel) {
			pos.pos += Scene::DeltaTime() * vel.vel;
		});
	}
};
