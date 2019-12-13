/*
 * NOTE:
 *
 * I'm using the following flags:
 *
 * Linker: -lPocoFoundation
 *
 */
#include <algorithm>
#include <iostream>
#include <string>

#include <Poco/DigestEngine.h>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>

#include <boost/range/irange.hpp>

// NOTE: this program takes a couple seconds to run
int main() {

	using uint64 = std::uint64_t;

	constexpr auto pwdlen = uint64{8};
	constexpr auto empty = ' ';

	auto pwd = std::string(pwdlen, empty);

	const auto door_id = std::string{"reyedfim"};

	const auto prefix = std::string{"00000"};
	const auto prefix_size = prefix.size();

	const auto letter_pos = (prefix_size + 1);

	auto letter_count = uint64{};

	auto md5 = Poco::MD5Engine{};
	auto out_stream = Poco::DigestOutputStream{md5};

	for(const auto i : boost::irange(std::numeric_limits<uint64>::max())) {

		out_stream << door_id << i;

		out_stream.close();

		const auto result = Poco::DigestEngine::digestToHex(md5.digest());

		const auto has_prefix = (result.compare(0, prefix_size, prefix) == 0);

		if(has_prefix) {

			const auto pwd_pos = result[prefix_size];

			if(std::isdigit(pwd_pos)) {

				auto& pwd_letter = pwd[pwd_pos-'0'];

				// we want to avoid overwriting an already filled position of the password
				if(pwd_letter == empty) {

					pwd_letter = result[letter_pos];
					++letter_count;

					if(letter_count >= pwdlen) {
						break;
					}
				}
			}
		}
	}

	std::cout << pwd << std::endl;

	return 0;
}
