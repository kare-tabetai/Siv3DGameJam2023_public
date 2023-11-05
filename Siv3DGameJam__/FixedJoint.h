#pragma once

struct FixedJoint {
	entt::entity parent = entt::null;
	Vec2 local_pos = Vec2::Zero();
	bool is_mirror_active = false;
};
