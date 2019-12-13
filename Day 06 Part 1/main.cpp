#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/range/irange.hpp>

using uint64 = std::uint64_t;

// a convenient way to track which letter is most frequently used
struct CharFrequencyTracker {
	char letter;
	uint64 count;
};

auto operator<(const CharFrequencyTracker& lhs, const CharFrequencyTracker& rhs) {
	return (lhs.count < rhs.count);
}

template<typename M>
auto map_reset_values(M&& map) {
	for(auto& pair : map) {
	    pair.second = {};
	}
}

int main() {

	const auto filename = std::string{"message.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		// in order to transpose the input line by line, we will need a string for each column,
		// so that we can add the letter from the same column to the same string on each line
		auto transposed_msg = std::vector<std::string>{};

		auto line = std::string{};

		// initialize the transposed message
		file >> line;
		for(const auto letter : line) {
			transposed_msg.push_back({letter});
		}

		while(file >> line) {
			for(const auto i : boost::irange(line.size())) {
				transposed_msg[i] += line[i];
			}
		}

		const auto begin = transposed_msg.begin();
		const auto end   = transposed_msg.end();
		const auto init  = std::string{};

		using LetterCounts = std::unordered_map<char, uint64>;

		// a mutable lambda will let us keep the state of letter_counts,
		// even if it's called again with new arguments
		const auto count = [letter_counts = LetterCounts{}] (auto& msg, const auto& column) mutable {

			// we can still use the keys collected so far,
			// only the values need to be reset to default
			map_reset_values(letter_counts);

			for(const auto letter : column) {
				++letter_counts[letter];
			}

			auto most_frequent_letter = CharFrequencyTracker{};

			for(const auto letter : letter_counts) {
				most_frequent_letter = std::max(most_frequent_letter, {letter.first, letter.second});
			}

			return (msg += most_frequent_letter.letter);
		};

		const auto message = std::accumulate(begin, end, init, count);

		std::cout << message << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
