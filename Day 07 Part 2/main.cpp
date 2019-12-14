#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using uint64 = std::uint64_t;

template<typename S, typename L>
auto collect_objects(const S& sequences, L&& list) {

	list.clear();

	for(const auto& seq : sequences) {

		for(uint64 i = 0, j = (i + 2); j < seq.size(); ++i, ++j) {

			auto has_object = ((seq[i] != seq[i+1]) && (seq[i] == seq[j]));

			if(has_object) {
				list.push_back(seq.substr(i, 3));
			}
		}
	}
}

int main() {

	const auto filename = std::string{"ip_list.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto ip_count = uint64{};

		using vector_string = std::vector<std::string>;

		auto super_sequences = vector_string{};
		auto hyper_sequences = vector_string{};

		auto aba_list = vector_string{};
		auto bab_list = vector_string{};

		auto sstream = std::stringstream{};
		auto token = std::string{};

		std::string line;

		while(std::getline(file, line)) {

			super_sequences.clear();
			hyper_sequences.clear();

			sstream.clear();
			sstream.str(line);

			while(sstream >> token) {
				super_sequences.push_back(token);
				// we know every second sequence is a hypernet sequence
				if(sstream >> token) {
					hyper_sequences.push_back(token);
				}
			}

			collect_objects(super_sequences, aba_list);
			collect_objects(hyper_sequences, bab_list);

			auto supports_ssl = std::any_of(aba_list.begin(), aba_list.end(), [&bab_list] (const auto& aba) {

				const auto result = std::find_if(bab_list.begin(), bab_list.end(), [&aba] (const auto& bab) {
					const auto is_reverse_equal = ((aba[0] == bab[1]) && (bab[0] == aba[1]));
					return is_reverse_equal;
				});

				const auto is_found = (result != bab_list.end());

				return is_found;
			});

			ip_count += supports_ssl;
		}

		std::cout << ip_count << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
