#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

template<typename S, typename N = std::uint64_t>
auto decompress(const S& compressed, N repeat = 1) -> decltype(auto) {

	const auto compressed_size = compressed.size();

	static constexpr auto marker_on  = '(';
	static constexpr auto marker_off = ')';
	static constexpr auto multiplier = 'x';

	static constexpr auto get_next = [] (auto number) {
		return ++number;
	};

	static constexpr auto get_value = [] (const auto& seq, auto start, auto end) {
		return std::stoul(seq.substr(start, (end - start)));
	};

	const auto end_of_string = [compressed_size] (auto pos) {
		return (pos < compressed_size);
	};

	auto marker_begin = compressed.find(marker_on);

	auto has_marker = end_of_string(marker_begin);

	// ends the recursion for the current compressed substring
	if(!has_marker) {
		return (repeat * compressed_size);
	}

	// if there are any letters before the first marker, they are counted here
	auto accumulator = marker_begin;

	while(has_marker) {

		const auto sublength_begin = get_next(marker_begin);
		const auto sublength_end   = compressed.find(multiplier, sublength_begin);

		const auto sublength = get_value(compressed, sublength_begin, sublength_end);

		const auto subrepeat_begin = get_next(sublength_end);
		const auto subrepeat_end   = compressed.find(marker_off, subrepeat_begin);

		const auto subrepeat = (repeat * get_value(compressed, subrepeat_begin, subrepeat_end));

		const auto substr_begin = get_next(subrepeat_end);
		const auto substr_end   = (substr_begin + sublength);

		const auto substring = compressed.substr(substr_begin, sublength);

		accumulator += decompress(substring, subrepeat);

		marker_begin = compressed.find(marker_on, substr_end);

		has_marker = end_of_string(marker_begin);

		// count unmarked letters if any
		if(has_marker) {

			accumulator += (marker_begin - substr_end);

		} else if(end_of_string(substr_end)) {

			accumulator += (compressed_size - substr_end);
		}
	}

	return accumulator;
}

int main() {

	const auto filename = std::string{"compressed.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		const auto compressed = *(std::istream_iterator<std::string>{file});

		std::cout << decompress(compressed) << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
