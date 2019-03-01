#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>

struct Triangle {
	std::uintmax_t a;
	std::uintmax_t b;
	std::uintmax_t c;
};

struct TripleTriangle {
	Triangle A, B, C;
};

auto& operator>>(std::istream& in, TripleTriangle& tt) {
	return in >> tt.A.a >> tt.B.a >> tt.C.a
			  >> tt.A.b >> tt.B.b >> tt.C.b
			  >> tt.A.c >> tt.B.c >> tt.C.c;
}

template<typename T>
auto is_triangle_valid(const T& triangle) {
	return ((triangle.a + triangle.b) > triangle.c)
		   && ((triangle.a + triangle.c) > triangle.b)
		   && ((triangle.b + triangle.c) > triangle.a);
}

int main() {

	auto filename = std::string{"triangles.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto count = std::accumulate(std::istream_iterator<TripleTriangle>{file}, {}, 0UL, [] (auto acc, const auto& tt) {
			return acc + is_triangle_valid(tt.A)
					   + is_triangle_valid(tt.B)
					   + is_triangle_valid(tt.C);
		});

		std::cout << count << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
