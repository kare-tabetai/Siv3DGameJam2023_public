#pragma once

struct CircleRender {
	ColorF col = Palette::Black;
	double r = 1.0f;
	double thickness = 1.0f;
};

struct SpriteRender {
	SpriteRender(AssetNameView name) :texture_asset(name) {}

	Vec2 offset = Vec2::Zero();
	bool is_mirrored = false;
	int priority = 0;
	TextureAsset texture_asset;
};
