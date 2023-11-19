#include "Schedule.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>

Schedule::Schedule(std::string data_path) {
	std::ifstream in(data_path, std::ios::in);
	assert(in.is_open());
	in >> num_tasks >> num_machines;
	assert(num_tasks > 0 && num_machines > 0 && num_machines < num_tasks);
	task_time.resize(num_tasks + 1, 0);
	for (unsigned i = 1; i <= num_tasks; ++i) {
		in >> task_time[i];
		assert(task_time[i] > 0);
	}
	best_schedule.resize(num_tasks + 1, 0);
	curr_schedule.resize(num_tasks + 1, 0);
	machine_time.resize(num_machines + 1, 0);
	machine_max_task_time.resize(num_machines + 1, INT_MAX);
	best_spend = INT_MAX;
	curr_spend = 0;
}

std::pair<unsigned, std::map<unsigned, std::map<unsigned, unsigned>>>
Schedule::GetBestSchedule() {
	best_spend = InitByGreedy();
	BackTrace(1, 0);
	std::map<unsigned, std::map<unsigned, unsigned>> best_schedule_map;
	for (size_t task_index = 1; task_index <= num_tasks; ++task_index) {
		auto machine_index = best_schedule[task_index];
		// std::cout << "Task " << task_index << " Time " <<
		// task_time[task_index] << " assigned to machine " << machine_index <<
		// std::endl;
		best_schedule_map[machine_index][task_index] = task_time[task_index];
	}
	return std::make_pair(best_spend, best_schedule_map);
}

unsigned Schedule::InitByGreedy() {
	std::vector<unsigned> greedy_machine_time(num_machines + 1, 0);
	unsigned			  greedy_spend = INT_MAX;
	sort(task_time.begin() + 1, task_time.end(), std::greater<int>());
	for (auto i = 1; i <= num_machines; i++) {
		greedy_machine_time[i] += task_time[i];
	}
	greedy_spend = task_time[0];
	for (auto i = num_machines + 1; i <= num_tasks; i++) {
		auto min_index = std::min_element(greedy_machine_time.begin() + 1,
										  greedy_machine_time.end());
		*min_index += task_time[i];
		greedy_spend = std::max(greedy_spend, *min_index);
	}
	// std::cout << "Greedy spend: " << greedy_spend << std::endl;
	return greedy_spend;
}

void Schedule::BackTrace(unsigned task_index, unsigned curr_spend) {
	if (task_index == num_tasks + 1) {
		if (curr_spend < best_spend) {
			best_spend	  = curr_spend;
			best_schedule = curr_schedule;
		}
		return;
	}
	for (auto k = 1; k <= num_machines; k++) {
		if (std::max(curr_spend, machine_time[k] + task_time[task_index]) <
			best_spend) {
			if (machine_time[k] == 0) {
				if (task_time[task_index] > machine_max_task_time[k - 1]) {
					break;
				}
				machine_max_task_time[k] = task_time[task_index];
			}

			curr_schedule[task_index] = k;
			machine_time[k] += task_time[task_index];

			auto new_spend = std::max(curr_spend, machine_time[k]);
			BackTrace(task_index + 1, new_spend);

			machine_time[k] -= task_time[task_index];
			curr_schedule[task_index] = 0;

			if (machine_time[k] == 0) machine_max_task_time[k] = 0;
		}
	}
}
