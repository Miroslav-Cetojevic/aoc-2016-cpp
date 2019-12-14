#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using uint64 = std::uint64_t;

template<typename S>
auto has_norm_tls_support(const S& seq) {

	auto result = false;

	for(uint64 i = 0, j = (i + 3); !result && (j < seq.size()); ++i, ++j) {

		result = ((seq[i] != seq[i+1]) && (seq[i] == seq[j]) && (seq[i+1] == seq[j-1]));
	}

	return result;
}

template<typename S>
auto has_hyper_tls_support(const S& seq) {

	auto result = true;

	for(uint64 i = 0, j = (i + 3); result && (j < seq.size()); ++i, ++j) {

		result = ((seq[i] != seq[j]) || (seq[i+1] != seq[j-1]));
	}

	return result;
}

int main() {

	const auto filename = std::string{"ip_list.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto num_ips = uint64{};

		auto norm_sequences = std::vector<std::string>{};
		auto hyper_sequences = std::vector<std::string>{};

		auto sstream = std::stringstream{};
		auto token = std::string{};

		std::string line;

		while(std::getline(file, line)) {

			norm_sequences.clear();
			hyper_sequences.clear();

			sstream.clear();
			sstream.str(line);

			while(sstream >> token) {
				norm_sequences.push_back(token);
				// we know every second sequence is a hypernet sequence
				if(sstream >> token) {
					hyper_sequences.push_back(token);
				}
			}

			// ====================
			// normal net sequences
			// ====================
			const auto norm_begin = norm_sequences.begin();
			const auto norm_end   = norm_sequences.end();

			const auto norm_support = [] (const auto& sequence) {
				return has_norm_tls_support(sequence);
			};

			const auto norm_supports_tls = std::any_of(norm_begin, norm_end, norm_support);

			// ====================
			// hyper net sequences
			// ====================
			const auto hyper_begin = hyper_sequences.begin();
			const auto hyper_end = hyper_sequences.end();

			const auto hyper_support = [] (const auto& sequence) {
				return has_hyper_tls_support(sequence);
			};

			const auto hyper_supports_tls = std::all_of(hyper_begin, hyper_end, hyper_support);

			// number of ip addresses that support TLS
			num_ips += (norm_supports_tls && hyper_supports_tls);
		}

		std::cout << num_ips << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
