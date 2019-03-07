#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "unordered_map.hpp"

enum class ItemType { BOT, OUTPUT };

struct FactoryItem {
	ItemType type;
	std::uintmax_t id;
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
			return std::bit_xor{}(get_hash(item.type), get_hash(item.id));
		}
	};
}

struct Instruction {
	FactoryItem distributor;
	FactoryItem low_recipient;
	FactoryItem high_recipient;
};

int main() {

	const auto filename = std::string{"instructions.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto type_map = ska::flat_hash_map<std::string, ItemType>{
			{{"bot"}, ItemType::BOT},
			{{"output"}, ItemType::OUTPUT},
		};

		auto instructions = std::vector<Instruction>{};

		using Chips = std::vector<std::uintmax_t>;

		auto items = ska::unordered_map<FactoryItem, Chips>{};

		auto tokens = std::vector<std::string>{};

		auto token = std::string{};

		auto sstream = std::stringstream{};

		auto line = std::string{};

		while(std::getline(file, line)) {

			sstream.clear();
			sstream.str(line);

			tokens.clear();
			while(sstream >> token) {
				tokens.emplace_back(token);
			}

			if(tokens.size() == 6) {
				instructions.emplace_back(Instruction{
					{type_map[tokens[0]], std::stoul(tokens[1])},
					{type_map[tokens[2]], std::stoul(tokens[3])},
					{type_map[tokens[4]], std::stoul(tokens[5])}
				});
			} else {
				// 4 tokens, two of which are ignored, as their values are the same in each case
				items[{ItemType::BOT, std::stoul(tokens[3])}].push_back(std::stoul(tokens[1]));
			}
		}

		auto sort_chips = [] (auto& chips) {
			if(chips.front() > chips.back()) {
				std::swap(chips.front(), chips.back());
			}
		};

		auto has_two_chips = [] (auto& chips) { return (chips.size() == 2); };

		auto has_instructions = [&instructions] { return !instructions.empty(); };

		for(auto instruction = instructions.begin(); has_instructions();) {

			const auto distributor = items.find(instruction->distributor);

			const auto is_distributor_mapped = (distributor != items.end());

			if(is_distributor_mapped && has_two_chips(distributor->second)) {

				auto& chips = distributor->second;

				sort_chips(chips);

				items[instruction->low_recipient].push_back(chips.front());
				items[instruction->high_recipient].push_back(chips.back());

				instruction = instructions.erase(instruction);

			} else {
				// instruction is not actionable yet, we will return later
				++instruction;
			}

			if(instruction == instructions.end() && has_instructions()) {
				instruction = instructions.begin();
			}
		}

		auto product = 1UL;

		for(const auto& item : items) {
			if((item.first.type == ItemType::OUTPUT) && (item.first.id <= 2)) {
				product *= item.second.front();
			}
		}

		std::cout << product << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
