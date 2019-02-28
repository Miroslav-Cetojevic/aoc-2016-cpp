#include <fstream>
#include <iostream>
#include <string>

#include "unordered_map.hpp"

struct Position {
	std::intmax_t x = 0;
	std::intmax_t y = 0;
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
			return std::bit_xor{}(get_hash(pos.x), get_hash(pos.y));
		}
	};
}

constexpr auto MIN = -1L;
constexpr auto MAX = 1L;

auto up(Position& pos) {
	if(pos.y + 1 <= MAX) { ++pos.y; }
}

auto down(Position& pos) {
	if(pos.y - 1 >= MIN) { --pos.y; }
}

auto left(Position& pos) {
	if(pos.x - 1 >= MIN) { --pos.x; }
}

auto right(Position& pos) {
	if(pos.x + 1 <= MAX) { ++pos.x; }
}

int main() {

	auto filename = std::string{"instructions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		using cmd = void (*) (Position&);
		auto cmd_map = ska::flat_hash_map<char, cmd>{
			{'U', up},
			{'D', down},
			{'L', left},
			{'R', right}
		};

		auto key_map = ska::flat_hash_map<Position, char> {
			{{-1,1}, '1'},
			{{0,1}, '2'},
			{{1,1}, '3'},
			{{-1,0}, '4'},
			{{0,0}, '5'},
			{{1,0}, '6'},
			{{-1,-1}, '7'},
			{{0,-1}, '8'},
			{{1,-1}, '9'}
		};

		auto code = std::string{};
		auto position = Position{};

		std::string line;
		while(file >> line) {

			for(const auto step : line) { cmd_map[step](position); }

			code.push_back(key_map[position]);
		}

		std::cout << code << std::endl;

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
