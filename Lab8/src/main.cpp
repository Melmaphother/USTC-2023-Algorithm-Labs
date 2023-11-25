#include "SparseMatrix.h"

#include <algorithm>
#include <iostream>
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
	std::string data_path  = "../data/twitter_small.txt";
	std::string mode	   = "small";
	auto		help_info  = cmdOptionExists(argv, argv + argc, "-h");
	auto		input_info = getCmdOption(argv, argv + argc, "-f");
	auto		mode_info  = getCmdOption(argv, argv + argc, "-m");
	if (help_info) {
		std::cout << "-h : help message" << std::endl;
		std::cout << "-f [file path] : input file path" << std::endl;
		std::cout << "-m [small/large] : input file mode" << std::endl;
	}
	if (input_info.first) data_path = input_info.second;
	if (mode_info.first) mode = mode_info.second;

	SparseMatrix sm(data_path, mode);

	// std::cout << sm.get_num_vertices() << ' ' << sm.get_num_edges()
	// 		  << std::endl;
    
	sm.bfs();
	return 0;
}
