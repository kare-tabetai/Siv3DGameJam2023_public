#pragma once
#include "CircleTrigger.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "LevelManager.h"

class BulletSystem {
public:
	static void Update() {
		if (!LevelManager::IsShooting()) { return; }

		std::array<entt::entity, 10> erase_entity;
		size_t current_index = 0;

		for (const auto& [bullet, trigger] : Grobal::s_registry.view<Bullet, CircleTrigger>().each())
		{
			if (Grobal::s_registry.valid(trigger.hit_target)) {
				if (Grobal::s_registry.all_of<Player>(trigger.hit_target)) {
					GameOver();
				}
				if (Grobal::s_registry.all_of<Enemy>(trigger.hit_target)) {
					erase_entity[current_index] = trigger.hit_target;
					current_index++;

					if (erase_entity.size() <= current_index) { break; }
				}
			}
		}

		for (size_t i = 0; i < current_index; i++)
		{
			Grobal::s_registry.destroy(erase_entity[i]);
		}
	}

private:
	static void GameOver() {

	}
};
