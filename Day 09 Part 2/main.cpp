#include <fstream>
#include <iostream>
#include <string>

template<typename S, typename N = std::uintmax_t>
auto decompress(const S& compressed, N repeat = 1) -> decltype(auto) {

	static constexpr auto marker_on = '(';
	static constexpr auto marker_off = ')';
	static constexpr auto multiplier = 'x';

	static auto get_next = [] (auto number) { return ++number; };

	static auto get_value = [] (const auto& seq, auto start, auto end) {
		return std::stoul(seq.substr(start, (end - start)));
	};

	static auto end_of_string = [&] (auto pos) { return (pos < compressed.size()); };

	auto marker_begin = compressed.find(marker_on);

	auto has_marker = end_of_string(marker_begin);

	if(!has_marker) { return (repeat * compressed.size()); }

	// if there are any letters before the first marker, they are counted here
	auto accumulator = marker_begin;

	while(has_marker) {

		auto sublength_begin = (marker_begin + 1);
		auto sublength_end = compressed.find(multiplier, sublength_begin);

		auto sublength = get_value(compressed, sublength_begin, sublength_end);

		auto subrepeat_begin = get_next(sublength_end);
		auto subrepeat_end = compressed.find(marker_off, subrepeat_begin);

		auto subrepeat = (repeat * get_value(compressed, subrepeat_begin, subrepeat_end));

		auto substr_begin = get_next(subrepeat_end);
		auto substr_end = (substr_begin + sublength);

		auto substring = compressed.substr(substr_begin, sublength);

		accumulator += decompress(substring, subrepeat);

		marker_begin = compressed.find(marker_on, substr_end);

		has_marker = end_of_string(marker_begin);

		// count unmarked letters if any
		if(has_marker) {

			accumulator += (marker_begin - substr_end);

		} else if(end_of_string(substr_end)) {

			accumulator += (compressed.size() - substr_end);
		}
	}

	return accumulator;
}

int main() {

	auto filename = std::string{"compressed.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto compressed = std::string{};

		file >> compressed;

		std::cout << decompress(compressed) << std::endl;
	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
