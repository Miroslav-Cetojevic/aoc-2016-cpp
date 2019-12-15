#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

enum class ItemType { BOT, OUTPUT };

using uint64 = std::uint64_t;

struct FactoryItem {
	ItemType type;
	uint64 id;
};

auto operator==(const FactoryItem& lhs, const FactoryItem& rhs) {
	return (lhs.type == rhs.type) && (lhs.id == rhs.id);
}

template<typename T>
auto get_hash(T t) { return std::hash<T>{}(t); }

namespace std {
	template<>
	struct hash<FactoryItem> {
		auto operator()(const FactoryItem& item) const {
			return (get_hash(item.type) ^ get_hash(item.id));
		}
	};
}

struct Instruction {
	FactoryItem distributor;
	FactoryItem low_recipient;
	FactoryItem high_recipient;
};

template<typename Sequence>
void my_split(Sequence&& tokens, const std::string& str, const std::string& delims = " ") {

	tokens.clear();

	decltype(tokens.size()) current = 0;

	while (current != str.npos) {
		const auto end = str.find_first_of(delims, current);
		tokens.push_back(str.substr(current, end - current));
		current = str.find_first_not_of(delims, end);
	}
}

int main() {

	const auto filename = std::string{"instructions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto item_types = std::unordered_map<std::string, ItemType>{
			{{"bot"}, ItemType::BOT},
			{{"output"}, ItemType::OUTPUT},
		};

		// we use std::list instead of std::vector, because because we will
		// be calling erase() later on, which can be expensive on a vector
		auto instructions = std::list<Instruction>{};

		using Chips = std::vector<uint64>;

		auto items = std::unordered_map<FactoryItem, Chips>{};

		auto tokens = std::vector<std::string>{};

		auto line = std::string{};

		while(std::getline(file, line)) {

			my_split(tokens, line);

			if(tokens.size() == 6) {
				instructions.push_back({
					{item_types[tokens[0]], std::stoul(tokens[1])},
					{item_types[tokens[2]], std::stoul(tokens[3])},
					{item_types[tokens[4]], std::stoul(tokens[5])}
				});
			} else {
				// 4 tokens, two of which are ignored, as their values
				// are known and not needed ("value" and "bot")
				items[{ItemType::BOT, std::stoul(tokens[3])}].push_back(std::stoul(tokens[1]));
			}
		}

		const auto has_two_chips = [] (auto& chips) { return (chips.size() == 2); };

		const auto has_instructions = [&instructions] { return !instructions.empty(); };

		const auto sort = [] (auto&& chips) {
			auto& chip1 = chips.front();
			auto& chip2 = chips.back();
			if(chip1 > chip2) { std::swap(chip1, chip2); }
		};

		for(auto instruction = instructions.begin(); has_instructions();) {

			const auto distributor = items.find(instruction->distributor);

			const auto is_distributor_mapped = (distributor != items.end());

			if(is_distributor_mapped && has_two_chips(distributor->second)) {

				auto& chips = distributor->second;

				// sort in ascending order, so the low and high
				// recipients always get the appropriate chip
				sort(chips);

				items[instruction->low_recipient].push_back(chips.front());
				items[instruction->high_recipient].push_back(chips.back());

				instruction = instructions.erase(instruction);

			} else {
				// instruction is not actionable yet, we will return later
				++instruction;
			}

			const auto end_of_instructions = (instruction == instructions.end());

			if(end_of_instructions && has_instructions()) {
				instruction = instructions.begin();
			}
		}

		const auto begin = items.begin();
		const auto end   = items.end();
		const auto init  = uint64{1};

		const auto accumulator = [] (auto acc, const auto& item) {
			if((item.first.type == ItemType::OUTPUT) && (item.first.id <= 2)) {
				acc *= item.second.front();
			}

			return acc;
		};

		const auto result = std::accumulate(begin, end, init, accumulator);

		std::cout << result << std::endl;


	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
