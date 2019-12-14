#include <fstream>
#include <iostream>
#include <string>

int main() {

	const auto filename = std::string{"compressed.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		constexpr auto mark_begin = '(';
		constexpr auto mark_end = ')';
		constexpr auto delimiter = 'x';

		const auto get_next = [] (auto number) { return ++number; };

		const auto get_value = [] (auto& compressed, auto start, auto pos) {
			return std::stoul(compressed.substr(start, (pos - start)));
		};

		using uint64 = std::uint64_t;

		auto decompressed_length = uint64{};

		auto compressed = std::string{};

		file >> compressed;

		// we simply pass through the compressed string, find the markers
		// get the length and repeat numbers and apply some calculation
		for(auto i = uint64{}; i < compressed.size();) {

			if(compressed[i] == mark_begin) {

				const auto length_begin = get_next(i);
				const auto length_end   = compressed.find(delimiter, length_begin);

				const auto length = get_value(compressed, length_begin, length_end);

				const auto repeat_begin = get_next(length_end);
				const auto repeat_end   = compressed.find(mark_end, repeat_begin);

				const auto repeat = get_value(compressed, repeat_begin, repeat_end);

				decompressed_length += (length * repeat);

				i = (get_next(repeat_end) + length);

			} else {
				// no marker means this character will appear
				// in the decompressed string "as is"
				++decompressed_length;
				++i;
			}
		}

		std::cout << decompressed_length << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
