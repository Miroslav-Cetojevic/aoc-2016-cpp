#include <fstream>
#include <iostream>
#include <string>

#include "flat_hash_map.hpp"

struct Position {
	std::intmax_t x = -2;
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

struct Support {
	bool can_go_up;
	bool can_go_down;
	bool can_go_left;
	bool can_go_right;
};

template<typename P, typename B>
auto up(P& pos, B& boundaries) { if(boundaries[pos].can_go_up) { ++pos.y; } }

template<typename P, typename B>
auto down(P& pos, B& boundaries) { if(boundaries[pos].can_go_down) { --pos.y; } }

template<typename P, typename B>
auto left(P& pos, B& boundaries) { if(boundaries[pos].can_go_left) { --pos.x; } }

template<typename P, typename B>
auto right(P& pos, B& boundaries) { if(boundaries[pos].can_go_right) { ++pos.x; } }

int main() {

	auto filename = std::string{"instructions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		using Boundaries = ska::flat_hash_map<Position, Support>;
		auto boundaries = Boundaries{
			{{0,2}, {false,true,false,false}},
			{{-1,1}, {false,true,false,true}},
			{{0,1}, {true,true,true,true}},
			{{1,1}, {false,true,true,false}},
			{{-2,0}, {false,false,false,true}},
			{{-1,0}, {true,true,true,true}},
			{{0,0}, {true,true,true,true}},
			{{1,0}, {true,true,true,true}},
			{{2,0}, {false,false,true,false}},
			{{-1,-1}, {true,false,false,true}},
			{{0,-1}, {true,true,true,true}},
			{{1,-1}, {true,false,true,false}},
			{{0,-2}, {true,false,false,false}}
		};

		using cmd = void (*) (Position&, Boundaries&);
		auto cmd_map = ska::flat_hash_map<char, cmd>{
			{'U', up},
			{'D', down},
			{'L', left},
			{'R', right}
		};

		auto key_map = ska::flat_hash_map<Position, char> {
			{{0,2}, '1'},
			{{-1,1}, '2'},
			{{0,1}, '3'},
			{{1,1}, '4'},
			{{-2,0}, '5'},
			{{-1,0}, '6'},
			{{0,0}, '7'},
			{{1,0}, '8'},
			{{2,0}, '9'},
			{{-1,-1}, 'A'},
			{{0,-1}, 'B'},
			{{1,-1}, 'C'},
			{{0,-2}, 'D'}
		};

		auto code = std::string{};
		auto position = Position{};

		std::string line;
		while(file >> line) {

			for(auto step : line) { cmd_map[step](position, boundaries); }

			code.push_back(key_map[position]);
		}

		std::cout << code << std::endl;

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
