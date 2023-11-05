# include <Siv3D.hpp> // Siv3D v0.6.12
#include "entt.hpp"
#include "PlayerSystem.h"
#include "PhysicsSystem.h"
#include "DrawSystem.h"
#include "TriggerSystem.h"
#include "UIManager.h"
#include "Grobal.h"
#include "Spawner.h"
#include "AssetManager.h"
#include "BulletSystem.h"
#include "LifeTimeSystem.h"
#include "TimerSystem.h"
#include "EnemySystem.h"
#include "SinMoveSystem.h"
#include "FixedJointSystem.h"
#include "SpawnTableManager.h"
#include "RandomSpawnTableManager.h"
#include "LevelManager.h"
#include "FightersData.h"
#include "FightSystem.h"
#include "EventManager.h"
#include "SoundManager.h"

void Main()
{
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);
	Scene::Resize(Grobal::kWindowSize);
	Window::SetTitle(U"くものいと");

	AssetManager::Initialize();

	LevelManager::AddOnSceneChangeEv([]([[maybe_unused]] Level previous, Level current) {
		if (current == Level::Title) {
			Spawner::CreateBackGrounds();
		}
		else if (current == Level::Fight) {
			Grobal::s_registry.clear();
			Spawner::CreateFightBackGrounds();
		}
});

	SoundManager::Initialize();
	RandomSpawnTableManager::Initialize();
	PlayerSystem::Initialize();
	SpawnTableManager::Initialize();
	EnemySystem::Initilaize();
	FightersData::Initialize();
	LevelManager::Initilaize();
	FightSystem::Initialize();
	UIManager::Initialize();
	EventManager::Initialize();
	LevelManager::ChangeLevel(Level::Title);

	System::Update();//なぜか開始時にクリックされてしまうため
	while (System::Update())
	{
		// 背景の色を設定する | Set the background color
		Scene::SetBackground(Palette::Black);

		ClearPrint();
		//Print << FileSystem::CurrentDirectory();


		UIManager::Update();
		LevelManager::Update();
		if (!LevelManager::IsPausing()) {
			SpawnTableManager::Update();
			RandomSpawnTableManager::Update();

			TimerSystem::Update();
			LifeTimeSystem::Update();

			SinMoveSystem::Update();
			PhysicsSystem::Update();


			EnemySystem::Update();
			PlayerSystem::Update();
			FightSystem::Update();

			FixedJointSystem::Update();
			TriggerSystem::Update();

			EventManager::Update();
		}
		DrawSystem::Update();
		EventManager::Draw();
		UIManager::Draw();
	}
}
