#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

struct Position {
	// position of key no. 5
	// where we always start at
	std::int64_t x = -2;
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

struct LegalMoves {
	bool can_go_up;
	bool can_go_down;
	bool can_go_left;
	bool can_go_right;
};

int main() {

	const auto filename = std::string{"instructions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		const auto key1 = Position{0,2};
		const auto key2 = Position{-1,1};
		const auto key3 = Position{0,1};
		const auto key4 = Position{1,1};
		const auto key5 = Position{-2,0};
		const auto key6 = Position{-1,0};
		const auto key7 = Position{0,0};
		const auto key8 = Position{1,0};
		const auto key9 = Position{2,0};
		const auto keyA = Position{-1,-1};
		const auto keyB = Position{0,-1};
		const auto keyC = Position{1,-1};
		const auto keyD = Position{0,-2};

		// given the unconventional keypad layout, each key
		// needs to be associated with a set of boolean values
		// denoting the legality of the possible moves
		using Boundaries = std::unordered_map<Position, LegalMoves>;

		const auto boundaries = Boundaries{
			{key1, {false, true,  false, false}},
			{key2, {false, true,  false, true}},
			{key3, {true,  true,  true,  true}},
			{key4, {false, true,  true,  false}},
			{key5, {false, false, false, true}},
			{key6, {true,  true,  true,  true}},
			{key7, {true,  true,  true,  true}},
			{key8, {true,  true,  true,  true}},
			{key9, {false, false, true,  false}},
			{keyA, {true,  false, false, true}},
			{keyB, {true,  true,  true,  true}},
			{keyC, {true,  false, true,  false}},
			{keyD, {true,  false, false, false}}
		};

		const auto keypad = std::unordered_map<Position, char> {
			{key1, '1'}, {key2, '2'}, {key3, '3'}, {key4, '4'},
			{key5, '5'}, {key6, '6'}, {key7, '7'}, {key8, '8'},
			{key9, '9'}, {keyA, 'A'}, {keyB, 'B'}, {keyC, 'C'},
			{keyD, 'D'}
		};

		const auto move_to_key = [] (auto condition, auto& dimension, auto modifier) {
			if(condition) {
				dimension += modifier;
			}
		};

		auto code = std::string{};

		auto pos = Position{};
		auto& x = pos.x;
		auto& y = pos.y;

		std::string line;

		while(file >> line) {

			for(const auto move : line) {

				const auto legal_moves = boundaries.find(pos)->second;

				switch(move) {
					case 'U': move_to_key(legal_moves.can_go_up,    y,  1); break;
					case 'D': move_to_key(legal_moves.can_go_down,  y, -1); break;
					case 'L': move_to_key(legal_moves.can_go_left,  x, -1); break;
					case 'R': move_to_key(legal_moves.can_go_right, x,  1); break;
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
