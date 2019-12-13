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

// NOTE: it takes a couple seconds for this program to finish
int main() {

	auto pwd = std::string{};

	constexpr decltype(pwd.size()) pwdlen = 8;

	const auto door_id = std::string{"reyedfim"};

	const auto prefix = std::string{"00000"};
	const auto prefix_size = prefix.size();

	auto md5 = Poco::MD5Engine{};
	auto out = Poco::DigestOutputStream{md5};

	for(decltype(pwd.size()) i = 0; (pwd.size() < pwdlen); ++i) {

		// interestingly enough, if I write out << (door_id + std::to_string(i))
		// instead, the program becomes slower - I guess, the additional time is
		// spent on creating strings over and over again
		out << door_id << i;

		out.close();

		// retrieve the hash in hexadecimal form as a string
		const auto result = Poco::DigestEngine::digestToHex(md5.digest());

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
