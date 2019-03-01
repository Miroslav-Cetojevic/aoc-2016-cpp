#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

int main() {

	auto filename = std::string{"rooms.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto sum_id = 0UL;

		using Count = std::uintmax_t;
		using Alphabet = std::unordered_map<char, Count>;
		auto letter_map = Alphabet{};

		auto tokens = std::vector<std::string>{};

		auto sstream = std::stringstream{};

		auto line = std::string{};
		while(std::getline(file, line)) {

			sstream.clear();
			sstream.str(line);

			tokens.clear();

			auto token = std::string{};
			while(sstream >> token) {
				tokens.push_back(token);
			}

			// count letters from first (size()-2) tokens
			letter_map.clear();
			std::for_each(tokens.begin(), (tokens.end() - 2), [&letter_map] (const auto& token) {
				for(const auto letter : token) {
					++letter_map[letter];
				}
			});

			// check if checksum has the correct order
			const auto& checksum = tokens.back();

			auto checksum_ok = true;
			for(auto it = std::next(checksum.begin()); (it != checksum.end()) && checksum_ok; ++it) {

				auto curr_letter = *it;
				auto prev_letter = *std::prev(it);

				auto curr_letter_count = letter_map[curr_letter];
				auto prev_letter_count = letter_map[prev_letter];

				if(curr_letter_count == 0 || prev_letter_count == 0) {
					checksum_ok = false;
					continue;
				}

				if(curr_letter_count < prev_letter_count) {
					checksum_ok = true;
				} else if(curr_letter_count == prev_letter_count) {

					if(curr_letter > prev_letter) {
						checksum_ok = true;
					} else {
						checksum_ok = false;
					}
				} else {
					checksum_ok = false;
				}
			}

			std::cout << checksum << " -> ";

			for(const auto letter : checksum) {
				std::cout << letter_map[letter] << ",";
			}

			std::cout << std::boolalpha << checksum_ok << std::endl;

			if(checksum_ok) { sum_id += std::stoul(tokens[tokens.size()-2]); }

		}

		std::cout << sum_id << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
