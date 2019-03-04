#include <fstream>
#include <iostream>
#include <string>

int main() {

	auto filename = std::string{"compressed.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		const auto mark_begin = '(';
		const auto mark_end = ')';
		const auto delimiter = 'x';

		auto get_next = [] (auto number) { return ++number; };

		auto get_value = [&] (auto& compressed, auto start, auto pos) {
			return std::stoul(compressed.substr(start, (pos - start)));
		};

		auto decompressed_length = 0UL;

		auto compressed = std::string{};

		file >> compressed;

		for(auto i = 0UL; i < compressed.size();) {

			if(compressed[i] == mark_begin) {

				auto length_begin = get_next(i);
				auto length_end = compressed.find(delimiter, length_begin);

				auto length = get_value(compressed, length_begin, length_end);

				auto repeat_begin = get_next(length_end);
				auto repeat_end = compressed.find(mark_end, repeat_begin);

				auto repeat = get_value(compressed, repeat_begin, repeat_end);

				for(auto j = 0UL; j < repeat; ++j) {
					decompressed_length += length;
				}

				i = (get_next(repeat_end) + length);

			} else {
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
