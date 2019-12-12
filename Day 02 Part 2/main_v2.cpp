#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

struct Position {
	// position of key no. 5
	// where we always start at
	std::int64_t x = 3;
	std::int64_t y = 1;
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

template<typename T, typename U>
constexpr auto index(T row, T col, U gridlen) {
	return (row * gridlen) + col;
}

template<typename T>
constexpr auto get_restricted_grid(const T& accessible_grid) {

	constexpr auto gridlen = 7;
	constexpr auto gridsize = (gridlen * gridlen);

	auto base_grid = std::array<bool, gridsize>{};

	for(const auto position : accessible_grid) {

		const auto row = position.x;
		const auto col = position.y;

		base_grid[index(row, col, gridlen)] = true;
	}

	return base_grid;
}

int main() {

	const auto filename = std::string{"instructions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		constexpr auto N_KEYS = 13;
		const auto keys = std::array<Position, N_KEYS>{{
			{1,3}, {2,2}, {2,3}, {2,4}, {3,1},
		    {3,2}, {3,3}, {3,4}, {3,5}, {4,2},
			{4,3}, {4,4}, {5,3}
		}};

		const auto key_numbers = std::unordered_map<Position, char> {
			{keys[0],  '1'}, {keys[1], '2'}, {keys[2],  '3'}, {keys[3],  '4'},
			{keys[4],  '5'}, {keys[5], '6'}, {keys[6],  '7'}, {keys[7],  '8'},
			{keys[8],  '9'}, {keys[9], 'A'}, {keys[10], 'B'}, {keys[11], 'C'},
			{keys[12], 'D'}
		};

		// this will give me
		constexpr auto restricted_grid = get_restricted_grid(keys);
		constexpr auto gridlen = static_cast<int>(std::sqrt(restricted_grid.size()));

		auto pos = Position{};
		auto& x = pos.x;
		auto& y = pos.y;

		const auto move_to_key = [gridlen, &restricted_grid, &pos] (Position&& tmp_pos) {

			const auto is_accessible = restricted_grid[index(tmp_pos.x, tmp_pos.y, gridlen)];

			if(is_accessible) {
				pos = tmp_pos;
			}
		};

		auto code = std::string{};

		std::string line;

		while(file >> line) {

			for(const auto move : line) {

				switch(move) {
					case 'U': move_to_key({(x - 1), y}); break;
					case 'D': move_to_key({(x + 1), y}); break;
					case 'L': move_to_key({x, (y - 1)}); break;
					case 'R': move_to_key({x, (y + 1)}); break;
					default: std::cout << "WTF?!?" << std::endl;
				}
			}

			code.push_back(key_numbers.find(pos)->second);
		}

		std::cout << code << std::endl;

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
