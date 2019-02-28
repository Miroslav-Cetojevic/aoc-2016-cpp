#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/iterator/counting_iterator.hpp>

#include "unordered_map.hpp"

template<typename T>
struct Range {
	boost::counting_iterator<T, boost::use_default, T> begin, end;

	Range(T b, T e): begin(b), end(e) {}
};

struct Point {
	std::intmax_t x = 0;
	std::intmax_t y = 0;
};

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
	std::string turn;
};

auto operator==(const Situation& lhs, const Situation& rhs) {
	return ((lhs.direction == rhs.direction) && (lhs.turn == rhs.turn));
}

template<typename T>
auto get_hash(T t) { return std::hash<T>{}(t); }

namespace std {
	template<>
	struct hash<Point> {
		auto operator()(const Point& point) const {
			return std::bit_xor{}(get_hash(point.x), get_hash(point.y));
		}
	};

	template<>
	struct hash<Situation> {
		auto operator()(const Situation& situation) const {
			return std::bit_xor{}(get_hash(situation.direction), get_hash(situation.turn));
		}
	};
}

int main() {
	auto filename = std::string{"directions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		const auto left = std::string("L");
		const auto right = std::string("R");

		auto dir_switch = ska::unordered_map<Situation, Direction>{
			{{Direction::NORTH, left}, Direction::WEST},
			{{Direction::NORTH, right}, Direction::EAST},
			{{Direction::EAST, left}, Direction::NORTH},
			{{Direction::EAST, right}, Direction::SOUTH},
			{{Direction::SOUTH, left}, Direction::EAST},
			{{Direction::SOUTH, right}, Direction::WEST},
			{{Direction::WEST, left}, Direction::SOUTH},
			{{Direction::WEST, right}, Direction::NORTH}
		};

		auto steps = ska::unordered_map<Direction, intmax_t>{
			{Direction::NORTH, 1},
			{Direction::EAST, 1},
			{Direction::SOUTH, -1},
			{Direction::WEST, -1}
		};

		auto pos = Position{};

		auto locations = ska::unordered_set<Point>{};
		locations.emplace(pos.point);

		auto revisited = false;
		std::string input;
		while(!revisited && (file >> input)) {

			const auto turn = input.substr(0, 1);
			pos.direction = dir_switch[{pos.direction, turn}];

			auto& point = pos.point;
			const auto dir = pos.direction;
			auto& coordinate = ((dir == Direction::EAST || dir == Direction::WEST) ? point.x : point.y);

			auto blocks = std::stoul(input.substr(1, (input.size() - 1)));
			auto range = Range<decltype(blocks)>{0, blocks};

			revisited = std::any_of(range.begin, range.end, [&steps, &locations, &point, dir, &coordinate] (auto) {
				coordinate += steps[dir];
				return !(locations.emplace(point).second);
			});
		}

		auto distance = (std::abs(pos.point.x) + std::abs(pos.point.y));

		std::cout << distance << std::endl;

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
