#include "IntervalTree.h"
#include <utility>

typedef std::pair<int, int> Key_Type;

// #ifndef DEBUG
// #define DEBUG
// #endif

template <typename T>
void InsertAllNodes(RBTree<T> &RBT, const std::string Insert_path) {
	std::ifstream f;
	int			  num, low, high;
	T			  key;
	f.open(Insert_path, std::ios::in);
	if (f.is_open()) {
		f >> num;
		for (int i = 0; i < num; i++) {
			f >> low >> high;
			key = std::make_pair(low, high);
			RBT.RBInsert(key);
		}
	} else {
		std::cout << "Can not open" << Insert_path << std::endl;
	}
	f.close();
}
template <typename T> void Search(RBTree<T> &RBT) {
    bool isFirst = true;
	while (true) {
        std::string message = isFirst ? "Input interval: [like: 12 14] ": "Input intercal: "; 
        isFirst = false;
		std::cout << message;
		int low, high;
		std::cin >> low >> high;
		getchar(); // fflush

		auto		   interval = std::make_pair(low, high);
		std::vector<T> result;
		RBT.IntervalSearch(interval, result);
		if (result.empty()) {
			std::cout << "No result" << std::endl;
		} else {
			std::cout << "The result is as follows:" << std::endl;
		}
		for (auto &i : result) { std::cout << i; }

		std::cout << "Continue? [Y/n] ";
		char c;
		std::cin >> c;
		if (c == 'n') break;
	}
}

int main(int argc, char **argv) {
	RBTree<Key_Type> RBT;
	std::string		 Insert_path = "insert.txt";
	std::string		 LNR_path	 = "Result/LNR.txt";
	std::string		 NLR_path	 = "Result/NLR.txt";
	std::string		 LOT_path	 = "Result/LOT.txt";
	InsertAllNodes<Key_Type>(RBT, Insert_path);
	RBT.Display(LNR_path, NLR_path, LOT_path);
	Search<Key_Type>(RBT);
	return 0;
}