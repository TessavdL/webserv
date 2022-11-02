#include <vector>
#include <string>
#include <iostream>

std::vector<std::string>	split_string_on_delimeter_string(std::string str, std::string const& delimeter) {
    size_t                      pos;
    std::string                 tmp;
	std::vector<std::string>	v;

	while ((pos = str.find(delimeter)) != std::string::npos) {
		tmp = str.substr(0, pos);
		v.push_back(tmp);
        str.erase(0, pos += delimeter.length());
	}
    if (!str.empty()) {
        v.push_back(str);
    }
	return (v);
}

int main(void) {
    std::string str = "hi \thello \they";
    std::vector<std::string> v;

    v = split_string_on_delimeter_string(str, " \t");
    for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++) {
        std::cout << *it << std::endl;
    }
    return (0);
}