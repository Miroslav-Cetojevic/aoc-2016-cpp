#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

struct Triangle {
	std::uintmax_t a;
	std::uintmax_t b;
	std::uintmax_t c;
};

auto& operator>>(std::istream& in, Triangle& triangle) {
	return in >> triangle.a >> triangle.b >> triangle.c;
}

int main() {

	auto filename = std::string{"triangles.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto count = std::count_if(std::istream_iterator<Triangle>{file}, {}, [] (const auto& triangle) {
			return ((triangle.a + triangle.b) > triangle.c)
				   && ((triangle.a + triangle.c) > triangle.b)
				   && ((triangle.b + triangle.c) > triangle.a);
		});

		std::cout << count << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
