#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Reader {
	std::string line;
};

auto& operator>>(std::istream& in, Reader& reader) {
	return std::getline(in, reader.line);
}

template<typename S>
auto has_norm_tls_support(const S& seq) {
	auto result = false;

	for(auto i = 0UL, j = (i + 3); !result && (j < seq.size()); ++i, ++j) {

		result = ((seq[i] != seq[i+1]) && (seq[i] == seq[j]) && (seq[i+1] == seq[j-1]));
	}

	return result;
}

template<typename S>
auto has_hyper_tls_support(const S& seq) {
	auto result = true;

	for(auto i = 0UL, j = (i + 3); result && (j < seq.size()); ++i, ++j) {

		result = ((seq[i] != seq[j]) || (seq[i+1] != seq[j-1]));
	}

	return result;
}

int main() {

	auto filename = std::string{"ip_list.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto norm_sequences = std::vector<std::string>{};
		auto hyper_sequences = std::vector<std::string>{};

		auto sstream = std::stringstream{};
		auto token = std::string{};

		auto tls_count = std::count_if(std::istream_iterator<Reader>{file}, {}, [&] (auto& reader) {

			norm_sequences.clear();
			hyper_sequences.clear();

			sstream.clear();
			sstream.str(reader.line);

			auto id = 0UL;
			while(sstream >> token) {

				// we know every second sequence is a hypernet sequence
				if(id++ % 2 == 0) { norm_sequences.push_back(token); }
				else { hyper_sequences.push_back(token); }
			}

			auto norm_supports_tls = std::any_of(norm_sequences.begin(), norm_sequences.end(), [] (const auto& seq) {
				return has_norm_tls_support(seq);
			});

			auto hyper_supports_tls = std::all_of(hyper_sequences.begin(), hyper_sequences.end(), [] (const auto& seq) {
				return has_hyper_tls_support(seq);
			});

			return (norm_supports_tls && hyper_supports_tls);

		});

		std::cout << tls_count << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
