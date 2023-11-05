#pragma once
#include "Fighter.h"
#include "Grobal.h"
#include "LevelManager.h"
#include "FighterAttack.h"
#include "FighterAI.h"
#include "CircleTrigger.h"

class FightSystem {
public:
	static void Initialize() {
		LevelManager::AddOnSceneChangeEv([]([[maybe_unused]]Level previous, Level current) {
			if (current == Level::Fight) {
				constexpr double kWidthOneThird = Grobal::kWindowSize.x / 3;
				constexpr Vec2 kPlayerStartPos = { kWidthOneThird,kFloorY - kFighterSize };
				Spawner::CreatePlayerFighter(kPlayerStartPos, 1);
				constexpr Vec2 kAutoPlayerStartPos = { 2 * kWidthOneThird,kFloorY - kFighterSize };
				Spawner::CreateAIFighter(kAutoPlayerStartPos, -1);
			}});
	}

	static void Update() {

		auto view = Grobal::s_registry.view<Fighter>();
		for (auto fighter_entity : view)
		{
			if (!Grobal::s_registry.valid(fighter_entity)) { continue; }

			auto& fighter = Grobal::s_registry.get<Fighter>(fighter_entity);
			FighterAI* fighter_ai = Grobal::s_registry.try_get<FighterAI>(fighter_entity);
			if (fighter_ai) {
				UpdateAIFighter(fighter_entity, fighter, *fighter_ai);
			}
			else {

#if _DEBUG
				FightersData::EditData(fighter);
				if (Key0.down()) {
					s_cant_update_player = !s_cant_update_player;
				}
#endif
				UpdatePlayerFighter(fighter_entity, fighter);
			}
		}
		UpdatehitEraseAttack();
	}

private:
	static constexpr double kWalkSpeed = 300.0;
	static constexpr double kJumpSpeed = 1400.0;
	static constexpr double kSyouryukenSpeed = 1300.0;
	static constexpr double kFloorY = 820.0;
	static constexpr double kFighterSize = 50;
	static constexpr double kGravity = 3000;
	static constexpr Vec2 kNormalizedRightJumpDir = { 0.3484,-0.93735 };
	static constexpr Vec2 kNormalizedLeftJumpDir = { -0.3484,-0.93735 };
	static constexpr double kAttackTime = 0.6;
	static constexpr double kJumpAttackTime = 0.5;
	static constexpr double kDamagedLaunchSpeed = 1000.0;
	static constexpr double kDamagedHorizontalSpeed = 800.0;
	static constexpr double kDamagedHorizontalDrag = 100.0;
	static constexpr double kDamagedTime = 0.25;
	static constexpr double kHadokenTime = 1.25;

	static void UpdatePlayerFighter(entt::entity entity, Fighter& fighter) {
		auto& pos = Grobal::s_registry.get<Position>(entity);
		auto& scale = Grobal::s_registry.get<Scale>(entity);
		auto& sprite = Grobal::s_registry.get<SpriteRender>(entity);
		auto& vel = Grobal::s_registry.get<Velocity>(entity);
		auto& timer = Grobal::s_registry.get<ElapsedTimer>(entity);
		auto& trigger = Grobal::s_registry.get<CircleTrigger>(entity);

		auto target_entity = FindAtherFighter(entity);
		if (!Grobal::s_registry.valid(target_entity)) { return; }
		auto& target_pos = Grobal::s_registry.get<Position>(target_entity);
		bool is_mirrored = IsMirrored(target_pos, fighter, sprite, pos);

		Vec2 to_mouse = Cursor::PosF() - pos.pos;
		to_mouse.normalize();
		Print << U"to_mouse" << to_mouse;

# if _DEBUG
		if (!s_cant_update_player)
#endif
		{
			DamagedUpdate(fighter, vel, trigger, timer,false);

			if (fighter.state == Fighter::State::Idle) {
				UpdateIdle(entity, fighter, pos, vel, timer, is_mirrored);
			}
			else if (fighter.state == Fighter::State::PunchAttack) {
				UpdatePunchAttack(fighter, timer);
			}
			else if (fighter.state == Fighter::State::Jumping) {
				UpdateJumping(entity, fighter, pos, vel, timer, is_mirrored);
			}
			else if (fighter.state == Fighter::State::JumpAttack) {
				UpdateJumpAttack(fighter, pos, vel, timer);
			}
			else if (fighter.state == Fighter::State::Damaged) {
				UpdateDamaged(fighter, vel, timer);
			}
			else if (fighter.state == Fighter::State::DamagedAir) {
				UpdateDamagedAir(fighter, pos, vel, timer);
			}
			else if (fighter.state == Fighter::State::Hadoken) {
				UpdateHadoken(fighter, timer);
			}
			else if (fighter.state == Fighter::State::Syouryuken) {
				UpdateSyoryuken(fighter, vel, timer);
			}
		}

		LimitUpdate(pos);
		SpriteUpdate(fighter, scale, sprite, is_mirrored);
	}


	static void UpdateAIFighter(entt::entity entity, Fighter& fighter, FighterAI& ai) {
		auto& pos = Grobal::s_registry.get<Position>(entity);
		auto& scale = Grobal::s_registry.get<Scale>(entity);
		auto& sprite = Grobal::s_registry.get<SpriteRender>(entity);
		auto& vel = Grobal::s_registry.get<Velocity>(entity);
		auto& timer = Grobal::s_registry.get<ElapsedTimer>(entity);
		auto& trigger = Grobal::s_registry.get<CircleTrigger>(entity);

		auto target_entity = FindAtherFighter(entity);
		if (!Grobal::s_registry.valid(target_entity)) { return; }
		auto& target_pos = Grobal::s_registry.get<Position>(target_entity);
		bool is_mirrored = IsMirrored(target_pos, fighter, sprite, pos);

		bool is_damaged = DamagedUpdate(fighter, vel, trigger, timer, true);
		if (is_damaged) {
			ai.LotteryPurpose();
		}

		LimitUpdate(pos);

		if (fighter.state == Fighter::State::Idle) {
			UpdateIdleAI(entity, fighter, ai, pos, vel, timer, is_mirrored, target_pos);
		}
		else if (fighter.state == Fighter::State::PunchAttack) {
			UpdatePunchAttack(fighter, timer, &ai);
		}
		else if (fighter.state == Fighter::State::Jumping) {
			UpdateJumpingAI(entity, fighter, ai, pos, vel, timer, is_mirrored, target_pos);
		}
		else if (fighter.state == Fighter::State::JumpAttack) {
			UpdateJumpAttack(fighter, pos, vel, timer, &ai);
		}
		else if (fighter.state == Fighter::State::Damaged) {
			UpdateDamaged(fighter, vel, timer, &ai);
		}
		else if (fighter.state == Fighter::State::DamagedAir) {
			UpdateDamagedAir(fighter, pos, vel, timer, &ai);
		}
		else if (fighter.state == Fighter::State::Hadoken) {
			UpdateHadoken(fighter, timer, &ai);
		}
		else if (fighter.state == Fighter::State::Syouryuken) {
			UpdateSyoryuken(fighter, vel, timer, &ai);
		}

		SpriteUpdate(fighter, scale, sprite, is_mirrored);
	}

	static void UpdatehitEraseAttack() {
		std::set<entt::entity> erase_entity;
		auto view = Grobal::s_registry.view<FighterAttack>();
		for (auto entity : view)
		{
			auto& attack = Grobal::s_registry.get<FighterAttack>(entity);
			auto& trigger = Grobal::s_registry.get<CircleTrigger>(entity);
			if (attack.hit_erase && Grobal::s_registry.valid(trigger.hit_target)) {
				auto* hit_attack = Grobal::s_registry.try_get<FighterAttack>(trigger.hit_target);
				if (hit_attack && hit_attack->hit_erase) {
					erase_entity.insert(entity);
					erase_entity.insert(trigger.hit_target);

					auto& pos = Grobal::s_registry.get<Position>(entity);
					auto& target_pos = Grobal::s_registry.get<Position>(trigger.hit_target);
					const auto midway = (pos.pos + target_pos.pos) / 2;
					Spawner::CreateExplocionVfx(midway);
				}
			}
		}

		for (auto entity : erase_entity) {
			Grobal::s_registry.destroy(entity);
		}
	}

	static bool DamagedUpdate(
		Fighter& fighter,
		Velocity& vel,
		CircleTrigger& trigger,
		ElapsedTimer& timer,
		bool is_enemy) {
		if (fighter.IsDamaged()) { return false; }
		if (!Grobal::s_registry.valid(trigger.hit_target)) { return false; }

		const FighterAttack* fighter_attack = Grobal::s_registry.try_get<FighterAttack>(trigger.hit_target);
		if (!fighter_attack) { return false; }

		if (fighter_attack->dir.y == 0 && !fighter.IsAir()) {
			fighter.state = Fighter::State::Damaged;
			ChangeState(fighter, Fighter::State::Damaged, timer);
			vel.vel = fighter_attack->dir.normalized() * kDamagedHorizontalSpeed;
		}
		else {
			ChangeState(fighter, Fighter::State::DamagedAir, timer);
			vel.vel = fighter_attack->dir.normalized() * kDamagedLaunchSpeed;
		}

		if (is_enemy) {
			UIManager::s_enemy_hp.damage(fighter_attack->damage);
			if (UIManager::s_enemy_hp.getHP() == 0) {
				LevelManager::ChangeLevel(Level::ClearFight);
			}
		}
		else {
			UIManager::s_player_hp.damage(fighter_attack->damage);
			if (UIManager::s_player_hp.getHP() == 0) {
				LevelManager::ChangeLevel(Level::GameOverFight);
			}
		}


		if (30 <= fighter_attack->damage) {
			SoundManager::PlaySE(U"punch1");
		}
		else {
			SoundManager::PlaySE(U"punch0");
		}

		if (Grobal::s_registry.valid(trigger.hit_target)) {
			Grobal::s_registry.destroy(trigger.hit_target);
			trigger.hit_target = entt::null;
		}
		return true;
	}

	static void UpdateIdle(entt::entity entity, Fighter& fighter, Position& pos, Velocity& vel, ElapsedTimer& timer, bool is_mirrored) {
		bool is_down_jump = KeyUp.down() || KeyW.down();
		bool is_pressed_right = KeyRight.pressed() || KeyD.pressed();
		bool is_pressed_left = KeyLeft.pressed() || KeyA.pressed();

		if (is_down_jump && is_pressed_right) {
			ChangeState(fighter, Fighter::State::Jumping, timer);
			vel.vel = kNormalizedRightJumpDir * kJumpSpeed;
		}
		else if (is_down_jump && is_pressed_left)
		{
			ChangeState(fighter, Fighter::State::Jumping, timer);
			vel.vel = kNormalizedLeftJumpDir * kJumpSpeed;
		}
		else if (is_down_jump)
		{
			ChangeState(fighter, Fighter::State::Jumping, timer);
			vel.vel = Vec2::Up() * kJumpSpeed;
		}
		else if (is_pressed_left)
		{
			vel.vel = Vec2::Left() * kWalkSpeed;
		}
		else if (is_pressed_right)
		{
			vel.vel = Vec2::Right() * kWalkSpeed;
		}
		else {
			vel.vel = Vec2::Zero();
		}

		if (MouseL.down()) {
			vel.vel = Vec2::Zero();
			ChangePunchAttack(entity, fighter, timer, is_mirrored, TriggerTag::PlayerAttack);
		}
		else if (MouseR.down()) {
			ChangeHadoken(fighter, pos, vel, timer, is_mirrored, TriggerTag::PlayerAttack);
		}
		else if (MouseM.down()) {
			vel.vel = kNormalizedRightJumpDir * kSyouryukenSpeed;
			vel.vel.x *= is_mirrored ? -1 : 1;
			ChangeSyoryuken(entity, fighter, timer, is_mirrored, TriggerTag::PlayerAttack);
		}
	}

	static void UpdateIdleAI(
		entt::entity entity,
		Fighter& fighter,
		FighterAI& ai,
		Position& pos,
		Velocity& vel,
		ElapsedTimer& timer,
		bool is_mirrored,
		const Position& target
	) {
		constexpr double kLimitSideOffset = 10;
		constexpr double kHadoRadius = 600;
		constexpr double kFlontJumpRadius = 550;
		constexpr double kFlontJumpRadiusRange = 20;
		constexpr double kAttackRadius = 100;
		constexpr int kPunchAttackProbability = 50;

		const auto target_dist = pos.pos.distanceFrom(target.pos);
		const double flont_dir = is_mirrored ? -1 : 1;
		const bool on_limit =
			Scene::Width() - kFighterSize - kLimitSideOffset <= pos.pos.x ||
			pos.pos.x <= kFighterSize + kLimitSideOffset;

#if _DEBUG
		Print << U"enemy_purpose" << FighterAI::kPurposeNames[ std::to_underlying(ai.purpose)];
		Circle{ target.pos, kHadoRadius }.drawFrame(10, Palette::Orange);
		Circle{ target.pos , kFlontJumpRadius - kFlontJumpRadiusRange }.drawFrame(10, Palette::Hotpink);
		Circle{ target.pos , kFlontJumpRadius + kFlontJumpRadiusRange }.drawFrame(10, Palette::Hotpink);
		Circle{ target.pos , kAttackRadius }.drawFrame(10, Palette::Crimson);
#endif

		if (ai.purpose == FighterAI::Purpose::AwayHado) {
			vel.vel = Vec2::Left() * kWalkSpeed * flont_dir;

			if (kHadoRadius <= target_dist || on_limit) {
				ChangeHadoken(fighter, pos, vel, timer, is_mirrored, TriggerTag::EnemyAttack,&ai);
				vel.vel = Vec2::Zero();
				ai.LotteryPurpose();
			}
		}
		else if (ai.purpose == FighterAI::Purpose::JumpAttack) {
			auto dest = -flont_dir * kFlontJumpRadius + target.pos.x;
			auto to_dest = dest - pos.pos.x;
			vel.vel = Vec2::Right() * kWalkSpeed * Util::Sign(to_dest);

			const auto jump_pos_min = dest - kFlontJumpRadiusRange;
			const auto jump_pos_max = dest + kFlontJumpRadiusRange;
			const bool is_ranging = jump_pos_min <= pos.pos.x && pos.pos.x <= jump_pos_max;
			if (is_ranging || on_limit) {
				ChangeState(fighter, Fighter::State::Jumping, timer, &ai);
				ai.LotteryPurpose();
				if (0 < flont_dir) {
					vel.vel = kNormalizedRightJumpDir * kJumpSpeed;
				}
				else {
					vel.vel = kNormalizedLeftJumpDir * kJumpSpeed;
				}
			}
		}
		else if (ai.purpose == FighterAI::Purpose::PunchAttack) {
			vel.vel = Vec2::Right() * kWalkSpeed * flont_dir;

			if (target_dist <= kAttackRadius) {
				if (ai.Check(kPunchAttackProbability)) {
					ChangePunchAttack(entity, fighter, timer, is_mirrored, TriggerTag::EnemyAttack, &ai);
					vel.vel = Vec2::Zero();
				}
				ai.LotteryPurpose();
			}
		}
	}

	static void UpdatePunchAttack(Fighter& fighter, ElapsedTimer& timer, FighterAI* ai = nullptr) {
		if (kAttackTime <= timer.elapsed_sum) {
			ChangeState(fighter, Fighter::State::Idle, timer, ai);
		}
	}

	static void UpdateJumping(
		entt::entity entity,
		Fighter& fighter,
		Position& pos,
		Velocity& vel,
		ElapsedTimer& timer,
		bool is_mirrored) {
		vel.vel.y += Scene::DeltaTime() * kGravity;

		if (kFloorY - kFighterSize <= pos.pos.y) {
			pos.pos.y = kFloorY - kFighterSize;
			ChangeState(fighter, Fighter::State::Idle, timer);
			vel.vel = Vec2::Zero();
		}
		else if (MouseL.down()) {
			ChangeJumpAttack(entity, fighter, timer, is_mirrored, TriggerTag::PlayerAttack);
		}
	}

	static void UpdateJumpingAI(
		entt::entity entity,
		Fighter& fighter,
		FighterAI& ai,
		Position& pos,
		Velocity& vel,
		ElapsedTimer& timer,
		bool is_mirrored,
		const Position& target
	) {
		constexpr double kAttackRadius = 240;
		constexpr int kAttackProbability = 80;

#if _DEBUG
		Circle{ pos.pos, kAttackRadius }.drawFrame(10, Palette::Aquamarine);
#endif

		vel.vel.y += Scene::DeltaTime() * kGravity;

		if (kFloorY - kFighterSize <= pos.pos.y) {
			pos.pos.y = kFloorY - kFighterSize;
			ChangeState(fighter, Fighter::State::Idle, timer, &ai);
			vel.vel = Vec2::Zero();
		}


		if (ai.Check(kAttackProbability) && pos.pos.distanceFrom(target.pos) <= kAttackRadius) {
			ChangeJumpAttack(entity, fighter, timer, is_mirrored, TriggerTag::EnemyAttack, &ai);
		}
	}

	static void UpdateJumpAttack(Fighter& fighter, Position& pos, Velocity& vel, ElapsedTimer& timer, FighterAI* ai = nullptr) {
		vel.vel.y += Scene::DeltaTime() * kGravity;

		if (kFloorY - kFighterSize <= pos.pos.y) {
			pos.pos.y = kFloorY - kFighterSize;
			ChangeState(fighter, Fighter::State::Idle, timer, ai);
			vel.vel = Vec2::Zero();
		}
		else if (kJumpAttackTime <= timer.elapsed_sum) {
			ChangeState(fighter, Fighter::State::Jumping, timer, ai);
		}
	}

	static void UpdateDamaged(Fighter& fighter, Velocity& vel, ElapsedTimer& timer, FighterAI* ai = nullptr) {
		if (std::signbit(vel.vel.x)) {
			vel.vel.x += Scene::DeltaTime() * kGravity;
			vel.vel.x = std::min(vel.vel.x, 0.0);
		}
		else {
			vel.vel.x -= Scene::DeltaTime() * kGravity;
			vel.vel.x = std::max(vel.vel.x, 0.0);
		}
		vel.vel.y = 0.0;

		if (kDamagedTime <= timer.elapsed_sum) {
			vel.vel = Vec2::Zero();
			ChangeState(fighter, Fighter::State::Idle, timer, ai);
		}
	}

	static void UpdateDamagedAir(Fighter& fighter, Position& pos, Velocity& vel, ElapsedTimer& timer, FighterAI* ai = nullptr) {
		vel.vel.y += Scene::DeltaTime() * kGravity;

		if (kFloorY - kFighterSize <= pos.pos.y) {
			pos.pos.y = kFloorY - kFighterSize;
			vel.vel.y = 0;
			ChangeState(fighter, Fighter::State::Idle, timer, ai);
		}
	}

	static void UpdateHadoken(Fighter& fighter, ElapsedTimer& timer, FighterAI* ai = nullptr) {
		if (kHadokenTime <= timer.elapsed_sum) {
			ChangeState(fighter, Fighter::State::Idle, timer, ai);
		}
	}

	static void UpdateSyoryuken(Fighter& fighter, Velocity& vel, ElapsedTimer& timer, FighterAI* ai = nullptr) {
		vel.vel.y += Scene::DeltaTime() * kGravity;

		if (0.0 <= vel.vel.y) {
			ChangeState(fighter, Fighter::State::Jumping, timer, ai);
		}
	}

	static void LimitUpdate(Position& pos) {
		pos.pos.x = Math::Clamp(pos.pos.x, kFighterSize, Scene::Width() - kFighterSize);
		pos.pos.y = Math::Clamp(pos.pos.y, kFighterSize, kFloorY - kFighterSize);
	}

	static void SpriteUpdate(const Fighter& fighter, Scale& scale, SpriteRender& sprite, bool is_mirrored) {
		const auto& tex_data = FightersData::GetCharactersTex(fighter.chara, fighter.state);
		sprite.texture_asset = tex_data.tex;
		sprite.offset = tex_data.offset;
		scale.scale = tex_data.scale * Vec2::One();
		sprite.is_mirrored = is_mirrored;
	}


	static bool IsMirrored(const Position& target_pos, const Fighter& fighter, const SpriteRender& sprite, const Position& pos) {
		bool is_mirrored = sprite.is_mirrored;
		if (fighter.IsAir()) {
			return is_mirrored;
		}

		Vec2 to_target = target_pos.pos - pos.pos;
		is_mirrored = to_target.x < 0.0;
		return is_mirrored;
	}

	static entt::entity FindAtherFighter(entt::entity self) {
		auto view = Grobal::s_registry.view<Fighter>();
		for (auto fighter_entity : view)
		{
			if (fighter_entity != self) {
				return fighter_entity;
			}
		}
		return entt::null;
	}

	static void ChangeState(Fighter& fighter, Fighter::State state, ElapsedTimer& timer, FighterAI* ai = nullptr) {
		fighter.state = state;
		timer.Reset();
		if (ai) { ai->Lottery(); }

		for (auto& trigger_entity : fighter.attack_triggers)
		{
			if (!Grobal::s_registry.valid(trigger_entity)) { continue; }
			if (Grobal::s_registry.all_of<CircleTrigger>(trigger_entity)) {
				Grobal::s_registry.destroy(trigger_entity);
			}
			trigger_entity = entt::null;
		}
	}

	static void ChangePunchAttack(
		entt::entity entity,
		Fighter& fighter,
		ElapsedTimer& timer,
		bool is_mirrored,
		TriggerTag tag,
		FighterAI* ai = nullptr) {
		constexpr Vec2 kKaeruLocalTriggerPos = { 160.0,-50.0 };
		constexpr Vec2 kUsagiLocalTriggerPos = { 120.0,-50.0 };
		constexpr double kTriggerRadius = 30.0;
		constexpr int kDamage = 5;

		ChangeState(fighter, Fighter::State::PunchAttack, timer, ai);

		Vec2 local_pos = fighter.chara == Fighter::Character::Kaeru ? kKaeruLocalTriggerPos : kUsagiLocalTriggerPos;
		Vec2 dir = is_mirrored ? Vec2::Left() : Vec2::Right();
		fighter.attack_triggers[0] = Spawner::CreateFighterAttackTrrigger(entity, local_pos, dir, kTriggerRadius, tag, kDamage);
	}

	static void ChangeHadoken(
		Fighter& fighter,
		const Position& pos,
		Velocity& vel,
		ElapsedTimer& timer,
		bool is_mirrored,
		TriggerTag tag,
		FighterAI* ai = nullptr
	) {
		int x_dir = is_mirrored ? -1 : 1;
		Spawner::CreateHado(tag, pos.pos, x_dir);
		vel.vel = Vec2::Zero();
		ChangeState(fighter, Fighter::State::Hadoken, timer,ai);
	}

	static void ChangeJumpAttack(
		entt::entity entity,
		Fighter& fighter,
		ElapsedTimer& timer,
		bool is_mirrored,
		TriggerTag tag,
		FighterAI* ai = nullptr) {
		constexpr Vec2 kKaeruLocalTriggerPos = { 120.0,100.0 };
		constexpr Vec2 kUsagiLocalTriggerPos = { 120.0,0.0 };
		constexpr double kTriggerRadius = 30.0;
		constexpr int kDamage = 10;

		ChangeState(fighter, Fighter::State::JumpAttack, timer, ai);

		Vec2 local_pos = fighter.chara == Fighter::Character::Kaeru ? kKaeruLocalTriggerPos : kUsagiLocalTriggerPos;
		Vec2 dir = is_mirrored ? Vec2::Left() : Vec2::Right();
		fighter.attack_triggers[0] = Spawner::CreateFighterAttackTrrigger(entity, local_pos, dir, kTriggerRadius, tag, kDamage);
	}

	static void ChangeSyoryuken(
		entt::entity entity,
		Fighter& fighter,
		ElapsedTimer& timer,
		bool is_mirrored,
		TriggerTag tag,
		FighterAI* ai = nullptr) {
		constexpr Vec2 kKaeruLocalTriggerPos = { 60.0,-160.0 };
		constexpr Vec2 kUsagiLocalTriggerPos = { 60.0,-120.0 };
		constexpr double kTriggerRadius = 30.0;
		constexpr int kDamage = 45;

		ChangeState(fighter, Fighter::State::Syouryuken, timer, ai);

		Vec2 local_pos = fighter.chara == Fighter::Character::Kaeru ? kKaeruLocalTriggerPos : kUsagiLocalTriggerPos;
		Vec2 dir = is_mirrored ? Vec2::Left() : Vec2::Right();
		fighter.attack_triggers[0] = Spawner::CreateFighterAttackTrrigger(entity, local_pos, dir, kTriggerRadius, tag, kDamage);
	}

# if _DEBUG
	static inline bool s_cant_update_player = false;
#endif
};
