#include "texture.h"

std::string OriginalTexture::split_string(const std::string& str, uint8_t begin, char sep, uint8_t& end) {
	//include begin place
	uint16_t str_size = str.size();

	end = str_size;
	for (int i = begin;i < str_size;i++) {
		if (str[i] == sep) {
			end = i;
			break;
		}
	}
	//std::cout << split_begin << ":" << split_end << std::endl;
	std::string split = str.substr(begin, end - begin);
	return split;
}


std::vector<uint16_t> OriginalTexture::line2vec(const std::string& line, uint8_t num) {
	const char sep = ',';
	uint8_t sep_start = 0;
	std::vector<uint16_t> result{};

	uint8_t s = 0;
	uint8_t e = 0;
	for (int i = 0;i < num - 1;i++) {
		std::string sp = split_string(line, s, ',', e);
		result.push_back(static_cast<uint16_t>(stoi(sp)));
		s = e + 1;
	}
	std::string sp = split_string(line, s, ' ', e);
	result.push_back(static_cast<uint16_t>(stoi(sp)));

	return result;
}