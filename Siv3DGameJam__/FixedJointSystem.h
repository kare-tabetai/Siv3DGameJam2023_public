#pragma once
#include "Position.h"
#include "FixedJoint.h"

class FixedJointSystem {
public:
	static void Update() {
		Grobal::s_registry.view<Position, FixedJoint>().each([](Position& pos, FixedJoint& joint) {
			const auto& parent_pos = Grobal::s_registry.get<Position>(joint.parent);

			if (joint.is_mirror_active && Grobal::s_registry.all_of<SpriteRender>(joint.parent)) {
				const auto& parent_sprite = Grobal::s_registry.get<SpriteRender>(joint.parent);
				if (parent_sprite.is_mirrored) {
					pos.pos = parent_pos.pos + Vec2{ -joint.local_pos.x, joint.local_pos.y };
				}
				else {
					pos.pos = parent_pos.pos + joint.local_pos;
				}
			}
			else {
				pos.pos = parent_pos.pos + joint.local_pos;
			}
			
		});
	}
};
