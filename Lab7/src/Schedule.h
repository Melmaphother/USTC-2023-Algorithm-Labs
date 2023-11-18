#include <map>
#include <string>
#include <utility>
#include <vector>

class Schedule {
public:
	Schedule(std::string data_path);
	std::pair<unsigned, std::map<unsigned, std::map<unsigned, unsigned>>>
			 GetBestSchedule();
	unsigned InitByGreedy();
	void	 BackTrace(unsigned task_index, unsigned curr_spend);

private:
	std::vector<unsigned> task_time;
	unsigned			  num_tasks;
	unsigned			  num_machines;

private:
	//* best_schedule[num_tasks] indicates which machine the i−th task is
	//* assigned to.
	//! task_index start from 1, best_schedule[i] == 0 means no arrangment
	std::vector<unsigned> best_schedule;
	//* curr_schedule[task_index] indicates the temporary arrangement of which
	//* machine the task_index−th task is assigned to.
	std::vector<unsigned> curr_schedule;
	//* machine_time[machine_index] indicates the total time of the tasks in the
	//* queue of the machine_index-th machine.
	//! machine_index start from 1
	std::vector<unsigned> machine_time;
	//* machine_max_task_time[machine_index] indicates the max time of the tasks
	//* in the queue of the machine_index-th machine.
    //* as I pre-sort the task_time, so the max time of the tasks in each machine
    //* is the first task in the queue.
    //* So if the machine_max_task_time[machine_index] is larger than its previous
    //* it is a duplicate case. 
    //* This method can reduce the time complexity from O(n) to O(n/k!).
	std::vector<unsigned> machine_max_task_time;
	//* best_spend indicates the time spend in the best arrangement.
	unsigned best_spend;
	//* curr_spend indicates the time spend in the current arrangement.
	//* which is the longest time of the machine queue.
	unsigned curr_spend;
};
