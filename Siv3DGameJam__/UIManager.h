#pragma once
#include "LevelManager.h"
#include "SpawnTableManager.h"
#include "GameUIUtil.h"

class UIManager {
public:
	static constexpr int kMaxHP = 100;

	static void Initialize() {
		LevelManager::AddOnSceneChangeEv([](Level previous, [[maybe_unused]] Level current) {
			if (previous == Level::Fight) {
				s_player_hp.setHP(kMaxHP);
				s_enemy_hp.setHP(kMaxHP);
			}
			if (current == Level::ClearFight || current == Level::GameOverFight) {
				Grobal::s_registry.clear();
			}

			});
		LicenseInfo ent;
		ent.title = U"EnTT";
		ent.copyright = U"Copyright(c) 2017 - 2023 Michele Caini, author of EnTT";
		ent.text = U"https://github.com/skypjack/entt";
		LicenseManager::AddLicense(ent);
	}

	static void Update() {
		if (KeyF10.down()) {
			bool is_fullscreen = Window::GetState().fullscreen;
			Window::SetFullscreen(!is_fullscreen);
		}
	}

	static void Draw() {
		if (LevelManager::GetLevel() == Level::Title) {
			const Vec2 title_pos = { Scene::Center().x,Grobal::kWindowSize.y * 2 / 5 };
			FontAsset(U"ShipporiMincho")(U"くものいと").draw(120, Arg::bottomCenter = title_pos, Palette::Black);

			const Vec2 kClickToStartPos = { Scene::Center().x,Grobal::kWindowSize.y * 4 / 5 };
			FontAsset(U"ShipporiMincho")(U"ひだり くりっく とぅ すたーと").draw(40, Arg::bottomCenter = kClickToStartPos, Palette::Black);
		}
		else if (LevelManager::GetLevel() == Level::GameOverShooting) {
			FontAsset(U"ShipporiMincho")(U"げえむ おーばー").draw(80, Arg::bottomCenter = Scene::Center(), Palette::Black);
			const Vec2 kclickToStartPos = { Scene::Center().x,Grobal::kWindowSize.y * 4 / 5 };
			FontAsset(U"ShipporiMincho")(U"ひだり くりっく とぅ たいとる").draw(40, Arg::bottomCenter = kclickToStartPos, Palette::Black);
		}
		else if (LevelManager::GetLevel() == Level::Fight) {
			constexpr Rect player_rect = Rect{
				Grobal::kWindowSize.x / 10,
				Grobal::kWindowSize.y / 20,
				Grobal::kWindowSize.x * 2 / 10,
				32 };
			s_player_hp.draw(player_rect);

			constexpr Rect enemy_rect = Rect{
				Grobal::kWindowSize.x * 6 / 10,
				Grobal::kWindowSize.y / 20,
				 Grobal::kWindowSize.x * 2 / 10,
				32 };
			s_enemy_hp.draw(enemy_rect);
		}
		else if (LevelManager::GetLevel() == Level::GameOverFight) {
			FontAsset(U"ShipporiMincho")(U"げえむ おーばー").draw(80, Arg::bottomCenter = Scene::Center(), Palette::White);
			const Vec2 kclickToStartPos = { Scene::Center().x,Grobal::kWindowSize.y * 4 / 5 };
			FontAsset(U"ShipporiMincho")(U"ひだり くりっく とぅ たいとる").draw(40, Arg::bottomCenter = kclickToStartPos, Palette::White);
		}
		else if (LevelManager::GetLevel() == Level::ClearFight) {
			TextureAsset(U"owari").scaled(0.3).drawAt(Grobal::kWindowSize.x / 2, Grobal::kWindowSize.y / 2);
			const Vec2 kclickToStartPos = { Scene::Center().x,Grobal::kWindowSize.y * 4 / 5 };
			FontAsset(U"ShipporiMincho")(U"ひだり くりっく とぅ たいとる").draw(40, Arg::bottomCenter = kclickToStartPos, Palette::White);
		}

		if (LevelManager::IsPausing()) {
			FontAsset(U"ShipporiMincho")(U"ぽおず ちゅう").draw(Arg::bottomCenter = Scene::Center(), Palette::Black);
		}

		if (SimpleGUI::Button(U"Licenses", Vec2{ 20, 20 }))
		{
			// ライセンス情報を表示
			LicenseManager::ShowInBrowser();
		}

#if _DEBUG
		s_player_hp.update();
		s_enemy_hp.update();
		if (SimpleGUI::Button(U"SetTableIndexMax", { 100,100 })) {
			SpawnTableManager::SetTableIndexMax();
		}
		if (SimpleGUI::Button(U"ToFightScene", { 100,150 })) {
			LevelManager::ChangeLevel(Level::Fight);
		}

		if (SimpleGUI::Button(U"ToEventScene", { 100,200 })) {
			LevelManager::ChangeLevel(Level::Event);
		}
#endif
	}

	static inline HPBar s_player_hp = HPBar{ kMaxHP };
	static inline HPBar s_enemy_hp = HPBar{ kMaxHP };
};
