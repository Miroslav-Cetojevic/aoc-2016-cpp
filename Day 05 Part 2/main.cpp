/*
 * NOTE: this code uses the poco library and is compiled with the -lPocoFoundation flag
 */
#include <algorithm>
#include <iostream>
#include <string>

#include <Poco/DigestEngine.h>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>

int main() {

	auto pwd = std::string{};
	constexpr auto pwdlen = 8UL;
	constexpr auto empty = '_';
	pwd.resize(pwdlen);
	std::fill(pwd.begin(), pwd.end(), empty);

	const auto door_id = std::string{"reyedfim"};

	const auto prefix = std::string{"00000"};
	const auto pxsize = prefix.size();
	const auto letter_pos = (pxsize + 1);

	auto md5 = Poco::MD5Engine{};
	auto out = Poco::DigestOutputStream{md5};

	for(auto i = 0UL, letter_count = 0UL; (letter_count < pwdlen); ++i) {

		out << door_id << i;

		out.flush();

		const auto result = Poco::DigestEngine::digestToHex(md5.digest());

		const auto has_prefix = (result.compare(0, pxsize, prefix) == 0);

		const auto pwd_pos = result[pxsize];

		auto& pwd_letter = pwd[pwd_pos-'0'];

		auto is_valid = (std::isdigit(pwd_pos) != 0) && (pwd_letter == empty);

		if(has_prefix && is_valid) {
			pwd_letter = result[letter_pos];

			++letter_count;
		}
	}

	std::cout << pwd << std::endl;

	return 0;
}
