#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>

template<typename S, typename N>
auto rect(S& screen, S&, const N width, const N height) {

	for(auto row = 0UL; row < height; ++row) {

		for(auto col = 0UL; col < width; ++col) {

			screen[row][col] = true;
		}
	}
}

template<typename S, typename N>
auto rotrow(S& screen, S& tmp_screen, const N row, const N shift) {

	tmp_screen = screen;

	for(auto& col : tmp_screen[row]) { col = false; }

	auto width = tmp_screen[row].size();

	for(auto col = 0UL; col < width; ++col) {
		if(screen[row][col]) {
			auto new_pos = ((col + shift) % width);
			tmp_screen[row][new_pos] = true;
		}
	}

	screen = tmp_screen;
}

template<typename S, typename N>
auto rotcol(S& screen, S& tmp_screen, const N col, const N shift) {

	tmp_screen = screen;

	for(auto& row : tmp_screen) { row[col] = false; };

	auto height = screen.size();

	for(auto row = 0UL; row < height; ++row) {
		if(screen[row][col]) {
			auto new_row = ((row + shift) % height);
			tmp_screen[new_row][col] = true;
		}
	}

	screen = tmp_screen;
}

struct Args {
	std::string cmd;
	std::uintmax_t A, B;
};

auto& operator>>(std::istream& in, Args& args) {
	return in >> args.cmd >> args.A >> args.B;
}

template<typename S>
auto print(const S& screen) {
	for(const auto& row : screen) {
		for(const auto col : row) {
			std::cout << (col ? '#' : ' ');
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

int main() {

	auto filename = std::string{"operations.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		constexpr auto width = 50UL;
		constexpr auto height = 6UL;

		using Bool = std::uint8_t;
		auto screen = std::array<std::array<Bool, width>, height>{};
		auto tmp_screen = decltype(screen){};

		using cmd = void (*) (decltype(screen)&, decltype(tmp_screen)&, decltype(width), decltype(height));

		auto cmd_map = std::unordered_map<std::string, cmd>{
			{"rect", rect},
			{"rotrow", rotrow},
			{"rotcol", rotcol}
		};

		auto args = Args{};

		while(file >> args) {
			cmd_map[args.cmd](screen, tmp_screen, args.A, args.B);
		}

		print(screen);

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
