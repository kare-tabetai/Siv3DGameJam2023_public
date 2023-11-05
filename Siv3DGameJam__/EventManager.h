#pragma once
#include "LevelManager.h"
#include "SoundManager.h"
#include "Grobal.h"

class EventManager {
private:
	static constexpr double kInputInterval = 0.5;
	static constexpr int kMaxIndex = 6;
	static inline int s_event_index = 0;
	static inline double s_timer = 0;

public:
	static void Initialize() {
		LevelManager::AddOnSceneChangeEv([]([[maybe_unused]] Level previous, Level current) {
			if (current == Level::Event) {
				Grobal::s_registry.clear();
				Spawner::CreateWasi();
				s_event_index = 0;
				s_timer = 0;
			}
			});
	}

	static void Update() {
		EventUpdate();
	}

	static void EventUpdate() {
		if (LevelManager::GetLevel() != Level::Event) { return; }

		s_timer += Scene::DeltaTime();
		if (kInputInterval <= s_timer && MouseL.down()) {
			SoundManager::PlaySE(U"taiko");
			s_event_index++;
			s_timer = 0;
			if (kMaxIndex <= s_event_index) {
				LevelManager::ChangeLevel(Level::Fight);
			}
			if (s_event_index == 5) {
				SoundManager::PlayBGM(U"ZetuHomura");
			}
		}

		if (MouseR.down()) {
			LevelManager::ChangeLevel(Level::Fight);
		}
	}

	static void Draw() {
		EventDraw();
	}

	static void EventDraw() {
		if (LevelManager::GetLevel() != Level::Event) { return; }

		constexpr Vec2 kTextPos = { Grobal::kWindowSize.x / 2,Grobal::kWindowSize.y * 9 / 10 };
		constexpr Vec2 kSkipTextPos = { Grobal::kWindowSize.x,0 };

		if (s_event_index == 0 || s_event_index == 1 || s_event_index == 2) {
			TextureAsset(U"usagi_erasou").scaled(0.3).drawAt(300, 450);
			TextureAsset(U"kaeru_odoroki").scaled(0.5).drawAt(800, 550);
		}
		else if (s_event_index == 3 || s_event_index == 4) {
			TextureAsset(U"kaeru_koshinite").scaled(0.3).drawAt(300, 450);
			TextureAsset(U"usagi_ushiro").scaled(0.5).drawAt(800, 550);
		}
		else if (s_event_index == 5) {
			TextureAsset(U"kaeru_event_fight").scaled(0.3).drawAt(300, 450);
			TextureAsset(U"usagi_ushiro").scaled(0.5).drawAt(800, 550);
		}

		Rect top_rect = {
			0,0,Grobal::kWindowSize.x,Grobal::kWindowSize.y / 5
		};
		top_rect.draw(Palette::Black);
		Rect bottom_rect = {
			0,Grobal::kWindowSize.y * 4 / 5,Grobal::kWindowSize.x,Grobal::kWindowSize.y / 5
		};
		bottom_rect.draw(Palette::Black);

		FontAsset(U"ShipporiMincho")(U"みぎ くりっく とぅ すきっぷ")
			.draw(30, Arg::topRight = kSkipTextPos);

		if (s_event_index == 0) {
			FontAsset(U"ShipporiMincho")(U"うさぎ:\nよくぞ じごくから 脱出しましたね\nわたしが 糸をたらして \"たすけてあげた\"のですよ")
				.draw(40, Arg::center = kTextPos, Palette::White);
		}
		else if (s_event_index == 1) {
			FontAsset(U"ShipporiMincho")(U"うさぎ:\nあなたが 必死に糸を上る姿 とても感動的でしたよ")
				.draw(40, Arg::center = kTextPos, Palette::White);
		}
		else if (s_event_index == 2) {
			FontAsset(U"ShipporiMincho")(U"うさぎ:\nご褒美に この天界で わたしに仕える仕事を与えましょう")
				.draw(40, Arg::bottomCenter = kTextPos, Palette::White);
		}
		else if (s_event_index == 3) {
			FontAsset(U"ShipporiMincho")(U"かえる:\n\"たすけてあげた\"か 随分と偉いんだな")
				.draw(40, Arg::bottomCenter = kTextPos, Palette::White);
		}
		else if (s_event_index == 4) {
			FontAsset(U"ShipporiMincho")(U"かえる:\nおれの必死な姿を 娯楽にしていただけだろう?")
				.draw(40, Arg::bottomCenter = kTextPos, Palette::White);
		}
		else if (s_event_index == 5) {
			FontAsset(U"ShipporiMincho")(U"かえる:\n下につくのは あんたのほうだ 覚悟はいいか!")
				.draw(40, Arg::bottomCenter = kTextPos, Palette::White);
		}
	}
};
