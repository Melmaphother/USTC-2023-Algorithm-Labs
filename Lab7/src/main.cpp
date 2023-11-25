#include "Schedule.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
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
	std::string data_path  = "..\\data\\test1.txt";
	auto		help_info  = cmdOptionExists(argv, argv + argc, "-h");
	auto		input_info = getCmdOption(argv, argv + argc, "-f");
	if (help_info) {
		std::cout << "-h : help message" << std::endl;
		std::cout << "-f [file path] : input file path" << std::endl;
	}
	if (input_info.first) data_path = input_info.second;

    auto start = std::chrono::high_resolution_clock::now();

	Schedule schedule(data_path);
	auto	 best_schedule = schedule.GetBestSchedule();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Duration of get best schedule is " << duration.count() << "us" << std::endl;

	std::cout << "Best spend: " << best_schedule.first << std::endl;
	std::cout << "Best schedule: " << std::endl;
	for (auto [machine, tasks] : best_schedule.second) {
		std::cout << "Machine " << machine << ": ";
		auto total_time = 0;
		for (auto [task_index, task_time] : tasks) { total_time += task_time; }
		std::cout << "Total time: " << total_time << "  ";
		for (auto [task_index, task_time] : tasks) {
			std::cout << '(' << task_index << ", " << task_time << ')' << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}
