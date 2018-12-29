#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b) {
	return a > b ? a : b;
}

int MFK(int bag, int *weight, int *price, int len) {
	int i, j, p = 0;

	// alloc F[i][j] and init
	int **F = (int **)malloc(sizeof(int *) * (len + 1));
	for (i = 0; i <= len; i++) {
		F[i] = (int *)malloc(sizeof(int) * (bag + 1));
		memset(F[i], 0, sizeof(int) * (bag + 1));
	}

	// get F[i][j]
	for (i = 1; i <= len; i++) {
		for (j = 1; j <= bag; j++) {
			F[i][j] = F[i - 1][j];
			if (j - weight[i - 1] >= 0) {
				F[i][j] = max(F[i][j], F[i - 1][j - weight[i - 1]] + price[i - 1]);
			}
		}
	}

	p = F[len][bag];

	for (i = 0; i <= len; i++) { free(F[i]); } free(F);
	return p;
}

int main() {
	int weight[] = {3, 2, 1, 4, 5};
	int price[] = {25, 20, 15, 40, 50};
	int len = sizeof(weight) / sizeof(int);

	int bag = 6;
	int p = MFK(bag, weight, price, len);
	printf("MFK: %d\n", p);

	return 0;
}
