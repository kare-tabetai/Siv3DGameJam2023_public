#pragma once
#include "ElapsedTimer.h"
#include "LifeTime.h"

class LifeTimeSystem {
public:
	static void Update() {
		std::array<entt::entity, 10> erase_entity;
		size_t current_index = 0;

		Grobal::s_registry.view<entt::entity,ElapsedTimer,LifeTime>().each([ &current_index, &erase_entity](entt::entity entity,const ElapsedTimer& elpased_timer,LifeTime& life_time) {
			if (erase_entity.size() <= current_index) { return; }
			if (life_time.life_time <= elpased_timer.elapsed_sum) {
				erase_entity[current_index] = entity;
				current_index++;
			}
			});

		for (size_t i = 0; i < current_index; i++)
		{
			Grobal::s_registry.destroy(erase_entity[i]);
		}
	}
};
