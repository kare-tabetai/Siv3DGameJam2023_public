#pragma once
#include "Spawner.h"
#include "LevelManager.h"
#include "Grobal.h"
#include "Util.h"

class SpawnTableManager {
public:
	static void Initialize() {
		LevelManager::AddOnSceneChangeEv([]([[maybe_unused]] Level previous,Level current) {
			if (current == Level::Title) {
				Reset();
			}
});
	}

	static void Reset() {
		s_timer = 0.0f;
		s_table_index = 0;
	}

	static void Update() {
#if _DEBUG
		Print << U"SpawnTableManager::m_table_index: " << s_table_index;
#endif

		if (!(LevelManager::GetLevel() == Level::Gaming)) { return; }

		s_timer += Scene::DeltaTime();

		if (kSpawnTable.size() <= s_table_index &&
			kGameClearTime <= s_timer && LevelManager::GetLevel() == Level::Gaming) {
			LevelManager::ChangeLevel(Level::ClearShooting);
		}

		while (s_table_index < kSpawnTable.size()) {
			if (kSpawnTable[s_table_index].interval <= s_timer) {
				Spawn( kSpawnTable[s_table_index]);
				s_table_index++;
				s_timer = 0;
			}
			else {
				break;
			}

		}
	}

	static void SetTableIndexMax() {
		s_timer = 0.0f;
		s_table_index = kSpawnTable.size();
	}

private:
	enum class EnemyType :unsigned char{
		Bird,
		Ghost,
	};

	struct SpawnInfo {
		constexpr SpawnInfo() = default;
		constexpr SpawnInfo(
			double _interval,
			EnemyType _type,
			const Vec2& _pos,
			const Vec2& _dir)
			:interval(_interval), type(_type), pos(_pos), dir(_dir) {
		}

		double interval = 0;
		EnemyType type = EnemyType::Bird;
		Vec2 pos = Vec2::Zero();
		Vec2 dir = Vec2::Zero();
	};

	static constexpr double kGameClearTime = 4.0f;

	static constexpr double kLeftSide = -5.0f;
	static constexpr double kRightSide = Grobal::kWindowSize.x + 5.0f;
	static constexpr double kTopSide = -5.0f;
	static constexpr double kBottomSide = Grobal::kWindowSize.y + 5.0f;
	static constexpr Vec2 kTopRight = Vec2(kRightSide, kTopSide);
	static constexpr Vec2 kTopLeft = Vec2(kLeftSide, kTopSide);
	static constexpr Vec2 kBottomRight = Vec2(kRightSide, kBottomSide);
	static constexpr Vec2 kBottomLeft = Vec2(kLeftSide, kBottomSide);

	static constexpr double kWidthOneThird = Grobal::kWindowSize.x / 3.0f;
	static constexpr double kWidthOneFifth = Grobal::kWindowSize.x / 5.0f;
	static constexpr double kHeightOneThird = Grobal::kWindowSize.y / 3.0f;
	static constexpr double kHeightOneFifth = Grobal::kWindowSize.y / 5.0f;

	static inline const std::array<SpawnInfo, 66>kSpawnTable = {
		SpawnInfo(1.0f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 1},Vec2::Right()),
		SpawnInfo(3.0f,EnemyType::Ghost,{kRightSide,kHeightOneFifth * 3},Vec2::Left()),

		SpawnInfo(4.0f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 1},Vec2::Right()),
		SpawnInfo(1.0f,EnemyType::Ghost,{kLeftSide,kHeightOneFifth * 3},Vec2::Right()),

		SpawnInfo(3.0f,EnemyType::Bird,kTopRight,Util::DownLeft()),
		SpawnInfo(0.0f,EnemyType::Bird,kTopLeft,Util::DownRight()),

		SpawnInfo(1.0f,EnemyType::Bird,kBottomRight,Util::UpLeft()),
		SpawnInfo(0.0f,EnemyType::Bird,kBottomLeft,Util::UpRight()),

		SpawnInfo(5.0f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 1},Vec2::Right()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kLeftSide,kHeightOneFifth * 2},Vec2::Right()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kLeftSide,kHeightOneFifth * 3},Vec2::Right()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kLeftSide,kHeightOneFifth * 4},Vec2::Right()),

		SpawnInfo(5.0f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 1},Vec2::Left()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kRightSide,kHeightOneFifth * 2},Vec2::Left()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kRightSide,kHeightOneFifth * 3},Vec2::Left()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kRightSide,kHeightOneFifth * 4},Vec2::Left()),

		SpawnInfo(3.0f,EnemyType::Bird,{kLeftSide, kHeightOneThird * 1},Vec2::Right()),
		SpawnInfo(0.0f,EnemyType::Bird,{kLeftSide,kHeightOneThird * 2},Vec2::Right()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneThird * 1,kTopSide},Vec2::Down()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneThird * 2,kTopSide},Vec2::Down()),

		SpawnInfo(3.0f,EnemyType::Bird,kTopRight,Util::DownLeft()),
		SpawnInfo(0.0f,EnemyType::Bird,kTopLeft,Util::DownRight()),
		SpawnInfo(0.0f,EnemyType::Bird,kBottomRight,Util::UpLeft()),
		SpawnInfo(0.0f,EnemyType::Bird,kBottomLeft,Util::UpRight()),

		SpawnInfo(3.0f,EnemyType::Ghost,{kRightSide, kHeightOneThird * 1},Vec2::Left()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kRightSide,kHeightOneThird * 2},Vec2::Left()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneThird * 1,kBottomSide},Vec2::Up()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneThird * 2,kBottomSide},Vec2::Up()),

		SpawnInfo(3.0f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 1},Vec2::Left()),
		SpawnInfo(0.25f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 2},Vec2::Left()),
		SpawnInfo(0.25f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 3},Vec2::Left()),
		SpawnInfo(0.25f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 4},Vec2::Left()),
		SpawnInfo(0.25f,EnemyType::Bird,kBottomRight,Util::UpLeft()),
		SpawnInfo(0.25f,EnemyType::Bird,{kWidthOneFifth * 4,kBottomSide},Vec2::Up()),
		SpawnInfo(0.25f,EnemyType::Bird,{kWidthOneFifth * 3,kBottomSide},Vec2::Up()),
		SpawnInfo(0.25f,EnemyType::Bird,{kWidthOneFifth * 2,kBottomSide},Vec2::Up()),
		SpawnInfo(0.25f,EnemyType::Bird,{kWidthOneFifth* 1,kBottomSide},Vec2::Up()),
		SpawnInfo(0.25f,EnemyType::Bird,kBottomLeft,Util::UpRight()),
		SpawnInfo(0.25f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 4},Vec2::Right()),
		SpawnInfo(0.25f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 3},Vec2::Right()),
		SpawnInfo(0.25f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 2},Vec2::Right()),
		SpawnInfo(0.25f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 1},Vec2::Right()),
		SpawnInfo(0.25f,EnemyType::Bird,kTopLeft,Util::DownRight()),
		SpawnInfo(0.25f,EnemyType::Bird,{kWidthOneThird * 1,kTopSide},Vec2::Down()),
		SpawnInfo(0.25f,EnemyType::Bird,{kWidthOneThird * 2,kTopSide},Vec2::Down()),
		SpawnInfo(0.25f,EnemyType::Bird,{kWidthOneThird * 3,kTopSide},Vec2::Down()),
		SpawnInfo(0.25f,EnemyType::Bird,{kWidthOneThird * 4,kTopSide},Vec2::Down()),
		SpawnInfo(0.25f,EnemyType::Bird,kTopRight,Util::DownLeft()),

		SpawnInfo(3.0f,EnemyType::Bird,{kWidthOneFifth * 4,kBottomSide},Vec2::Up()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneFifth * 3,kBottomSide},Vec2::Up()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneFifth * 2,kBottomSide},Vec2::Up()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneFifth * 1,kBottomSide},Vec2::Up()),
		SpawnInfo(0.0f,EnemyType::Bird,kBottomLeft,Util::UpRight()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 4},Vec2::Right()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 3},Vec2::Right()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 2},Vec2::Right()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kLeftSide, kHeightOneFifth * 1},Vec2::Right()),

		SpawnInfo(3.0f,EnemyType::Bird,{kWidthOneThird * 1,kTopSide},Vec2::Down()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneThird * 2,kTopSide},Vec2::Down()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneThird * 3,kTopSide},Vec2::Down()),
		SpawnInfo(0.0f,EnemyType::Bird,{kWidthOneThird * 4,kTopSide},Vec2::Down()),
		SpawnInfo(0.0f,EnemyType::Bird,kTopRight,Util::DownLeft()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 1},Vec2::Left()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 2},Vec2::Left()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 3},Vec2::Left()),
		SpawnInfo(0.0f,EnemyType::Ghost,{kRightSide, kHeightOneFifth * 4},Vec2::Left())

	};

	static void Spawn( const SpawnInfo& info) {
		switch (info.type)
		{
		case EnemyType::Bird:
			Spawner::CreateBird( info.pos, info.dir);
			break;
		case EnemyType::Ghost:
			Spawner::CreateGhost( info.pos, info.dir.x);
			break;
		default:
			assert(false);
			break;
		}
	}

	static inline double s_timer = 0;
	static inline size_t s_table_index = 0;
};
