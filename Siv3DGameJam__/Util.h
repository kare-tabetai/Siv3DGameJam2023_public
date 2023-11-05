#pragma once

class Util {
public:
	static consteval Vec2 UpRight() {
		return Vec2(kRootHalf, -kRootHalf);
	}
	static consteval Vec2 UpLeft() {
		return Vec2(-kRootHalf, -kRootHalf);
	}
	static consteval Vec2 DownRight() {
		return Vec2(kRootHalf, kRootHalf);
	}
	static consteval Vec2 DownLeft() {
		return Vec2(-kRootHalf, kRootHalf);
	}

	static double GetAngle(Vec2 dir) {
		//ゼロ除算防止,浮動小数点誤差は考慮しない
		if (dir.x == 0.0) {
			return static_cast<double>(0 < dir.y ? Math::HalfPi : -Math::HalfPi);
		}

		double tan = dir.y / dir.x;
		return static_cast<double>(std::atan(tan));
	}

	template <class T>
	static auto Sign(T&& val) {
		bool b =  std::signbit(val);
		return b ? -1 : 1;
	}


private:
	static constexpr double kRootHalf = 0.7071f;
};
