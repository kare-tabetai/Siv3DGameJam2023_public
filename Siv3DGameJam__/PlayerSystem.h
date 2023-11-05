#pragma once
#include "Util.h"
#include "Position.h"
#include "Rotation.h"
#include "Scale.h"
#include "Velocity.h"
#include "Render.h"
#include "Player.h"
#include "FixedJoint.h"
#include "CircleTrigger.h"
#include "LevelManager.h"
#include "Spawner.h"

class PlayerSystem {
public:
	static void Initialize() {
		LevelManager::AddOnSceneChangeEv([]([[maybe_unused]] Level previous, Level current) {
			if (current == Level::Title) {
				Spawner::CreatePlayer(Scene::Center() + Vec2::Down() * 120.0f);
			}});
	}

	static void Update() {
		if (LevelManager::IsShooting()) {
			UpdateShooting();
		}
		else if (LevelManager::GetLevel() == Level::ClearShooting) {
			UpdateClearShooting();
		}
	}

private:
	static constexpr double kSpeed = 500.0f;
	static constexpr Vec2 kBaseHandPos = { 10,-30 };
	static constexpr double kHandLength = 25;
	static constexpr double kShotPosLength = 50;

	static void MoveUpdate(Velocity& vel) {

		Vec2 dir = Vec2::Zero();

		if (KeyLeft.pressed() || KeyA.pressed())
		{
			dir.x = -1;
		}
		else if (KeyRight.pressed() || KeyD.pressed())
		{
			dir.x = 1;
		}
		else {
			dir.x = 0;
		}

		if (KeyUp.pressed() || KeyW.pressed())
		{
			dir.y = -1;
		}
		else if (KeyDown.pressed() || KeyS.pressed())
		{
			dir.y = 1;
		}
		else {
			dir.y = 0;
		}

		if (!dir.isZero()) {
			dir.normalize();
		}
		vel.vel = dir * kSpeed;

	}

	static void LimitUpdate(Position& pos) {
		constexpr int kSize = 50;
		pos.pos.x = Math::Clamp(pos.pos.x, kSize, Scene::Width() - kSize);
		pos.pos.y = Math::Clamp(pos.pos.y, kSize, Scene::Height() - kSize);
	}

	static void UpdateHand(bool is_look_left, entt::entity hand_entity,const Vec2& hand_dir,const Vec2& player_pos) {
		Vec2 hand_pos = kBaseHandPos;
		hand_pos.x = is_look_left ? -hand_pos.x : hand_pos.x;

		auto [hadnd_pos, hand_rot, hand_sprite] = Grobal::s_registry.get<Position, Rotation, SpriteRender>(hand_entity);
		hadnd_pos.pos = player_pos + hand_pos + hand_dir * kHandLength;
		hand_rot.rot = Util::GetAngle(hand_dir);
		hand_sprite.is_mirrored = is_look_left;
	}

	static void UpdateShooting() {
		entt::entity dead_player = entt::null;
		auto view = Grobal::s_registry.view<Player>();
		for (auto entity : view) {
			Position& pos = Grobal::s_registry.get<Position>(entity);

			CircleTrigger& trigger = Grobal::s_registry.get<CircleTrigger>(entity);
			if (trigger.hit_target != entt::null) {
				bool is_hit_enemy = Grobal::s_registry.all_of<Enemy>(trigger.hit_target);
				if (is_hit_enemy) {
					Spawner::CreateExplocionVfx(pos.pos);
					dead_player = entity;
					break;
				}
			}

			Velocity& vel = Grobal::s_registry.get<Velocity>(entity);
			MoveUpdate(vel);
			LimitUpdate(pos);

			Vec2 hand_direction = Cursor::PosF() - pos.pos;
			hand_direction.normalize();
			Player& player = Grobal::s_registry.get<Player>(entity);
			player.look_left = hand_direction.x < 0;

			SpriteRender& sprite = Grobal::s_registry.get<SpriteRender>(entity);
			sprite.is_mirrored = player.look_left;

			UpdateHand(player.look_left, player.hand,hand_direction,pos.pos);

			if (MouseL.down())
			{
				Vec2 hand_pos = kBaseHandPos;
				hand_pos.x = player.look_left ? -hand_pos.x : hand_pos.x;
				const auto shot_pos = pos.pos + hand_pos + hand_direction * kShotPosLength;
				Spawner::CreatePlayerBullet(shot_pos, hand_direction);
			}
		}

		if (dead_player != entt::null) {
			Player& player = Grobal::s_registry.get<Player>(dead_player);
			auto hand = player.hand;
			auto loup = player.loup;
			Grobal::s_registry.destroy(loup);
			Grobal::s_registry.destroy(hand);
			Grobal::s_registry.destroy(dead_player);

			LevelManager::ChangeLevel(Level::GameOverShooting);
		}
	}

	static void UpdateClearShooting() {
		for (auto [entity,player,pos,vel] : Grobal::s_registry.view<Player, Position,Velocity>().each())
		{
			vel.vel = Vec2::Up()* kSpeed;
			player.look_left = false;
			UpdateHand(false, player.hand, Vec2::Right(),pos.pos);
		}
	}
};
