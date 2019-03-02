#include <algorithm>
#include <iostream>
#include <string>

#include <Poco/DigestEngine.h>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>

int main() {

	auto pwd = std::string{};
	constexpr auto pwdlen = 8UL;

	const auto door_id = std::string{"reyedfim"};

	const auto prefix = std::string{"00000"};

	auto md5 = Poco::MD5Engine{};
	auto out = Poco::DigestOutputStream{md5};

	for(auto i = 0UL; (pwd.size() < pwdlen); ++i) {

		out << door_id << i;

		out.flush();

		const auto result = Poco::DigestEngine::digestToHex(md5.digest());

		auto has_prefix = (result.compare(0, prefix.size(), prefix) == 0);

		if(has_prefix) { pwd += result[prefix.size()]; }
	}

	std::cout << pwd << std::endl;

	return 0;
}
