#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <exception>

#define CRLFLEN 3
#define CRLF "\\r\n"

std::string remainder;

static int to_dec(std::string hex) {
	unsigned int 		x;   
	std::stringstream	ss;

	ss << std::hex << hex;
	ss >> x;
	return (static_cast<int>(x));
}

std::string	get_file_content(std::string const& file_location) {
	std::ifstream	input_stream;
	std::string		file_contents;

	input_stream.open(file_location);
	if (!input_stream || input_stream.bad()) {
		throw (std::exception());
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		file_contents.append(ss.str());
	}
	input_stream.close();
	return (file_contents);
}

bool	is_hex(std::string const& s) {
	for (size_t i = 0; i < s.length(); i++) {
		if (!isxdigit(s[i])) {
			return (false);
		}
	}
	return (true);
}

static int	get_dec(std::string str, size_t& index) {
	str = str.substr(index);

	std::string	s;
	size_t 		pos = str.find(CRLF);

	if (pos != std::string::npos) {
		s = str.substr(0, pos);
		if (!is_hex(s)) {
			return (-1);
		}
		index += (pos + CRLFLEN);
		return (to_dec(s));
	}
	return (-1);
}

static std::string	handle_pair(std::string str, size_t& index) {
	size_t	start = index;
	int dec = get_dec(str, index);
	if (dec == -1) {
		throw (std::runtime_error("bad chunked request"));
	}
	int total = 0;
	std::string res;

	str = str.substr(index);
	for (size_t pos = str.find(CRLF); pos != std::string::npos; pos = str.find(CRLF)) {
		std::string to_add = str.substr(0, pos);
		if (to_add.empty() && dec == 0) {
			return ("");
		}
		res += to_add;
		index += to_add.length() + CRLFLEN;
		str = str.substr(to_add.length() + CRLFLEN);
		total += to_add.length();
		if (total == dec) {
			return (res);
		}
		if (total > dec) {
			throw (std::runtime_error("bad chunked request"));
		}
		res.append("\n");
		total += 2;
	}
	index = start;
	return ("");
}

std::string chonky(std::string str) {
	size_t index = 0;
	std::string s;

	while (1) {
		std::string res;
		try {
			res = handle_pair(str, index);
		}
		catch (std::runtime_error const& e) {
			std::cout << e.what() << std::endl;
			return ("");
		}
		if (res.empty())
			break ;
		s.append(res);
	}
	remainder = str.substr(index);
	return (s);
}

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char**argv, char **envp) {
	// std::string request;
	// std::string first = get_file_content("chonky.txt");

	// std::string	second = get_file_content("chonky2.txt");

	// request = chonky(remainder.append(first));
	// request.append(chonky(remainder.append(second)));
	// std::cout << request << std::endl;
	if (argc != 4) {
		return (1);
	}

	std::string testString("a\0b\0c\0d", 7);

	std::stringstream ss;
	ss << testString;
	std::string s;
	s = ss.str();
	std::cout << s << std::endl;
	std::cout << s.c_str() << std::endl;

	int fd = open("tmp.txt", O_CREAT | O_TRUNC | O_WRONLY, 0777);

	for (int i = 1; argv[i]; i++) {
		std::cout << argv[i] << std::endl;
	}

	write(fd, testString.c_str(), testString.size());
	close(fd);
	execve(argv[1], argv + 1, envp);

	return (0);
}
