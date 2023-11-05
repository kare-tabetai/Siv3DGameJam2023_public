#pragma once
#include "Spawner.h"
#include "Grobal.h"
#include "Velocity.h"

class RandomSpawnTableManager {
public:
	static void Initialize() {
		for (size_t i = 0; i < kInitilaliSpawnCloudCount; ++i) {
			SpawnRandomCloud( true);
		}
	}

	static void Update() {
		if (LevelManager::GetLevel() == Level::Fight) { return; }
		if (LevelManager::GetLevel() == Level::Event) { return; }
		if (LevelManager::GetLevel() == Level::GameOverFight) { return; }
		if (LevelManager::GetLevel() == Level::ClearFight) { return; }

		s_cloud_timer += Scene::DeltaTime();

		if (kCloudSpawnInterval <= s_cloud_timer) {
			s_cloud_timer = 0.0f;
			SpawnRandomCloud();
		}
	}

private:
	static constexpr std::array< const char32_t*, 3> kCloudNames = {
		U"cloud00",
		U"cloud01",
		U"cloud02"
	};
	static constexpr double kCloudSpawnInterval = 5.0f;
	static constexpr double kCloudSpeed = 20.0f;
	static constexpr size_t kInitilaliSpawnCloudCount = 15;


	static void SpawnRandomCloud(bool is_initialize_spawn=false) {
		const auto& tex_name = kCloudNames[Random(0, static_cast<int>(kCloudNames.size() - 1))];
		int dir = Random(0, 1) ? 1 : -1;
		int x = 0;
		if (is_initialize_spawn) {
			x = Random(0, Grobal::kWindowSize.x);
		}
		else {
			x = Random(0, 1) == 1 ? 0 - 50 : Grobal::kWindowSize.x + 50;
		}
		int y = Random(0, Grobal::kWindowSize.y);
		auto cloud_entity = Spawner::CreateSprite( tex_name, { x,y }, Vec2::One() * 0.15f, 0.0f, false, -5);
		auto& vel = Grobal::s_registry.emplace<Velocity>(cloud_entity);
		vel.vel = kCloudSpeed * dir * Vec2::Right();
	}


	static inline double s_cloud_timer = 0.0f;
};
