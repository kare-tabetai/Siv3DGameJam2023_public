#pragma once
#include "SinMover.h"
#include "Velocity.h"

class SinMoveSystem {
public:
	static void Update() {
		Grobal::s_registry.view<Velocity,SinMover>().each([](Velocity& vel,SinMover& mover) {
			Vec2 previous = mover.GetPoint();
			mover.v += Scene::DeltaTime();
			Vec2 current = mover.GetPoint();
			Vec2 dir = current - previous;
			if (dir.isZero()) { return; }
			dir.normalize();
			vel.vel = dir * mover.speed * Math::Sign(vel.vel.x);

			if (2 * Math::Pi <= mover.v / mover.cycle_rate) {
				mover.v = 2 * Math::Pi - mover.v / mover.cycle_rate;
			}
			});
	}
};
