#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

void my_split(std::vector<std::string>& tokens, const std::string& str, const std::string& delims = " ") {

	tokens.clear();

    decltype(tokens.size()) current, previous = 0;

    current = str.find_first_of(delims);

    while(current != std::string::npos) {

        tokens.push_back(str.substr(previous, current - previous));

        previous = (current + 1);

        current = str.find_first_of(delims, previous);
    }

    // there's still some string left
    if(previous < str.size()) {
    	tokens.push_back(str.substr(previous));
    }
}

int main() {

	const auto filename = std::string{"rooms.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto sum = std::uint64_t{};

		using Count = std::uint64_t;

		auto letter_counts = std::unordered_map<char, Count>{};

		auto tokens = std::vector<std::string>{};

		std::string line;

		while(std::getline(file, line)) {

			my_split(tokens, line, "-[]");

			letter_counts.clear();

			std::for_each(tokens.begin(), (tokens.end() - 2), [&letter_counts] (const auto& token) {

				for(const auto letter : token) {
					++letter_counts[letter];
				}
			});

			// last token is always the checksum
			const auto& checksum = tokens.back();

			const auto checksum_begin = checksum.begin();
			const auto checksum_end   = checksum.end();

			const auto comparator = [&letter_counts] (const auto first_letter, const auto second_letter) {

				const auto first_letter_count = letter_counts[first_letter];
				const auto second_letter_count = letter_counts[second_letter];

				const auto letters_exist = (first_letter_count > 0 && second_letter_count > 0);
				const auto ordered_by_frequency = (first_letter_count > second_letter_count);
				const auto tied_by_frequency = (first_letter_count == second_letter_count);
				const auto ordered_by_alphabet = (first_letter < second_letter);

				return !(letters_exist && (ordered_by_frequency || (tied_by_frequency && ordered_by_alphabet)));
			};

			const auto validator = std::adjacent_find(checksum_begin, checksum_end, comparator);

			// we need to go through the whole checksum
			// before we can conclude that it's valid
			const auto is_checksum_valid = (validator == checksum_end);

			if(is_checksum_valid) {
				sum += std::stoul(tokens[tokens.size()-2]);
			}

		}

		std::cout << sum << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
