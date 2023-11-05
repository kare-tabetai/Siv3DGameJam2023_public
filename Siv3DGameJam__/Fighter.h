#pragma once

struct Fighter {
	enum class Character :unsigned char {
		Kaeru,
		Usagi,
		Length,
	};
	static constexpr std::array<StringView, std::to_underlying(Character::Length) >kCharacterNames = {
		U"Kaeru",
		U"Usagi"
	};

	enum class State :unsigned char {
		Idle,
		PunchAttack,
		Jumping,
		JumpAttack,
		Damaged,
		DamagedAir,
		Hadoken,
		Syouryuken,
		Length,
	};
	static constexpr std::array<StringView, std::to_underlying(State::Length) >kStateNames = {
		U"Idle",
		U"PanchAttack",
		U"Jumping",
		U"JumpAttack",
		U"Damaged",
		U"DamagedAir",
		U"Hadoken",
		U"Syouryuken"
	};

	bool IsAir()const {
		return state == State::DamagedAir || state == State::Jumping || state == State::JumpAttack || state == State::Syouryuken;
	}
	bool IsDamaged()const {
		return state == State::Damaged || state == State::DamagedAir;
	}

	Character chara = Character::Kaeru;
	State state = State::Idle;
	std::array<entt::entity,2> attack_triggers = { entt::null,entt::null };
};
