#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>

#include <boost/range/irange.hpp>

using uint64 = std::uint64_t;

struct Arguments {
	std::string strcmd;
	uint64 A, B;
};

auto& operator>>(std::istream& in, Arguments& args) {
	return in >> args.strcmd >> args.A >> args.B;
}

// tmp_screen is unnecessary as we are not doing any
// intermediate steps, thus we don't need to save any state
template<typename S, typename N>
auto set_rectangle(S& screen, S&, N width, N height) {

	for(const auto row : boost::irange(height)) {

		for(const auto col : boost::irange(width)) {

			screen[row][col] = true;
		}
	}
}

template<typename S, typename N>
auto shift_row(S& screen, S& tmp_screen, N row, N shift) {

	tmp_screen = screen;

	// on the temporary screen, the row is set to all-false
	// so that we can lay the original screen over it, and
	// based on the state of the original screen we can set
	// the columns on the temporary screen to true as if we
	// had shifted the row of the original screen
	for(auto& col : tmp_screen[row]) {
		col = false;
	}

	const auto width = tmp_screen[row].size();

	for(const auto col : boost::irange(width)) {

		// if the current column on the original screen is lit,
		// we can start doing the shifting and apply the result
		// to the corresponding column on the temporary screen
		if(screen[row][col]) {

			const auto new_pos = ((col + shift) % width);

			tmp_screen[row][new_pos] = true;
		}
	}

	screen = tmp_screen;
}

// works the same way as shift_row(), except the shift applies to columns instead of rows
template<typename S, typename N>
auto shift_col(S& screen, S& tmp_screen, N col, N shift) {

	tmp_screen = screen;

	for(auto& row : tmp_screen) {
		row[col] = false;
	};

	const auto height = tmp_screen.size();

	for(const auto row : boost::irange(height)) {

		if(screen[row][col]) {

			const auto new_row = ((row + shift) % height);

			tmp_screen[new_row][col] = true;
		}
	}

	screen = tmp_screen;
}

template<typename S>
auto print(const S& screen) {

	for(const auto& row : screen) {

		for(const auto col : row) {
			std::cout << (col ? '#' : ' ');
		}

		std::cout << "\n";
	}

	std::cout.flush();
}

int main() {

	const auto filename = std::string{"operations.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		constexpr auto width = uint64{50};
		constexpr auto height = uint64{6};

		using Array2D = std::array<std::array<bool, width>, height>;

		auto screen = Array2D{};
		auto tmp_screen = screen;

		using Command = void (*) (decltype(screen)&, decltype(tmp_screen)&, decltype(width), decltype(height));

		auto commands = std::unordered_map<std::string, Command>{
			{"rect", set_rectangle},
			{"row", shift_row},
			{"col", shift_col}
		};

		auto args = Arguments{};

		while(file >> args) {
			commands[args.strcmd](screen, tmp_screen, args.A, args.B);
		}

		print(screen);

	} else {
		std::cerr << "Error! Could not open \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
