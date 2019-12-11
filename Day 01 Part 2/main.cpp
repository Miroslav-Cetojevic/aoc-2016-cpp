#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <boost/range/irange.hpp>

struct Point {
	std::int64_t x = 0;
	std::int64_t y = 0;
};

auto& operator+=(Point& lhs, const Point& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

// this is not actually needed, but I'll keep it as a reference for
// how to implement operator+ for a struct in terms of operator+=
auto operator+(Point lhs, const Point& rhs) {
	lhs += rhs;
	return lhs;
}

auto operator==(const Point& lhs, const Point& rhs) {
	return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}

enum class Direction { NORTH, EAST, SOUTH, WEST };

struct Position {
	Point point;
	Direction direction = Direction::NORTH;
};

struct Situation {
	Direction direction;
	char turn;
};

auto operator==(const Situation& lhs, const Situation& rhs) {
	return ((lhs.direction == rhs.direction) && (lhs.turn == rhs.turn));
}

template<typename T>
auto get_hash(T value) {
	return std::hash<T>{}(value);
}

namespace std {
	template<>
	struct hash<Point> {
		auto operator()(const Point& point) const {
			return (get_hash(point.x) ^ get_hash(point.y));
		}
	};

	template<>
	struct hash<Situation> {
		auto operator()(const Situation& situation) const {
			return (get_hash(situation.direction) ^ get_hash(situation.turn));
		}
	};
}

int main() {

	const auto filename = std::string{"directions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		const auto left = 'L';
		const auto right = 'R';

		// get the new direction you are facing after the turn
		const auto guide = std::unordered_map<Situation, Direction>{
			{{Direction::NORTH, left}, Direction::WEST},
			{{Direction::NORTH, right}, Direction::EAST},
			{{Direction::EAST, left}, Direction::NORTH},
			{{Direction::EAST, right}, Direction::SOUTH},
			{{Direction::SOUTH, left}, Direction::EAST},
			{{Direction::SOUTH, right}, Direction::WEST},
			{{Direction::WEST, left}, Direction::SOUTH},
			{{Direction::WEST, right}, Direction::NORTH}
		};

		// converts a move in any direction to the corresponding
		// unit of distance, in this case, Point
		const auto point_units = std::unordered_map<Direction, Point>{
			{Direction::NORTH, {0, 1}},
			{Direction::EAST, {1, 0}},
			{Direction::SOUTH, {0, -1}},
			{Direction::WEST, {-1, 0}}
		};

		auto pos = Position{};
		auto& point = pos.point;
		auto& direction = pos.direction;

		auto locations = std::unordered_set<Point>{point};

		auto revisited = false;

		std::string instruction;

		while(!revisited && (file >> instruction)) {

			const auto turn = instruction.front();

			direction = guide.find({direction, turn})->second;

			auto blocks = std::stoul(instruction.substr(1, (instruction.size() - 1)));

			for(const auto i : boost::irange(blocks)) {

				point += point_units.find(direction)->second;

				// we make use of the fact that a set will fail to insert
				// an element if it's already present, so the first failed
				// insertion is our cue to leave the loop
				revisited = !(locations.emplace(point).second);

				if(revisited) {
					break;
				}
			}
		}

		const auto distance = (std::abs(point.x) + std::abs(point.y));

		std::cout << distance << std::endl;

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
