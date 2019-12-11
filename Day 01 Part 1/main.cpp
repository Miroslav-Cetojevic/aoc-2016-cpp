#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

enum class Direction { NORTH, EAST, SOUTH, WEST };

struct Position {
	std::int64_t x = 0;
	std::int64_t y = 0;
	Direction facing = Direction::NORTH;
};

struct Situation {
	Direction facing;
	char turn;
};

auto operator==(const Situation& lhs, const Situation& rhs) {
	return ((lhs.facing == rhs.facing) && (lhs.turn == rhs.turn));
}

template<typename T>
auto get_hash(T value) {
	return std::hash<T>{}(value);
}

namespace std {
	template<>
	struct hash<Situation> {
		auto operator()(const Situation& situation) const {
			return (get_hash(situation.facing) ^ get_hash(situation.turn));
		}
	};
}

int main() {

	const auto filename = std::string{"directions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto pos = Position{};

		const auto relocate = [&pos] (auto facing, auto blocks) {

			switch(facing) {
				case Direction::NORTH: pos.y += blocks; break;
				case Direction::EAST : pos.x += blocks; break;
				case Direction::SOUTH: pos.y -= blocks; break;
				case Direction::WEST : pos.x -= blocks; break;
				default: std::cerr << "WTF?!?" << std::endl;
			}

			pos.facing = facing;
		};

		const auto L = 'L';
		const auto R = 'R';

		auto guide = std::unordered_map<Situation, Direction>{
			{{Direction::NORTH, L}, Direction::WEST},
			{{Direction::NORTH, R}, Direction::EAST},
			{{Direction::EAST, L}, Direction::NORTH},
			{{Direction::EAST, R}, Direction::SOUTH},
			{{Direction::SOUTH, L}, Direction::EAST},
			{{Direction::SOUTH, R}, Direction::WEST},
			{{Direction::WEST, L}, Direction::SOUTH},
			{{Direction::WEST, R}, Direction::NORTH}
		};

		std::string instruction;

		while(file >> instruction) {

			const auto blocks = std::stol(instruction.substr(1, (instruction.size() - 1)));

			const auto turn = instruction.front();

			const auto new_direction = guide[{pos.facing, turn}];

			relocate(new_direction, blocks);
		}

		const auto distance = (std::abs(pos.x) + std::abs(pos.y));

		std::cout << distance << std::endl;

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
