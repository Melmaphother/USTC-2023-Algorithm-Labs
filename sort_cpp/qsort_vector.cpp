#include <fstream>
#include <iostream>

#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#include <ctime>
#include <random>
using namespace std;

#define K 4

class Qsort {
public:
	vector<int> LoadData(string data_path);
	int			GetSize() { return data_size; }
	void		StoreData(vector<int> &data, string store_path);

public:
	int			   GetRandom(double Min, double Max);
	pair<int, int> Partition(vector<int> &data, int p, int r);
	pair<int, int> Random_Partition(vector<int> &data, int p, int r);
	pair<int, int> Midium_Partition(vector<int> &data, int p, int r);

public:
	void InsertSort(vector<int> &data, int p, int r);
	void QuickSort(vector<int> &data, int p, int r);
	bool Verify(vector<int> &data, int p, int r);

private:
	vector<int> data;
	int			data_size;
};

vector<int> Qsort::LoadData(string data_path) {
	ifstream f;
	f.open(data_path, ios::in);
	int item;
	int count = 0;
	if (f.is_open()) {
		f >> data_size;
		while (!f.eof()) {
			f >> item;
			data.push_back(item);
			count++;
			if (count == data_size) { break; }
		}
	}
	f.close();
	// cout << count << ' ' << data.size() << endl;
	return this->data;
}

void Qsort::StoreData(vector<int> &data, string store_path) {
	ofstream f;
	f.open(store_path, ios::out);
	if (f.is_open()) {
		for (auto i = data.begin(); i != data.end(); ++i) {
			f << *i;
			if ((i - data.begin()) % 17 == 0 && i != data.begin())
				f << '\n';
			else
				f << ' ';
		}
	}
	f.close();
}

int Qsort::GetRandom(double Min = 1, double Max = 100000) {
	static random_device rand_dev;
	static mt19937 rng(rand_dev.entropy() > 0 ? rand_dev() : time(nullptr));
	static uniform_real_distribution<double> distrib;

	return static_cast<int>(
		distrib(rng, uniform_real_distribution<double>::param_type{Min, Max}));
}

pair<int, int> Qsort::Partition(vector<int> &data, int p, int r) {
	int lptr = p, rptr = r + 1;
	int i	 = lptr;
	int base = data[p];
	while (i < rptr) {
		if (data[i] < base) {
			swap(data[i], data[lptr + 1]);
			lptr++;
			i++;
		} else if (data[i] > base) {
			swap(data[i], data[rptr - 1]);
			rptr--;
		} else {
			i++;
		}
	}
	swap(data[p], data[lptr]);
	return make_pair(lptr, rptr);
}

pair<int, int> Qsort::Random_Partition(vector<int> &data, int p, int r) {
	int ran = GetRandom(static_cast<double>(p), static_cast<double>(r));
	swap(data[ran], data[p]);
	return Partition(data, p, r);
}

pair<int, int> Qsort::Midium_Partition(vector<int> &data, int p, int r) {
	if (r - p + 1 < 3) return make_pair(p, r);
	int arr[3]	 = {data[p], data[(p + r) / 2], data[r]};
	int index[3] = {p, (p + r) / 2, r};
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 3; j++) {
			if (arr[j] < arr[i]) {
				swap(arr[i], arr[j]);
				swap(index[i], index[j]);
			}
		}
	}
	swap(data[index[1]], data[p]);
	return Partition(data, p, r);
}

void Qsort::InsertSort(vector<int> &data, int p, int r) {
	for (int i = p + 1; i <= r; i++) {
		int elem = data[i];
		int j;
		for (j = i; j > p && data[j - 1] > elem; j--) { data[j] = data[j - 1]; }
		data[j] = elem;
	}
}

void Qsort::QuickSort(vector<int> &data, int p, int r) {
	if (p > r) return;
	if (r - p + 1 <= K) {
		InsertSort(data, p, r);
		return;
	}
	// auto ret = Random_Partition(data, p, r);
	auto ret = Midium_Partition(data, p, r);
	QuickSort(data, p, ret.first - 1);
	QuickSort(data, ret.second, r);
}

bool Qsort::Verify(vector<int> &data, int p, int r) {
	bool flag = true;
	if (p > r) return false;
	for (int i = p; i < r; i++) {
		if (data[i] > data[i + 1]) {
			flag = false;
			break;
		}
	}
	return flag;
}

int main() {
	Qsort	q;
	auto	arr = q.LoadData("data.txt");
	clock_t start, end;
	start = clock();
	q.QuickSort(arr, 0, q.GetSize() - 1);
	end = clock();
	cout << "duration of quicksort is " << end - start << "ms" << endl;
	bool flag = q.Verify(arr, 0, q.GetSize() - 1);
	cout << "flag = " << flag << endl;
	if (flag) q.StoreData(arr, "sorted.txt");


    auto	arr1 = q.LoadData("data.txt");
    start = clock();
	sort(arr1.begin(),  arr1.end());
	end = clock();
	cout << "duration of sort is " << end - start << "ms" << endl;
}
