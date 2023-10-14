#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

int main() {
	int	  n;
	FILE *f = fopen("data.txt", "r");
	fscanf(f, "%d", &n);
	printf("%d\n", n);
	int *arr   = (int *)malloc(n * sizeof(int));
	int	 count = 0;
	while (fscanf(f, "%d", &arr[count]) == 1) {
		count++;
		if (count == n) break;
	}
	fclose(f);
	// for (int i = 0; i < n; i++) {
	// 	printf("%d ", arr[i]);
	// 	if (i % 10 == 0 && 1 != 0) printf("\n");
	// }
	clock_t start, end;
	start = clock();
	qsort(arr, n, sizeof(arr[0]), compare);
	end = clock();
	// for (int i = 0; i < n; i++) {
	// 	printf("%d ", arr[i]);
	// 	if (i % 10 == 0 && i != 0) printf("\n");
	// }
	printf("duration of quicksort is %lf ms\n", (double)(end - start));
	return 0;
}
