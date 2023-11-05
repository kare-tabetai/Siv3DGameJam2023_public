#pragma once
#include "Position.h"
#include "CircleTrigger.h"

class TriggerSystem {
public:
	static void Update() {

		Grobal::s_registry.view<CircleTrigger>().each([](CircleTrigger& trigger) {trigger.hit_target = entt::null; });

		auto view = Grobal::s_registry.view<Position, CircleTrigger>();
		for (auto&& [entity, pos, trigger] : view.each()) {
			if (Grobal::s_registry.valid(trigger.hit_target)) { continue; }

			for (auto&& [check_entity, check_pos, check_trigger] : view.each()) {
				if (entity == check_entity) { continue; }
				if (!IsAbleToHit(trigger.tag, check_trigger.tag)) { continue; }
				if(Grobal::s_registry.valid(check_trigger.hit_target)) { continue; }

  				auto dist = s3d::Geometry2D::Distance(pos.pos, check_pos.pos);
				if (dist <= trigger.r + check_trigger.r) {
					trigger.hit_target = check_entity;
					check_trigger.hit_target = entity;
				}
			}
		}
	}
};
