#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// a proxy to enable the usage of istream_iterator that reads line by line
struct Reader {
	std::string line;
};

// allows the istream_iterator to read a file line by line
auto& operator>>(std::istream& in, Reader& reader) {
	return std::getline(in, reader.line);
}

int main() {

	const auto filename = std::string{"rooms.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		using Count = std::uintmax_t;
		using LetterMap = std::unordered_map<char, Count>;
		auto letter_map = LetterMap{};

		auto tokens = std::vector<std::string>{};

		auto sstream = std::stringstream{};

		std::string line, token;

		auto sum_id = std::accumulate(std::istream_iterator<Reader>{file}, {}, 0UL, [&] (auto acc, const auto& reader) {

			sstream.clear();
			sstream.str(reader.line);

			tokens.clear();
			while(sstream >> token) {
				tokens.push_back(token);
			}

			letter_map.clear();
			std::for_each(tokens.begin(), (tokens.end() - 2), [&letter_map] (const auto& token) {
				for(const auto letter : token) {
					++letter_map[letter];
				}
			});

			// last token is always the checksum
			const auto& checksum = tokens.back();

			// adjacent_find returns the iterator to the first pair of elements for which the predicate returns true.
			// In this particular case, the intent is to find out if the checksum has a valid ordering of its letters,
			// i.e letters are first sorted by frequency, then by alphabet. We want adjacent_find to return the end()
			// iterator to indicate a valid checksum, thus we negate the result of our validation.
			const auto validator = std::adjacent_find(checksum.begin(), checksum.end(),
													  [&] (const auto prev_letter, const auto curr_letter) {
				auto prev_letter_count = letter_map[prev_letter];
				auto curr_letter_count = letter_map[curr_letter];

				auto letters_exist = (prev_letter_count > 0 && curr_letter_count > 0);
				auto ordered_by_frequency = (prev_letter_count > curr_letter_count);
				auto tied_by_frequency = (prev_letter_count == curr_letter_count);
				auto ordered_by_alphabet = (prev_letter < curr_letter);

				return !(letters_exist && (ordered_by_frequency || (tied_by_frequency && ordered_by_alphabet)));
			});

			auto checksum_ok = (validator == checksum.end());

			return acc + (checksum_ok ? std::stoul(tokens[tokens.size()-2]) : 0);
		});

		std::cout << sum_id << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
