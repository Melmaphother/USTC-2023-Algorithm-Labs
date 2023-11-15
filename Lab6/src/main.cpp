#include "Huffman.h"
#include <algorithm>
#include <string>

bool cmdOptionExists(char **begin, char **end, const std::string &option) {
	return std::find(begin, end, option) != end;
}

int main(int argc, char **argv) {
	std::string data_path  = "..\\data\\origin.txt";
	std::string store_path = "..\\data\\table.txt";
	if (cmdOptionExists(argv, argv + argc, "-h")) {
		std::cout << "-h : help message" << std::endl;
		std::cout << "-f [file path] : input file path" << std::endl;
		std::cout << "-o [file path] : output file path" << std::endl;
	}
	if (cmdOptionExists(argv, argv + argc, "-f")) {
		if (argc >= 3) {
			data_path = argv[2];
		} else {
			std::cout << "Lack of parameter." << std::endl;
			std::cout << "Use \"-h\" to get some help." << std::endl;
		}
	}
	if (cmdOptionExists(argv, argv + argc, "-o")) {
		if (argc >= 3) {
			store_path = argv[2];
		} else {
			std::cout << "Lack of parameter." << std::endl;
			std::cout << "Use \"-h\" to get some help." << std::endl;
		}
	}
	HuffmanCode huffman(data_path, store_path);
	huffman.CalcCompressRatio();
	return 0;
}