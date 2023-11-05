#pragma once

struct SinMover {
	double height = 1.0f;
	double cycle_rate = 1.0f;
	double speed = 1.0f;

	double v = 0.0f;

	Vec2 GetPoint() {
		return { v, height * Math::Sin(v / cycle_rate) };
	}
};
