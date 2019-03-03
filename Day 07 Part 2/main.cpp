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

template<typename S, typename L>
auto collect_objects(const S& sequences, L& list) {

	for(const auto& seq : sequences) {

		for(auto i = 0UL, j = (i + 2); j < seq.size(); ++i, ++j) {

			auto has_object = ((seq[i] != seq[i+1]) && (seq[i] == seq[j]));

			if(has_object) {
				list.emplace_back(seq.substr(i, 3));
			}
		}
	}
}

int main() {

	auto filename = std::string{"ip_list.txt"};
	auto file = std::fstream{filename};

	if(file.is_open()) {

		auto super_sequences = std::vector<std::string>{};
		auto hyper_sequences = std::vector<std::string>{};

		auto sstream = std::stringstream{};
		auto token = std::string{};

		auto aba_list = std::vector<std::string>{};
		auto bab_list = std::vector<std::string>{};

		auto ssl_count = std::count_if(std::istream_iterator<Reader>{file}, {}, [&] (auto& reader) {

			sstream.clear();
			sstream.str(reader.line);

			super_sequences.clear();
			hyper_sequences.clear();

			for(auto id = 0UL; (sstream >> token); ++id) {

				// every second sequence is a hypernet sequence
				if(id % 2 == 0) { super_sequences.push_back(token); }
				else { hyper_sequences.push_back(token); }
			}

			aba_list.clear();
			bab_list.clear();

			collect_objects(super_sequences, aba_list);
			collect_objects(hyper_sequences, bab_list);

			auto supports_ssl = std::any_of(aba_list.begin(), aba_list.end(), [&] (const auto& aba) {

				const auto result = std::find_if(bab_list.begin(), bab_list.end(), [&] (const auto& bab) {
					return ((aba[0] == bab[1]) && (bab[0] == aba[1]));
				});

				return (result != bab_list.end());
			});

			return supports_ssl;
		});

		std::cout << ssl_count << std::endl;

	} else {
		std::cerr << "Error! Could not open file \"" << filename << "\"!" << std::endl;
	}

	return 0;
}
