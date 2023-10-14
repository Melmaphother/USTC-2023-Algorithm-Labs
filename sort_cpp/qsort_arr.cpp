#include <fstream>
#include <iostream>

#include <string>
#include <utility>
#include <algorithm>

#include <ctime>
#include <random>
using namespace std;

#define K 12

class Qsort {
public:
	int *LoadData(string data_path);
	int	 GetSize() { return data_size; }
	void StoreData(int *data, string store_path);

public:
	int			   GetRandom(double Min, double Max);
	pair<int, int> Partition(int *data, int p, int r);
	pair<int, int> Random_Partition(int *data, int p, int r);
	pair<int, int> Midium_Partition(int *data, int p, int r);

public:
	void InsertSort(int *data, int p, int r);
	void QuickSort(int *data, int p, int r);
	bool Verify(int *data, int p, int r);

private:
	int data[100000];
	int data_size;
};

int *Qsort::LoadData(string data_path) {
	ifstream f;
	f.open(data_path, ios::in);
	int item;
	int count = 0;
	if (f.is_open()) {
		f >> data_size;
		while (!f.eof()) {
			f >> item;
			data[count] = item;
			count++;
			if (count == data_size) { break; }
		}
	}
	f.close();
	return this->data;
}

void Qsort::StoreData(int *data, string store_path) {
	ofstream f;
	f.open(store_path, ios::out);
	if (f.is_open()) {
		for (int i = 0; i < data_size; ++i) {
			f << data[i];
			if (i % 17 == 0 && i != 0)
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

pair<int, int> Qsort::Partition(int *data, int p, int r) {
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

pair<int, int> Qsort::Random_Partition(int *data, int p, int r) {
	int ran = GetRandom(static_cast<double>(p), static_cast<double>(r));
	swap(data[ran], data[p]);
	return Partition(data, p, r);
}

pair<int, int> Qsort::Midium_Partition(int *data, int p, int r) {
	if (r - p + 1 < 5) return make_pair(p, r);
	int arr[5]	 = {data[p], data[(3 * p + r) / 4], data[(p + r) / 2],
					data[(p + 3 * r) / 4], data[r]};
	int index[5] = {p, (3 * p + r) / 4, (p + r) / 2, (p + 3 * r) / 4, r};
	for (int i = 0; i < 5; i++) {
		for (int j = i + 1; j < 5; j++) {
			if (arr[j] < arr[i]) {
				swap(arr[i], arr[j]);
				swap(index[i], index[j]);
			}
		}
	}
	swap(data[index[2]], data[p]);
	return Partition(data, p, r);
}

void Qsort::InsertSort(int *data, int p, int r) {
	for (int i = p + 1; i <= r; i++) {
		int elem = data[i];
		int j;
		for (j = i; j > p && data[j - 1] > elem; j--) { data[j] = data[j - 1]; }
		data[j] = elem;
	}
}

void Qsort::QuickSort(int *data, int p, int r) {
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

bool Qsort::Verify(int *data, int p, int r) {
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
	cout << "duration of my quicksort is " << end - start << "ms" << endl;
	bool flag = q.Verify(arr, 0, q.GetSize() - 1);
	// cout << "flag = " << flag << endl;
	if (flag) q.StoreData(arr, "sorted.txt");

    auto	arr1 = q.LoadData("data.txt");
    start = clock();
	sort(arr1,  arr1 + q.GetSize());
	end = clock();
	cout << "duration of sort is " << end - start << "ms" << endl;
}
