#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

struct Position {
	std::int64_t x = 0;
	std::int64_t y = 0;
};

auto operator==(const Position& lhs, const Position& rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

template<typename T>
auto get_hash(T t) { return std::hash<T>{}(t); }

namespace std {
	template <>
	struct hash<Position>{
		auto operator()(const Position& pos) const {
			return (get_hash(pos.x) ^ get_hash(pos.y));
		}
	};
}

int main() {

	const auto filename = std::string{"instructions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		const auto move_to_key = [] (auto condition, auto& dimension, auto modifier) {
			if(condition) {
				dimension += modifier;
			}
		};

		// we treat the keypad as a coordinate system
		const auto keypad = std::unordered_map<Position, char> {
			{{-1, 1}, '1'}, {{0, 1}, '2'}, {{1, 1}, '3'},
			{{-1, 0}, '4'}, {{0, 0}, '5'}, {{1, 0}, '6'},
			{{-1,-1}, '7'}, {{0,-1}, '8'}, {{1,-1}, '9'}
		};

		auto code = std::string{};

		auto pos = Position{};
		auto& x = pos.x;
		auto& y = pos.y;

		// no coordinate can go beyond 1 in
		// either positive or negative direction
		constexpr auto MIN = std::int64_t{-1};
		constexpr auto MAX = std::int64_t{1};

		std::string line;

		while(file >> line) {

			for(const auto move : line) {

				switch(move) {
					case 'U': move_to_key((y < MAX), y,  1); break;
					case 'D': move_to_key((y > MIN), y, -1); break;
					case 'L': move_to_key((x > MIN), x, -1); break;
					case 'R': move_to_key((x < MAX), x,  1); break;
					default: std::cout << "WTF?!?" << std::endl;
				}
			}

			code.push_back(keypad.find(pos)->second);
		}

		std::cout << code << std::endl;

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
