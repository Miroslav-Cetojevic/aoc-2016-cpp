#include <fstream>
#include <iostream>
#include <string>

enum class Direction { NORTH, EAST, SOUTH, WEST };

struct Position {
	std::intmax_t x = 0;
	std::intmax_t y = 0;
	Direction facing = Direction::NORTH;
};

int main() {
	auto filename = std::string{"directions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		const auto right = std::string("R");
		const auto left = std::string("L");

		auto pos = Position{};

		std::string instruction;
		while(file >> instruction) {

			auto confirmed = [&pos, &instruction] (const auto& turn, auto facing) {
				return (pos.facing == facing && instruction.substr(0, 1) == turn);
			};

			auto relocate = [&pos] (auto& coordinate, auto blocks, auto facing) {
				coordinate += blocks;
				pos.facing = facing;
			};

			auto blocks = std::stoul(instruction.substr(1, (instruction.size() - 1)));

			if(confirmed(right, Direction::NORTH) || confirmed(left, Direction::SOUTH)) {

				relocate(pos.x, blocks, Direction::EAST);

			} else if(confirmed(left, Direction::NORTH) || confirmed(right, Direction::SOUTH)) {

				relocate(pos.x, (-blocks), Direction::WEST);

			} else if(confirmed(left, Direction::EAST) || confirmed(right, Direction::WEST)) {

				relocate(pos.y, blocks, Direction::NORTH);

			} else {

				relocate(pos.y, (-blocks), Direction::SOUTH);

			}
		}

		auto distance = (std::abs(pos.x) + std::abs(pos.y));

		std::cout << distance << std::endl;

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
