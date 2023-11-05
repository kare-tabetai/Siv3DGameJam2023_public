#pragma once
#include "LevelManager.h"

class SoundManager {
public:
	static void Initialize() {
		LevelManager::AddOnSceneChangeEv([]([[maybe_unused]] Level previous, Level current) {
			if (current == Level::Title) {
				PlayBGM(U"KuniNakusi");
			}
			else if (current == Level::Gaming) {
				PlaySE(U"taiko");
			}
			else if (current == Level::Event) {
				PlayBGM(U"ICE_CAVE");
			}
			else if (current == Level::Fight) {
				PlayBGM(U"ZetuHomura");
			}
			}
		);
	}

	static void SetSoudSetting() {
		//AudioAsset(U"ICE_CAVE").setLoop(true);
		//AudioAsset(U"ICE_CAVE").setVolume(0.25);
		//AudioAsset(U"KuniNakusi").setLoop(true);
		//AudioAsset(U"KuniNakusi").setVolume(0.25);
		//AudioAsset(U"ZetuHomura").setLoop(true);
		//AudioAsset(U"ZetuHomura").setVolume(0.25);
	}

	static void PlayBGM(StringView str) {
		if (AudioAsset(str).isPlaying()) { return; }

		FadeAllBGM();
		SetSoudSetting();
		//AudioAsset(str).play();
	}
	static void PlaySE(StringView str) {

		double volume = 1.0;
		if (str == U"shot") {
			volume = 0.3;
		}

	}
		//AudioAsset(str).playOneShot(volume);

private:
	static void FadeAllBGM() {
		//AudioAsset(U"ICE_CAVE").stop(0.2s);
		//AudioAsset(U"KuniNakusi").stop(0.2s);
		//AudioAsset(U"ZetuHomura").stop(0.2s);
	}
};
