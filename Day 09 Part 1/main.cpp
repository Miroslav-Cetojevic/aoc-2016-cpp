#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

int main() {

	auto filename = std::string{"compressed.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto compressed = std::string{};

		file >> compressed;

		auto decompressed = std::string{};

		const auto mark_begin = '(';
		const auto mark_end = ')';
		const auto delimiter = 'x';

		auto get_next = [] (auto number) { return ++number; };

		auto get_value = [&] (auto start, auto pos) {
			return std::stoul(compressed.substr(start, (pos - start)));
		};

		auto decompress = [&] (auto start, auto length, auto repeat) {
			for(auto j = 0UL; j < repeat; ++j) {
				decompressed += compressed.substr(start, length);
			}
		};

		for(auto i = 0UL; i < compressed.size();) {

			if(compressed[i] == mark_begin) {

				auto start = get_next(i);

				auto pos = compressed.find(delimiter, start);

				auto length = get_value(start, pos);

				start = get_next(pos);

				pos = compressed.find(mark_end, start);

				auto repeat = get_value(start, pos);

				start = get_next(pos);

				decompress(start, length, repeat);

				i = (start + length);

			} else {
				decompressed += compressed[i];
				++i;
			}
		}

		std::cout << decompressed.size() << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
