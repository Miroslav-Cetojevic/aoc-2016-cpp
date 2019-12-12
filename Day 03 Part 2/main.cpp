#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>

struct Triangle {
	std::uint64_t a, b, c;
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
	const auto a = triangle.a;
	const auto b = triangle.b;
	const auto c = triangle.c;

	return ((a + b) > c) && ((a + c) > b) && ((b + c) > a);
}

int main() {

	const auto filename = std::string{"triangles.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto count = std::accumulate(std::istream_iterator<TripleTriangle>{file}, {}, 0, [] (auto acc, const auto& tt) {
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
