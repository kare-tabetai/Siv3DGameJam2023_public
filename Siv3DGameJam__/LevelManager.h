#pragma once

enum class Level :unsigned char {
	Title,
	Gaming,
	GameOverShooting,
	ClearShooting,
	Event,
	Fight,
	ClearFight,
	GameOverFight,
};

class LevelManager {
public:
	using OnSceneChangeEv = std::function<void(Level,Level)>;

	static void Initilaize() {
		
	}

	static void Update() {
#if _DEBUG
		Print << U"Level:" << std::to_underlying(s_level);
#endif

		if (s_level == Level::Title) {
			if (MouseL.down()) {
				ChangeLevel(Level::Gaming);
			}
		}
		else if (s_level == Level::GameOverShooting && !s_is_paused) {
			if (MouseL.down()) {
				ChangeLevel(Level::Title);
			}
		}
		else if (s_level == Level::ClearShooting && !s_is_paused) {
			s_timer += Scene::DeltaTime();
			if (kShootingClearWaitTime <= s_timer) {
				ChangeLevel(Level::Fight);
			}
		}
		else if (s_level == Level::GameOverFight && !s_is_paused) {
			if (MouseL.down()) {
				ChangeLevel(Level::Title);
			}
		}
		else if (s_level == Level::ClearFight && !s_is_paused) {
			if (MouseL.down()) {
				ChangeLevel(Level::Title);
			}
		}

		if (LevelManager::s_level != Level::Title && KeySpace.down()) {
			LevelManager::ChangePause();
		}
	}

	static void ChangePause() {
		if (s_level == Level::Title && s_is_paused == false) {
			return;
		}
		if (s_level == Level::GameOverShooting && s_is_paused == false) {
			return;
		}

		s_is_paused = !s_is_paused;
	}

	static void ChangeLevel(Level lv) {
		OnChangedScene(s_level, lv);
		s_level = lv;
	}

	static bool IsPausing() { return s_is_paused; }
	static bool IsShooting() {
		return s_level == Level::Gaming || s_level == Level::Title;
	}
	static Level GetLevel() { return s_level; }

	static void AddOnSceneChangeEv(OnSceneChangeEv&& ev) {
		s_on_change_scene.emplace_back(std::move(ev));
	}

private:
	static constexpr double kShootingClearWaitTime = 3.0f;

	static void OnChangedScene(Level previous,Level current) {
		for (auto& ev : s_on_change_scene)
		{
			ev(previous, current);
		}
	}

	static inline Level s_level = Level::Title;
	static inline bool s_is_paused = false;
	static inline std::vector<OnSceneChangeEv> s_on_change_scene;
	static inline double s_timer = 0;
};
