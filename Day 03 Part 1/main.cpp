#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

struct Triangle {
	std::uint64_t a, b, c;
};

auto& operator>>(std::istream& in, Triangle& triangle) {
	return in >> triangle.a >> triangle.b >> triangle.c;
}

int main() {

	const auto filename = std::string{"triangles.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		const auto count = std::count_if(std::istream_iterator<Triangle>{file}, {}, [] (const auto& triangle) {
			const auto a = triangle.a;
			const auto b = triangle.b;
			const auto c = triangle.c;

			return ((a + b) > c) && ((a + c) > b) && ((b + c) > a);
		});

		std::cout << count << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
