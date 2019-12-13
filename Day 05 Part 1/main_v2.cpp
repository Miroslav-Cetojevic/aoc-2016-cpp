/*
 * NOTE:
 *
 * I'm using the following flags:
 *
 * Compiler: -I/usr/include/botan-2
 * Linker  : -lbotan-2
 *
 */
#include <algorithm>
#include <iostream>
#include <string>

#include <botan/hash.h>
#include <botan/hex.h>

// NOTE: it takes a couple seconds for this program to finish
int main() {

	auto pwd = std::string{};

	constexpr decltype(pwd.size()) pwdlen = 8;

	const auto door_id = std::string{"reyedfim"};

	const auto prefix = std::string{"00000"};
	const auto prefix_size = prefix.size();

	auto md5 = Botan::HashFunction::create("MD5");

	for(decltype(pwd.size()) i = 0; (pwd.size() < pwdlen); ++i) {

		md5->update(door_id + std::to_string(i));

		// retrieve the hash in hexadecimal form as a string
		const auto result = Botan::hex_encode(md5->final());

		// uncomment this if you want to watch the program generating the password
//		std::cout << pwd << result[prefix_size] << std::endl;

		const auto has_prefix = (result.compare(0, prefix_size, prefix) == 0);

		// this is where the password construction happens
		if(has_prefix) {
			pwd += result[prefix_size];
		}
	}

	std::cout << "Password: " << pwd << std::endl;

	return 0;
}
