#pragma once
#include "Fighter.h"

struct StateData
{
	StateData(AssetNameView _tex,Vec2 _offset,double _scale):tex(_tex),offset(_offset),scale(_scale){}

	TextureAsset tex;
	Vec2 offset = Vec2::Zero();
	double scale = 1.0;

	String ToSS()const {
		return U"{" + ToString(offset.x) + U"," + ToString(offset.y) + U"}" + U"," + ToString(scale);
	}
};

class FightersData {
public:
	using CharacterTex = std::array<StateData, std::to_underlying(Fighter::State::Length)>;

	static void Initialize() {
		s_characters_textures.reserve(std::to_underlying(Fighter::Character::Length));
		s_characters_textures.emplace_back(CharacterTex{
			StateData{kDefaultTex,{10,0},0.2},
			StateData{U"kaeru_panch",{50,10},0.25},
			StateData{U"kaeru_jumping",{-20,0},0.25},
			StateData{U"kaeru_jump_attack",Vec2::Zero(),0.25},
			StateData{U"kaeru_damaged",Vec2::Zero(),0.2},
			StateData{U"kaeru_damaged_air",Vec2::Zero(),0.2},
			StateData{U"kaeru_hadoken",{10,10},0.2},
			StateData{U"kaeru_syouryuken",Vec2::Zero(),0.3}
		});
		s_characters_textures.emplace_back(CharacterTex{
			StateData{U"usagi_idle",Vec2::Zero(),0.4},
			StateData{U"usagi_panch",{10,-10},0.225},
			StateData{U"usagi_jumping",Vec2::Zero(),0.175},
			StateData{U"usagi_jump_attack",Vec2::Zero(),0.2},
			StateData{U"usagi_damaged",Vec2::Zero(),0.3},
			StateData{U"usagi_damaged_air",Vec2::Zero(),0.2},
			StateData{U"usagi_hadoken",Vec2::Zero(),0.175},
			StateData{U"usagi_syouryuken",Vec2::Zero(),0.2}
		});
	}

	static constexpr AssetNameView GetDefaultTex() { return kDefaultTex; }

	static const StateData& GetCharactersTex(Fighter::Character chara, Fighter::State state) {
		return GetCharactersTexRef(chara,state);
	}

#if _DEBUG
	static void EditData(Fighter& fighter) {
		auto& tex_data = FightersData::GetCharactersTexRef(fighter.chara, fighter.state);

		if (KeyO.down()) {
			tex_data.scale += 0.05;
		}
		else if (KeyL.down()) {
			tex_data.scale -= 0.05;
		}

		if (KeyI.down()) {
			auto index = std::min(std::to_underlying(fighter.state) + 1,std::to_underlying(Fighter::State::Length) - 1);
			fighter.state = static_cast<Fighter::State>(index);
		}
		else if (KeyK.down()) {
			auto index = std::max(std::to_underlying(fighter.state) - 1, 0);
			fighter.state = static_cast<Fighter::State>(index);
		}

		if (KeyU.down()) {
			tex_data.offset.x += 10;
		}
		else if (KeyJ.down()) {
			tex_data.offset.x -= 10;
		}

		if (KeyY.down()) {
			tex_data.offset.y += 10;
		}
		else if (KeyH.down()) {
			tex_data.offset.y -= 10;
		}

		if (KeyP.down()) {
			fighter.chara =
				fighter.chara == Fighter::Character::Kaeru ?
				Fighter::Character::Usagi :
				Fighter::Character::Kaeru;
		}

		if (SimpleGUI::Button(U"Log_s_characters_textures", { 100,400 })) {
			LogData();
		}

		Print << Fighter::kCharacterNames[std::to_underlying(fighter.chara)] << U","
			<< Fighter::kStateNames[std::to_underlying(fighter.state)] << U","
			<< tex_data.ToSS();
	}

	static void LogData() {
		for (const auto& chara : s_characters_textures)
		{
			for (const auto& tex : chara)
			{
				Logger << tex.ToSS();
			}
		}
	}
#endif

private:
	static constexpr AssetNameView kDefaultTex = U"kaeru_idle";

	static StateData& GetCharactersTexRef(Fighter::Character chara, Fighter::State state) {
		auto chara_index = std::to_underlying(chara);
		auto state_index = std::to_underlying(state);
		return s_characters_textures[chara_index][state_index];
	}

	static inline std::vector<CharacterTex> s_characters_textures;
};
