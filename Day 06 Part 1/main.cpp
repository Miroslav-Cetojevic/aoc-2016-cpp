#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/iterator/counting_iterator.hpp>

#include "flat_hash_map.hpp"

template<typename T>
struct Range {
	boost::counting_iterator<T, boost::use_default, T> begin, end;
	Range(T b, T e): begin(b), end(e) {}
};

// a convenient way to track which letter is most frequently used
struct MaxFrequencyLetter {
	std::uintmax_t count;
	char letter;

	auto clear() {
		count = 0;
		letter = char{};
	}
};

auto operator<(const MaxFrequencyLetter& lhs, const MaxFrequencyLetter& rhs) {
	return (lhs.count < rhs.count);
}

int main() {

	auto filename = std::string{"message.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto message = std::string{};

		auto transposed_msg = std::vector<std::string>{};

		auto line = std::string{};

		// initialize the transposed message
		file >> line;
		for(const auto letter : line) {

			transposed_msg.push_back({});

			transposed_msg.back() += letter;
		}

		auto range = Range{0UL, line.size()};

		while(file >> line) {

			std::for_each(range.begin, range.end, [&] (auto i) {
				transposed_msg[i] += line[i];
			});

		}

		auto letter_count = ska::flat_hash_map<char, std::uintmax_t>{};
		auto max_count = MaxFrequencyLetter{};

		for(const auto& column : transposed_msg) {

			letter_count.clear();

			for(const auto letter : column) {
				++letter_count[letter];
			}

			max_count.clear();

			for(const auto letter : letter_count) {
				max_count = std::max(max_count, MaxFrequencyLetter{letter.second, letter.first});
			}

			message += max_count.letter;
		}

		std::cout << message << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
