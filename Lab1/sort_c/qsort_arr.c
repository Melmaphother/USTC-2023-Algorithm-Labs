#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define K 64

int *LoadData(char *data_path);
void StoreData(int *data, int data_size, char *store_path);

int	 GetRandom(double Min, double Max);
int *Partition(int *data, int p, int r);
int *Random_Partition(int *data, int p, int r);
int *Medium_Partition(int *data, int p, int r);

void InsertSort(int *data, int p, int r);
void QuickSort(int *data, int p, int r);
bool Verify(int *data, int p, int r);

void swap(int *a, int *b) {
	int tmp = *a;
	*a		= *b;
	*b		= tmp;
}

int *LoadData(char *data_path) {
	FILE *f = fopen(data_path, "r");
	int	  data_size;
	int	  count = 0;
	int	 *data	= NULL;
	if (f != NULL) {
		fscanf(f, "%d", &data_size);
		data = (int *)malloc(data_size * sizeof(int));
		while (fscanf(f, "%d", &data[count]) == 1) {
			count++;
			if (count == data_size) break;
		}
	}
	fclose(f);
	return data;
}

void StoreData(int *data, int data_size, char *store_path) {
	FILE *f = fopen(store_path, "w");
	if (f != NULL) {
		for (int i = 0; i < data_size; ++i) {
			fprintf(f, "%d", data[i]);
			if (i % 17 == 0 && i != 0)
				fprintf(f, "\n");
			else
				fprintf(f, " ");
		}
	}
	fclose(f);
}

// int GetRandom(double Min, double Max) {
// 	static random_device rand_dev;
// 	static mt19937 rng(rand_dev.entropy() > 0 ? rand_dev() : time(nullptr));
// 	static uniform_real_distribution<double> distrib;

// 	return static_cast<int>(
// 		distrib(rng, uniform_real_distribution<double>::param_type{Min, Max}));
// }

int *Partition(int *data, int p, int r) {
	int lptr = p, rptr = r + 1;
	int i	 = lptr;
	int base = data[p];
	while (i < rptr) {
		if (data[i] < base) {
			swap(data + i, data + lptr + 1);
			lptr++;
			i++;
		} else if (data[i] > base) {
			swap(data + i, data + rptr - 1);
			rptr--;
		} else {
			i++;
		}
	}
	swap(data + p, data + lptr);
	int *ret = (int *)malloc(2 * sizeof(int));
	ret[0]	 = lptr;
	ret[1]	 = rptr;
	return ret;
}

// int *Random_Partition(int *data, int p, int r) {
// 	int ran = GetRandom(static_cast<double>(p), static_cast<double>(r));
// 	swap(data[ran], data[p]);
// 	return Partition(data, p, r);
// }

int *Medium_Partition(int *data, int p, int r) {
	int arr[5]	 = {data[p], data[(3 * p + r) / 4], data[(p + r) / 2],
					data[(p + 3 * r) / 4], data[r]};
	int index[5] = {p, (3 * p + r) / 4, (p + r) / 2, (p + 3 * r) / 4, r};
	for (int i = 0; i < 5; i++) {
		for (int j = i + 1; j < 5; j++) {
			if (arr[j] < arr[i]) {
				swap(arr + i, arr + j);
				swap(index + i, index + j);
			}
		}
	}
	swap(data + index[2], data + p);
	return Partition(data, p, r);
}

void InsertSort(int *data, int p, int r) {
	for (int i = p + 1; i <= r; i++) {
		int elem = data[i];
		int j;
		for (j = i; j > p && data[j - 1] > elem; j--) { data[j] = data[j - 1]; }
		data[j] = elem;
	}
}

void QuickSort(int *data, int p, int r) {
	if (p > r) return;
	if (r - p + 1 <= K) {
		InsertSort(data, p, r);
		return;
	}
	// int* ret = Random_Partition(data, p, r);
	int *ret = Medium_Partition(data, p, r);
	QuickSort(data, p, ret[0] - 1);
	QuickSort(data, ret[1], r);
}

bool Verify(int *data, int p, int r) {
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

int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

int main() {
	int	   *arr			= NULL;
	clock_t my_duration = 0, c_lib_duration = 0;
    //../Data/data.txt
	char   *data_path = "../Data/ExtraData/1.in";
	for (size_t i = 0; i < 100; i++) {
		arr = LoadData(data_path);
		clock_t start, end;
		start = clock();
		QuickSort(arr, 0, _msize(arr) / sizeof(int) - 1);
		end = clock();
		my_duration += end - start;

		arr	  = LoadData(data_path);
		start = clock();
		qsort(arr, _msize(arr) / sizeof(int), sizeof(arr[0]), compare);
		end = clock();
		c_lib_duration += end - start;
	}
	printf("Average duration of my quicksort is %lf ms\n",
		   (double)my_duration / 100);
	printf("Average duration of c-lib-qsort is %lf ms\n",
		   (double)c_lib_duration / 100);

	bool flag = Verify(arr, 0, _msize(arr) / sizeof(int) - 1);
	if (flag) StoreData(arr, _msize(arr) / sizeof(int), "../Data/sorted.txt");
}
