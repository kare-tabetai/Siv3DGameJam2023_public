#pragma once
#include <bitset>
#include <array>

enum class TriggerTag:unsigned char {
	Player,
	Enemy,
	PlayerAttack,
	EnemyAttack,
	Length,
};
inline constexpr size_t kTriggerTagLength = static_cast<size_t>(TriggerTag::Length);

inline constexpr std::array<std::bitset<kTriggerTagLength>, kTriggerTagLength> kTriggerMask = {
	//EPEP
	  0b1010,//P
	  0b0101,//E
	  0b1010,//P
	  0b0101//E
};

constexpr bool IsAbleToHit(TriggerTag lhs, TriggerTag rhs) {
	return kTriggerMask[std::to_underlying(lhs)].test(std::to_underlying(rhs));
}

struct CircleTrigger {
	double r = 1.0f;
	entt::entity hit_target = entt::null;
	TriggerTag tag;
};
