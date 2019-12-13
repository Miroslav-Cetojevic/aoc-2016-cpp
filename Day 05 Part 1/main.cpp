#include <algorithm>
#include <fstream>
#include <iostream>
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

		using Count = std::uint64_t;

		auto letter_counts = std::unordered_map<char, Count>{};

		auto tokens = std::vector<std::string>{};

		std::string line;

		auto found_room = false;

		while(!found_room && std::getline(file, line)) {

			my_split(tokens, line, "-[]");

			const auto new_end = tokens.end() - 2;

			// sector id is always the second to last token
			const auto sector_id = std::stoul(*new_end);

			// checksum is always the last token
			const auto checksum = tokens.back();

			tokens.erase(new_end);

			letter_counts.clear();
			for(const auto& token : tokens) {

				for(const auto letter : token) {
					++letter_counts[letter];
				}
			}

			const auto checksum_begin = checksum.begin();
			const auto checksum_end   = checksum.end();

			const auto comparator = [&letter_counts] (const auto first_letter, const auto second_letter) {

				const auto first_letter_count = letter_counts[first_letter];
				const auto second_letter_count = letter_counts[second_letter];

				const auto letters_exist = ((first_letter_count > 0) && (second_letter_count > 0));
				const auto ordered_by_frequency = (first_letter_count > second_letter_count);
				const auto tied_by_frequency = (first_letter_count == second_letter_count);
				const auto ordered_by_alphabet = (first_letter < second_letter);

				return !(letters_exist && (ordered_by_frequency || (tied_by_frequency && ordered_by_alphabet)));
			};

			const auto validator = std::adjacent_find(checksum_begin, checksum_end, comparator);

			// we need to go through the whole checksum before we can conclude that it's valid,
			// so the only way it COULD be valid is if the validator is at the end of it
			const auto is_checksum_valid = (validator == checksum_end);

			if(is_checksum_valid) {

				constexpr auto a = 'a';
				constexpr auto mod = 26;

				for(auto& token : tokens) {

					for(auto& letter : token) {
						// behold, the state-of-the-art shift cipher
						letter = (((letter - a + sector_id) % mod) + a);
					}

					if(token.find("north") != token.npos) {
						std::cout << sector_id << std::endl;
						found_room = true;
						break;
					}
				}
			}
		}

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
