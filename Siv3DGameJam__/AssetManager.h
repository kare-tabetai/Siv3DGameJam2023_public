#pragma once

class AssetManager {
public:
	static void Initialize() {
		InitializeSound();
		InitializeShootingTex();
		InitializeEventTex();
		InitializeFightTex();

		TextureAsset::Register(U"owari", Resource(U"asset/owari.png"));

		FontAsset::Register(U"ShipporiMincho",FontMethod::MSDF ,50, Resource(U"asset/sippori/ShipporiMincho-Regular.otf"));
	}

private:
	static void InitializeSound() {
		AudioAsset::Register(U"KuniNakusi", Resource(U"asset/sound/KuniNakusi.mp3"));
		AudioAsset::Register(U"ZetuHomura", Resource(U"asset/sound/ZetuHomura.mp3"));
		AudioAsset::Register(U"ICE_CAVE", Resource(U"asset/sound/ICE_CAVE.mp3"));

		AudioAsset::Register(U"taiko", Resource(U"asset/sound/taiko.mp3"));
		AudioAsset::Register(U"explosion", Resource(U"asset/sound/explosion.mp3"));
		AudioAsset::Register(U"punch0", Resource(U"asset/sound/punch0.mp3"));
		AudioAsset::Register(U"punch1", Resource(U"asset/sound/punch1.mp3"));
		AudioAsset::Register(U"shot", Resource(U"asset/sound/shot.mp3"));

	}

	static void InitializeShootingTex() {
		TextureAsset::Register(U"bird", Resource(U"asset/bird.png"));
		TextureAsset::Register(U"kaeru", Resource(U"asset/kaeru.png"));
		TextureAsset::Register(U"kaeru_hand", Resource(U"asset/kaeru_hand.png"));
		TextureAsset::Register(U"loup", Resource(U"asset/loup.png"));
		TextureAsset::Register(U"bullet", Resource(U"asset/bullet.png"));
		TextureAsset::Register(U"explosion", Resource(U"asset/explosion.png"));
		TextureAsset::Register(U"ghost", Resource(U"asset/ghost.png"));
		TextureAsset::Register(U"wasi", Resource(U"asset/wasi.png"));

		TextureAsset::Register(U"cloud00", Resource(U"asset/cloud00.png"));
		TextureAsset::Register(U"cloud01", Resource(U"asset/cloud01.png"));
		TextureAsset::Register(U"cloud02", Resource(U"asset/cloud02.png"));

		TextureAsset::Register(U"hill00", Resource(U"asset/hill00.png"));
		TextureAsset::Register(U"hill01", Resource(U"asset/hill01.png"));
	}

	static void InitializeEventTex() {
		TextureAsset::Register(U"usagi_erasou", Resource(U"asset/event/usagi_erasou.png"));
		TextureAsset::Register(U"kaeru_odoroki", Resource(U"asset/event/kaeru_odoroki.png"));

		TextureAsset::Register(U"kaeru_koshinite", Resource(U"asset/event/kaeru_koshinite.png"));
		TextureAsset::Register(U"usagi_ushiro", Resource(U"asset/event/usagi_ushiro.png"));
		TextureAsset::Register(U"kaeru_event_fight", Resource(U"asset/event/kaeru_event_fight.png"));
	}

	static void InitializeFightTex() {
		TextureAsset::Register(U"kaeru_idle", Resource(U"asset/fight/kaeru_idle.png"));
		TextureAsset::Register(U"usagi_idle", Resource(U"asset/fight/usagi_idle.png"));

		TextureAsset::Register(U"kaeru_panch", Resource(U"asset/fight/kaeru_panch.png"));
		TextureAsset::Register(U"usagi_panch", Resource(U"asset/fight/usagi_panch.png"));

		TextureAsset::Register(U"kaeru_jumping", Resource(U"asset/fight/kaeru_jumping.png"));
		TextureAsset::Register(U"usagi_jumping", Resource(U"asset/fight/usagi_jumping.png"));

		TextureAsset::Register(U"kaeru_jump_attack", Resource(U"asset/fight/kaeru_jump_attack.png"));
		TextureAsset::Register(U"usagi_jump_attack", Resource(U"asset/fight/usagi_jump_attack.png"));

		TextureAsset::Register(U"kaeru_damaged", Resource(U"asset/fight/kaeru_damaged.png"));
		TextureAsset::Register(U"usagi_damaged", Resource(U"asset/fight/usagi_damaged.png"));

		TextureAsset::Register(U"kaeru_damaged_air", Resource(U"asset/fight/kaeru_damaged_air.png"));
		TextureAsset::Register(U"usagi_damaged_air", Resource(U"asset/fight/usagi_damaged_air.png"));

		TextureAsset::Register(U"kaeru_hadoken", Resource(U"asset/fight/kaeru_hadoken.png"));
		TextureAsset::Register(U"usagi_hadoken", Resource(U"asset/fight/usagi_hadoken.png"));

		TextureAsset::Register(U"kaeru_syouryuken", Resource(U"asset/fight/kaeru_syouryuken.png"));
		TextureAsset::Register(U"usagi_syouryuken", Resource(U"asset/fight/usagi_syouryuken.png"));

		TextureAsset::Register(U"grass0", Resource(U"asset/fight/fight_back/grass0.png"));
		TextureAsset::Register(U"grass1", Resource(U"asset/fight/fight_back/grass1.png"));
		TextureAsset::Register(U"grass2", Resource(U"asset/fight/fight_back/grass2.png"));
		TextureAsset::Register(U"ground", Resource(U"asset/fight/fight_back/ground.png"));
		TextureAsset::Register(U"tree", Resource(U"asset/fight/fight_back/tree.png"));
		TextureAsset::Register(U"rock", Resource(U"asset/fight/fight_back/rock.png"));

	}
};
