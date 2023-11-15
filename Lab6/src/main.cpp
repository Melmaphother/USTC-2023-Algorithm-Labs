#include "Huffman.h"
#include <algorithm>
#include <cassert>
#include <string.h>
#include <string>
#include <utility>

std::pair<bool, std::string> getCmdOption(char **begin, char **end,
										  const std::string &option) {
	char **itr = std::find(begin, end, option);
	if (itr != end && ++itr != end) { return std::make_pair(true, *itr); }
	return std::make_pair(false, "");
}

bool cmdOptionExists(char **begin, char **end, const std::string &option) {
	return std::find(begin, end, option) != end;
}

int main(int argc, char **argv) {
	std::string data_path	= "..\\data\\original.txt";
	std::string store_path	= "..\\data\\table.txt";
	auto		help_info	= cmdOptionExists(argv, argv + argc, "-h");
	auto		input_info	= getCmdOption(argv, argv + argc, "-f");
	auto		output_info = getCmdOption(argv, argv + argc, "-o");
	if (help_info) {
		std::cout << "-h : help message" << std::endl;
		std::cout << "-f [file path] : input file path" << std::endl;
		std::cout << "-o [file path] : output file path" << std::endl;
	}
	if (input_info.first) data_path = input_info.second;
	if (output_info.first) store_path = output_info.second;
    
	HuffmanCode huffman(data_path, store_path);
	huffman.CalcCompressRatio();
	return 0;
}