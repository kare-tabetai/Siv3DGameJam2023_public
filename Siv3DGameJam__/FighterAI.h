#pragma once

struct FighterAI {
	enum class Purpose :int {
		AwayHado,
		JumpAttack,
		PunchAttack,
		Length,
	};
	static constexpr std::array< StringView,std::to_underlying(Purpose::Length)> kPurposeNames = {
		U"AwayHado",
		U"JumpAttack",
		U"PunchAttack",
	};

	void Lottery() {
		for (auto& item : current_state_random_table)
		{
			item = Random(0, 100);
		}
	}
	void LotteryPurpose() {
		int index = Random(0, static_cast<int>(Purpose::Length) - 1);
		purpose = static_cast<Purpose>(index);
	}

	bool Check(int probability, size_t index = 0) {
		assert(0 <= probability && probability <= 100);
		assert(index == 0 || index == 1);
		return current_state_random_table[index] <= probability;
	}

	Purpose purpose = Purpose::AwayHado;
	std::array<int, 2> current_state_random_table = { 0,0 };
};
