#pragma once

struct FighterAttack {
	Vec2 dir = Vec2::Zero();
	int damage = 0;
	bool hit_erase = false;
};
