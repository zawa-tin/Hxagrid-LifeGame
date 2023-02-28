# include <Siv3D.hpp> // OpenSiv3D v0.6.6

class HxaLifeGame {
private:
	static constexpr ColorF livingColor{ Palette::Aquamarine };
	static constexpr ColorF deathColor{ Palette::Black };
	static constexpr double radius{ 10 };
	const Array<int32> delta{ -1, 0, 1 };
	Grid<Vec2> pos;
	Grid<bool> state;
	const Font font{ FontMethod::MSDF, 12 };

	bool isIn(int32 y, int32 x) {
		return 0 <= y and y < state.height() and 0 <= x and x < state.width();
	}

	int32 count(int32 y, int32 x) {
		int32 res = 0;
		for (const int32& dy : delta) {
			for (const int32& dx : delta) {
				if (dy != dx) {
					res += isIn(y + dy, x + dx) and state[y + dy][x + dx];
				}
			}
		}
		return res;
	}

public:

	HxaLifeGame(size_t h = 100, size_t w = 100) : pos(w, h), state(w, h) {	
		Vec2 center{ w / 2, h / 2 };
		for (auto y : step(h)) {
			for (auto x : step(w)) {
				Vec2 diff = Vec2{ x, y } - center;
				pos[y][x] = Scene::Center() + 2 * radius * Math::Cos(30_deg) * Vec2{ (diff.x + diff.y * Math::Cos(60_deg)), diff.y * Math::Sin(60_deg) };
				state[y][x] = RandomBool();
			}
		}
	}

	void draw() const {
		for (auto y : step(pos.height())) {
			for (auto x : step(pos.width())) {
				Shape2D::Hexagon(radius, pos[y][x]).draw(state[y][x] ? livingColor : deathColor);
				//font(U"{}"_fmt(Vec2{ x, y })).drawAt(pos[y][x]);
			}
		}
	}

	void update() {
		Grid<bool> next(state);
		for (auto y : step(state.height())) {
			for (auto x : step(state.width())) {
				int32 cnt = count(y, x);
				if (state[y][x]) {
					next[y][x] = 1 <= cnt and cnt <= 2;
				}
				else {
					next[y][x] = cnt == 2;
				}
			}
		}
		state = move(next);
	}

	void restart() {
		for (auto y : step(state.height())) {
			for (auto x : step(state.width())) {
				state[y][x] = RandomBool();
			}
		}
	}
};

void Main() {
	HxaLifeGame hxalifegame;
	double time = 0.0;
	while (System::Update()) {
		hxalifegame.draw();
		time += Scene::DeltaTime();
		if (time > 0.25) {
			hxalifegame.update();
			time -= 0.25;
		}
		if (KeyR.pressed()) {
			hxalifegame.restart();
		}
	}
}
