#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <cstdint>

#include <freeglut.h>

class OriginalTexture {
	std::string split_string(const std::string& str, uint8_t begin, char sep, uint8_t& end);
	std::vector<uint16_t> line2vec(const std::string& line, uint8_t num);

public:
	GLubyte image[512][512][3];

	void init(const std::string& file_path) {
		std::ifstream ifs{ file_path };
		if (!ifs) {
			std::cerr << "load tex failed" << std::endl;
		}

		std::string head;
		std::getline(ifs, head);
		line2vec(head, 3);

		std::string line;
		for (int i = 0;i < 512;i++) {
			for (int j = 0;j < 512;j++) {
				std::getline(ifs, line);
				std::vector<uint16_t> pix = line2vec(line, 3);
				for (int k = 0; k < 3; k++) {
					image[i][j][k] = pix[k];
				}
			}
		}
	}
};
