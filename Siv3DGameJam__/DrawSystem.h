#pragma once
#include "Position.h"
#include "Scale.h"
#include "Rotation.h"
#include "Render.h"
#include "CircleTrigger.h"

class DrawSystem {

public:
	static void Update() {
		Grobal::s_registry.sort<SpriteRender>(
			[](const SpriteRender& lhs, const SpriteRender& rhs) {
				return lhs.priority < rhs.priority;
			});

		auto view = Grobal::s_registry.view<SpriteRender>();
		for (auto entity : view) {

			auto sprite = Grobal::s_registry.get<SpriteRender>(entity);

			Vec2 pos = sprite.offset;
			if (sprite.is_mirrored) { pos.x *= -1; }
			double rot = 0.0f;
			Vec2 scale = Vec2::One();

			if (auto* pos_cd = Grobal::s_registry.try_get<Position>(entity)) {
				pos += pos_cd->pos;
			}
			if (auto* rot_cd = Grobal::s_registry.try_get<Rotation>(entity)) {
				rot = rot_cd->rot;
			}
			if (auto* scale_cd = Grobal::s_registry.try_get<Scale>(entity)) {
				scale = scale_cd->scale;
			}

			//Print << U"priority" << sprite.priority;
			sprite.texture_asset
				.scaled(scale)
				.mirrored(sprite.is_mirrored)
				.rotated(rot)
				.drawAt(pos);
		}

		Grobal::s_registry.view<Position, CircleRender>().each([](const Position& pos, const CircleRender& circle) {
			Circle{ pos.pos, circle.r }.drawFrame(circle.thickness, circle.col);});

#if _DEBUG
		Grobal::s_registry.view<Position, CircleTrigger>().each([](const Position& pos, const CircleTrigger& trigger) {
			ColorF col = Grobal::s_registry.valid(trigger.hit_target) ? Palette::Red : Palette::White;
			Circle{ pos.pos, trigger.r }.drawFrame(1, col);
	});
#endif // DEBUG
	}
};
