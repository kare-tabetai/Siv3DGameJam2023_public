#pragma once
#include "Position.h"
#include "Rotation.h"
#include "Velocity.h"
#include "Render.h"
#include "Scale.h"
#include "FixedJoint.h"
#include "Player.h"
#include "Util.h"
#include "Bullet.h"
#include "CircleTrigger.h"
#include "Enemy.h"
#include "ElapsedTimer.h"
#include "LifeTime.h"
#include "SinMover.h"
#include "FightersData.h"
#include "Fighter.h"
#include "FighterAttack.h"
#include "FighterAI.h"
#include "SoundManager.h"

class Spawner {
public:

	static entt::entity CreateCircle() {
		auto entity = Grobal::s_registry.create();
		auto& pos = Grobal::s_registry.emplace<Position>(entity);
		double x = Random(0.0, static_cast<double>(Scene::Width()));
		double y = Random(0.0, static_cast<double>(Scene::Height()));
		pos.pos = { x,y };
		auto& vel = Grobal::s_registry.emplace<Velocity>(entity);
		vel.vel = RandomVec2() * Random(0, 200);
		auto& circle = Grobal::s_registry.emplace<CircleRender>(entity);
		circle.col = RandomColorF();
		circle.r = Random(1.0f, 100.0f);
		circle.thickness = Random(1.0f, 5.0f);

		return entity;
	}

	static entt::entity CreateSprite(
		AssetNameView texture_name,
		const Vec2& pos,
		const Vec2& scale,
		double rot = 0.0f,
		bool is_mirrored = false,
		int priority = 0) {

		if (!TextureAsset::IsReady(texture_name)) {
			TextureAsset::Load(texture_name);
		}

		auto entity = Grobal::s_registry.create();
		auto& pos_cd = Grobal::s_registry.emplace<Position>(entity);
		pos_cd.pos = pos;
		auto& rot_cd = Grobal::s_registry.emplace<Rotation>(entity);
		rot_cd.rot = rot;
		auto& scale_cd = Grobal::s_registry.emplace<Scale>(entity);
		scale_cd.scale = scale;
		auto& sprite = Grobal::s_registry.emplace<SpriteRender>(entity, texture_name);
		sprite.is_mirrored = is_mirrored;
		sprite.priority = priority;

		return entity;
	}

	static void CreateWasi() {
		Spawner::CreateSprite(U"wasi", Scene::Center(), Vec2::One(), 0.0f, false, -10);
	}

	static void CreateBackGrounds() {
		CreateWasi();
		Spawner::CreateSprite(U"hill00", { 150,Scene::Height() - 100 }, Vec2::One() * 0.7f, 0.0f, false, -8);
		Spawner::CreateSprite(U"hill00", { 500,Scene::Height() - 60 }, Vec2::One() * 0.2f, 0.0f, false, -8);
		Spawner::CreateSprite(U"hill01", { 950,Scene::Height() - 100 }, Vec2::One() * 0.6f, 0.0f, false, -8);
	}

	static void CreateFightBackGrounds() {
		CreateWasi();
		Spawner::CreateSprite(U"ground", { Scene::Width() * 1 / 5,Scene::Height() - 20 }, Vec2::One() * 0.5f, 0.0f, false, 5);
		Spawner::CreateSprite(U"ground", { Scene::Width() * 2 / 5,Scene::Height() - 20 }, Vec2::One() * 0.5f, 0.0f, false, 5);
		Spawner::CreateSprite(U"ground", { Scene::Width() * 4 / 5,Scene::Height() - 20 }, Vec2::One() * 0.5f, 0.0f, false, 5);

		Spawner::CreateSprite(U"grass1", { Scene::Width() * 2 / 10,Scene::Height() - 30 }, Vec2::One() * 0.1f, 0.0f, false, 5);
		Spawner::CreateSprite(U"grass0", { Scene::Width() * 4 / 10,Scene::Height() - 30 }, Vec2::One() * 0.1f, 0.0f, false, 5);
		Spawner::CreateSprite(U"grass2", { Scene::Width() * 4 / 10,Scene::Height() - 30 }, Vec2::One() * 0.1f, 0.0f, false, 5);

		Spawner::CreateSprite(U"tree", { Scene::Width() * 4 / 5,Scene::Height() - 300 }, Vec2::One() * 0.6f, 0.0f, false, -8);
		Spawner::CreateSprite(U"rock", { Scene::Width() * 1 / 5,Scene::Height() - 100 }, Vec2::One() * 0.2f, 0.0f, true, -8);
	}

	static entt::entity CreatePlayerBullet(const Vec2& pos, const Vec2& dir) {
		constexpr AssetNameView kTextureName = U"bullet";
		constexpr Vec2 kScale = Vec2::One() * 0.15f;
		constexpr double kSpeed = 1000.0f;
		constexpr double kTriggerRadius = 25.0f;

		bool is_mirrored = 0 < dir.x ? false : true;
		auto entity = CreateSprite(kTextureName, pos, kScale, Util::GetAngle(dir), is_mirrored);

		Grobal::s_registry.emplace<Bullet>(entity);

		auto& vel = Grobal::s_registry.emplace<Velocity>(entity);
		vel.vel = dir.normalized() * kSpeed;

		auto& trigger = Grobal::s_registry.emplace<CircleTrigger>(entity);
		trigger.tag = TriggerTag::PlayerAttack;
		trigger.r = kTriggerRadius;

		Grobal::s_registry.emplace<ElapsedTimer>(entity);
		auto& life_time = Grobal::s_registry.emplace<LifeTime>(entity);
		life_time.life_time = 20.0f;

		SoundManager::PlaySE(U"shot");

		return entity;
	}

	static entt::entity CreateHand(const Vec2& parent_pos) {
		constexpr Vec2 kScale = Vec2::One() * 0.15f;
		constexpr AssetNameView kTextureName = U"kaeru_hand";

		auto entity = CreateSprite(kTextureName, parent_pos, kScale, 0.0f, false, -1);
		return entity;
	}

	static entt::entity CreatePlayer(const Vec2& pos) {
		constexpr AssetNameView kTextureName = U"kaeru";
		constexpr AssetNameView kLoupTextureName = U"loup";
		constexpr Vec2 kScale = Vec2::One() * 0.15f;
		constexpr Vec2 kLoupLocalPos = Vec2::Up() * 400 + Vec2::Right() * 10;
		constexpr double kTriggerRadius = 30.0f;

		auto player_entity = CreateSprite(kTextureName, pos, kScale);
		auto hand_entity = CreateHand(pos);

		auto loup_entity = CreateSprite(kLoupTextureName, pos + kLoupLocalPos, kScale, 0.0f, false, -2);
		auto& loup_joint = Grobal::s_registry.emplace<FixedJoint>(loup_entity);
		loup_joint.parent = player_entity;
		loup_joint.local_pos = kLoupLocalPos;
		loup_joint.is_mirror_active = true;

		auto& player = Grobal::s_registry.emplace<Player>(player_entity);
		player.hand = hand_entity;
		player.loup = loup_entity;

		Grobal::s_registry.emplace<Velocity>(player_entity);

		auto& trigger = Grobal::s_registry.emplace<CircleTrigger>(player_entity);
		trigger.r = kTriggerRadius;
		trigger.tag = TriggerTag::Player;

		return player_entity;
	}

	static entt::entity CreateBird(const Vec2& pos, const Vec2& dir) {
		constexpr AssetNameView kTextureName = U"bird";
		constexpr Vec2 kScale = Vec2::One() * 0.08f;
		constexpr double kSpeed = 300.0f;
		constexpr double kTriggerRadius = 30.0f;

		auto entity = Spawner::CreateBaseEnemy(pos, dir, kSpeed, kScale, kTriggerRadius, kTextureName);

		return entity;
	}

	static entt::entity CreateGhost(const Vec2& pos, double x_dir) {
		constexpr AssetNameView kTextureName = U"ghost";
		constexpr Vec2 kScale = Vec2::One() * 0.08f;
		constexpr double kSpeed = 400.0f;
		constexpr double kTriggerRadius = 30.0f;
		constexpr double kSicleRate = 0.5f;
		constexpr double kHeight = 1.0f;

		auto entity = Spawner::CreateBaseEnemy(
			pos,
			Vec2::Right() * x_dir,
			kSpeed,
			kScale,
			kTriggerRadius,
			kTextureName);

		auto& sin_mover = Grobal::s_registry.emplace<SinMover>(entity);
		sin_mover.cycle_rate = kSicleRate;
		sin_mover.height = kHeight;
		sin_mover.speed = kSpeed;

		return entity;
	}

	static entt::entity CreateExplocionVfx(const Vec2& pos) {
		constexpr AssetNameView kTextureName = U"explosion";
		constexpr Vec2 kScale = Vec2::One() * 0.15f;
		constexpr double kLifeTime = 1.0f;

		auto entity = CreateSprite(kTextureName, pos, kScale);

		Grobal::s_registry.emplace<ElapsedTimer>(entity);
		auto& life_time = Grobal::s_registry.emplace<LifeTime>(entity);
		life_time.life_time = kLifeTime;

		SoundManager::PlaySE(U"explosion");

		return entity;
	}

	static entt::entity CreatePlayerFighter(const Vec2& pos,int x_dir) {
		constexpr AssetNameView kTextureName = FightersData::GetDefaultTex();
		constexpr Vec2 kScale = Vec2::One() * 0.15f;

		auto entity = CreateSprite(kTextureName, pos, kScale, 0.0f, x_dir < 0);

		Grobal::s_registry.emplace<Velocity>(entity);

		auto fighter = Grobal::s_registry.emplace<Fighter>(entity);
		fighter.chara = Fighter::Character::Kaeru;

		Grobal::s_registry.emplace<ElapsedTimer>(entity);

		auto& trigger = Grobal::s_registry.emplace<CircleTrigger>(entity);
		trigger.r = 60;
		trigger.tag = TriggerTag::Player;
		return entity;
	}

	static entt::entity CreateAIFighter(const Vec2& pos, int x_dir) {
		constexpr AssetNameView kTextureName = FightersData::GetDefaultTex();
		constexpr Vec2 kScale = Vec2::One() * 0.15f;

		auto entity = CreateSprite(kTextureName, pos, kScale, 0.0f, x_dir < 0);

		Grobal::s_registry.emplace<Velocity>(entity);

		auto& fighter = Grobal::s_registry.emplace<Fighter>(entity);
		fighter.chara = Fighter::Character::Usagi;

		Grobal::s_registry.emplace<FighterAI>(entity);

		Grobal::s_registry.emplace<ElapsedTimer>(entity);

		auto& trigger = Grobal::s_registry.emplace<CircleTrigger>(entity);
		trigger.r = 60;
		trigger.tag = TriggerTag::Enemy;

		return entity;
	}

	static entt::entity CreateHado(TriggerTag tag, const Vec2& pos, int x_dir) {
		constexpr AssetNameView kTextureName = U"bullet";
		constexpr Vec2 kScale = Vec2::One() * 0.3f;
		constexpr double kSpeed = 400.0;
		constexpr double kTriggerRadius = 50.0f;
		constexpr int kDamage = 5;

		bool is_mirrored = 0 < x_dir ? false : true;
		auto entity = CreateSprite(kTextureName, pos, kScale, 0.0, is_mirrored);

		Grobal::s_registry.emplace<Bullet>(entity);

		auto& vel = Grobal::s_registry.emplace<Velocity>(entity);
		vel.vel = Vec2::Right() * x_dir * kSpeed;

		auto& trigger = Grobal::s_registry.emplace<CircleTrigger>(entity);
		trigger.tag = tag;
		trigger.r = kTriggerRadius;

		Grobal::s_registry.emplace<ElapsedTimer>(entity);

		auto& life_time = Grobal::s_registry.emplace<LifeTime>(entity);
		life_time.life_time = 20.0;

		auto& fighter_attack = Grobal::s_registry.emplace<FighterAttack>(entity);
		fighter_attack.dir = Vec2::Right() * x_dir;
		fighter_attack.hit_erase = true;
		fighter_attack.damage = kDamage;

		SoundManager::PlaySE(U"shot");

		return entity;
	}

	static entt::entity CreateFighterAttackTrrigger(
		entt::entity parent,
		const Vec2& local_pos,
		const Vec2& dir,
		double r,
		TriggerTag tag,
		int damage) {
		auto entity = Grobal::s_registry.create();

		Grobal::s_registry.emplace<Position>(entity);

		auto& trigger = Grobal::s_registry.emplace<CircleTrigger>(entity);
		trigger.r = r;
		trigger.tag = tag;

		auto& attack = Grobal::s_registry.emplace<FighterAttack>(entity);
		attack.dir = dir;
		attack.damage = damage;

		auto& joint = Grobal::s_registry.emplace<FixedJoint>(entity);
		joint.is_mirror_active = true;
		joint.parent = parent;
		joint.local_pos = local_pos;

		return entity;
	}


private:
	static entt::entity CreateBaseEnemy(
		const Vec2& pos,
		const Vec2& dir,
		double speed,
		const Vec2& scale,
		double radius,
		AssetNameView asset_name) {
		bool is_mirrored = dir.x < 0;
		auto entity = Spawner::CreateSprite(asset_name, pos, scale, 0.0f, is_mirrored);

		Grobal::s_registry.emplace<Enemy>(entity);

		auto& vel = Grobal::s_registry.emplace<Velocity>(entity);
		vel.vel = dir.normalized() * speed;

		auto& trigger = Grobal::s_registry.emplace<CircleTrigger>(entity);
		trigger.tag = TriggerTag::Enemy;
		trigger.r = radius;

		Grobal::s_registry.emplace<ElapsedTimer>(entity);
		auto& life_time = Grobal::s_registry.emplace<LifeTime>(entity);
		life_time.life_time = 20.0f;

		return entity;
	}
};
