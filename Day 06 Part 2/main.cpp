#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/range/irange.hpp>

using uint64 = std::uint64_t;

// a convenient way to track the frequency of a letter
struct CharFrequencyTracker {
	char letter;
	uint64 count;
	CharFrequencyTracker() : letter{}, count{std::numeric_limits<uint64>::max()} {}
	CharFrequencyTracker(std::pair<char, uint64> p) : letter{p.first}, count{p.second} {}
};

auto operator<(const CharFrequencyTracker& lhs, const CharFrequencyTracker& rhs) {
	return (lhs.count < rhs.count);
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

		const auto binop = [] (auto& msg, const auto& column) {

			auto letter_counts = std::unordered_map<char, uint64>{};

			for(const auto letter : column) {
				++letter_counts[letter];
			}

			auto least_frequent_letter = CharFrequencyTracker{};

			for(const auto& letter : letter_counts) {
				least_frequent_letter = std::min(least_frequent_letter, {letter});
			}

			return (msg += least_frequent_letter.letter);
		};

		const auto message = std::accumulate(begin, end, init, binop);

		std::cout << message << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
