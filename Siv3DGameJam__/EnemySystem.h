#pragma once
#include "Position.h"
#include "Enemy.h"
#include "CircleTrigger.h"
#include "Bullet.h"
#include "Spawner.h"

class EnemySystem {
public:
	static void Initilaize() {
		LevelManager::AddOnSceneChangeEv([]([[maybe_unused]] Level previous,Level current) {
			if (current == Level::Title) {
				auto view = Grobal::s_registry.view<Enemy>();
				Grobal::s_registry.destroy(view.begin(), view.end());
			}});
	}

	static void Update() {
		std::array<entt::entity, 10> erase_entity = {entt::null};
		size_t current_index = 0;

		Grobal::s_registry.view<entt::entity,Position, Enemy, CircleTrigger>().each([&current_index,&erase_entity](entt::entity entity,const Position& position, [[maybe_unused]] Enemy& enemy, CircleTrigger& trigger) {
			if (erase_entity.size() <= current_index) { return; }
			if (Grobal::s_registry.valid(trigger.hit_target)) {
				if (Grobal::s_registry.all_of<Bullet>(trigger.hit_target)) {
					if (std::find(erase_entity.begin(), erase_entity.end(), entity) == erase_entity.end()) {
						erase_entity[current_index] = entity;
						current_index++;
					}
					if (std::find(erase_entity.begin(), erase_entity.end(), trigger.hit_target) == erase_entity.end()) {
						erase_entity[current_index] = trigger.hit_target;
						current_index++;
					}

					Spawner::CreateExplocionVfx( position.pos);

				}
			}
			});

		for (size_t i = 0; i < current_index; i++)
		{
			Grobal::s_registry.destroy(erase_entity[i]);
		}
	}
};
