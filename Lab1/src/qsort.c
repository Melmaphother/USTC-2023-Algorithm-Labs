#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int *LoadData(char *data_path);
void StoreData(int *data, int data_size, char *store_path);

int	 GetRandom(int Min, int Max);
int *Partition(int *data, int p, int r);
int *Random_Partition(int *data, int p, int r);
int *Medium_Partition(int *data, int p, int r);

void InsertSort(int *data, int p, int r);
void QuickSort_Medium_Partition(int *data, int p, int r, int K);
void QuickSort_Random_Partition(int *data, int p, int r, int K);
void QuickSort_Partition(int *data, int p, int r, int K);
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
	if (f == NULL) {
		printf("No such file or dictionary!\n");
	} else {
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

int GetRandom(int Min, int Max) {
	srand(time(NULL)); // initialize generator of random number
	return rand() % (Max + 1 - Min) + Min;
}

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

int *Random_Partition(int *data, int p, int r) {
	int ran = GetRandom(p, r);
	swap(data + ran, data + p);
	return Partition(data, p, r);
}

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

void QuickSort_Medium_Partition(int *data, int p, int r, int K) {
	if (p > r) return;
	if (r - p + 1 <= K) {
		InsertSort(data, p, r);
		return;
	}
	// int* ret = Random_Partition(data, p, r);
	int *ret = Medium_Partition(data, p, r);
	QuickSort_Medium_Partition(data, p, ret[0] - 1, K);
	QuickSort_Medium_Partition(data, ret[1], r, K);
}

void QuickSort_Random_Partition(int *data, int p, int r, int K) {
	if (p > r) return;
	if (r - p + 1 <= K) {
		InsertSort(data, p, r);
		return;
	}
	int *ret = Random_Partition(data, p, r);
	QuickSort_Random_Partition(data, p, ret[0] - 1, K);
	QuickSort_Random_Partition(data, ret[1], r, K);
}

void QuickSort_Partition(int *data, int p, int r, int K) {
	if (p > r) return;
	if (r - p + 1 <= K) {
		InsertSort(data, p, r);
		return;
	}
	int *ret = Partition(data, p, r);
	QuickSort_Partition(data, p, ret[0] - 1, K);
	QuickSort_Partition(data, ret[1], r, K);
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

int main(int argc, char **argv) {
	int	   *arr = NULL;
	clock_t start, end;
	clock_t duration_medium = 0, duration_random = 0, duration = 0,
			c_lib_duration = 0;
	char *data_path = "../Data/ExtraData/1.in";
    char *store_path = "../Data/sorted.txt";
	int	  K			= 32;
	if (argc < 1) {
		return 0;
	} else {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-f") == 0) {
				data_path = argv[i + 1];
			} else if (strcmp(argv[i], "-K") == 0 || strcmp(argv[i], "-k") == 0) {
				K = atoi(argv[i + 1]);
            }
            else if (strcmp(argv[i], "-o") == 0){
                store_path = argv[i + 1];
            }
			else if (strcmp(argv[i], "-h") == 0) {
				printf("-h    : help message\n");
				printf("-f    : file path\n");
                printf("-o    : store path\n");
				printf(
					"-K/-k : 2 ~ 128, length of sequence when stop recursion\n");
			}
		}
	}
	if (K < 2 ) {
		printf("please modify you parameter '-K'\n");
		return 0;
	}
	for (size_t i = 0; i < 100; i++) {
		arr	  = LoadData(data_path);
		start = clock();
		QuickSort_Medium_Partition(arr, 0, _msize(arr) / sizeof(int) - 1, K);
		end = clock();
		duration_medium += end - start;
	}
	for (size_t i = 0; i < 100; i++) {
		arr	  = LoadData(data_path);
		start = clock();
		QuickSort_Random_Partition(arr, 0, _msize(arr) / sizeof(int) - 1, K);
		end = clock();
		duration_random += end - start;
	}
	for (size_t i = 0; i < 100; i++) {
		arr	  = LoadData(data_path);
		start = clock();
		QuickSort_Partition(arr, 0, _msize(arr) / sizeof(int) - 1, K);
		end = clock();
		duration += end - start;
	}
	for (size_t i = 0; i < 100; i++) {
		arr	  = LoadData(data_path);
		start = clock();
		qsort(arr, _msize(arr) / sizeof(int), sizeof(arr[0]), compare);
		end = clock();
		c_lib_duration += end - start;
	}
	printf("Average duration of my quicksort (medium partition) is %lf ms\n",
		   (double)duration_medium / 100);
	printf("Average duration of my quicksort (random partition) is %lf ms\n",
		   (double)duration_random / 100);
	printf("Average duration of my quicksort (static partition) is %lf ms\n",
		   (double)duration / 100);
	printf("Average duration of c-lib-qsort is %lf ms\n",
		   (double)c_lib_duration / 100);

	// test and store
	arr = LoadData(data_path);
	QuickSort_Medium_Partition(arr, 0, _msize(arr) / sizeof(int) - 1, K);
	bool flag = Verify(arr, 0, _msize(arr) / sizeof(int) - 1);
	flag == true ? printf("Already sorted!") : printf("Not sorted!");
	if (flag) StoreData(arr, _msize(arr) / sizeof(int), store_path);
}
