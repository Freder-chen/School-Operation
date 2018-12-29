#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* base func */
int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

/* currency maximization problem */
int currency_max_recursive(int *coin_list, int coin_size) {
	switch(coin_size) {
		case 0: return 0;
		case 1: return coin_list[0];
		default: return max(
			coin_list[coin_size - 1] + currency_max_recursive(coin_list, coin_size - 2),
			currency_max_recursive(coin_list, coin_size - 1)
		);
	}
}

int _currency_max_dp(int *coin_list, int coin_size, int *value_list) {
	int i;

	value_list[1] = coin_list[0];
	for (i = 2; i <= coin_size; i++) {
		value_list[i] = max(coin_list[i - 1] + value_list[i - 2], value_list[i - 1]);
	}

	return value_list[coin_size];
}

int currency_max_dp(int *coin_list, int coin_size) {
	int *value_list = (int *)malloc(sizeof(int) * (coin_size + 1));
	memset(value_list, 0, sizeof(int) * (coin_size + 1));

	int value = _currency_max_dp(coin_list, coin_size, value_list);
	free(value_list);
	return value;
}

int currency_max(int *coin_list, int coin_size, const char *prop) {
	if (strcmp(prop, "dp") == 0) {
		return currency_max_dp(coin_list, coin_size);
	}
	else if (strcmp(prop, "rec") == 0) {
		return currency_max_recursive(coin_list, coin_size);
	}
	else {
		return -1;
	}
}

/* change problem */
int change_problem_recursive(int value, int *coin_list, int coin_size) {
	if (value == 0) return 0;
	if (value - coin_list[0] <= 0) return 0;

	int i, mc = change_problem_recursive(value - coin_list[0], coin_list, coin_size);
	for (i = 1; i < coin_size; i++) {
		if (value - coin_list[i] >= 0) {
			mc = min(mc, change_problem_recursive(value - coin_list[i], coin_list, coin_size));
		}
	}
	
	return mc + 1;
}

int _change_problem_dp(int value, int *coin_list, int coin_size, int *count_list) {
	int i, j;

	count_list[0] = 0;
	for (i = 1; i <= value; i++) {
		for (j = 0;  j < coin_size; j++) {
			if (i - coin_list[j] >= 0) {
				count_list[i] = min(count_list[i - coin_list[j]] + 1, count_list[i]);
			}
		}
	}

	return count_list[value];
}
int change_problem_dp(int value, int *coin_list, int coin_size) {
	int i, *count_list = (int *)malloc(sizeof(int) * (value + 1));
	for (i = 0; i <= value; i++) { count_list[i] = 999; }

	int count = _change_problem_dp(value, coin_list, coin_size, count_list);
	free(count_list);
	return count;
}

int change_problem(int value, int *coin_list, int coin_size, const char *prop) {
	if (strcmp(prop, "dp") == 0) {
		return change_problem_dp(value, coin_list, coin_size);
	}
	else if (strcmp(prop, "rec") == 0) {
		return change_problem_recursive(value, coin_list, coin_size);
	}
	else {
		return -1;
	}
}

void fun(int n) {
	clock_t start_t, end_t;
	int value, i;

	// get n elements
	int coin_size = n;
	int *coin_list = (int *)malloc(sizeof(int) * coin_size);
	for (i = 0; i < coin_size; i++) {
		// coin_list[i] = rand() % (coin_size / 5) + 2;
		coin_list[i] = i + 1;
	}
	
	// start_t = clock();
	// value = currency_max(coin_list, coin_size, "dp");
	// end_t = clock();
	// printf("dp: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);

	// start_t = clock();
	// value = currency_max(coin_list, coin_size, "res");
	// end_t = clock();
	// printf("res: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);

	start_t = clock();
	value = change_problem(coin_size * 10 + 7, coin_list, coin_size, "dp");
	end_t = clock();
	printf("dp: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);

	start_t = clock();
	value = change_problem(coin_size * 10 + 7, coin_list, coin_size, "rec");
	end_t = clock();
	printf("res: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);

	free(coin_list);
}

/* Main */
int main() {
	fun(30);
	// printf("Currency maximization problem:\n");
	// int coin_list[] = {5, 1, 2, 10, 6, 2};
	// int coin_size = sizeof(coin_list) / sizeof(int);
	// int value = currency_max(coin_list, coin_size, "dp");
	// printf("dp: %d\n", value);
	// value = currency_max(coin_list, coin_size, "rec");
	// printf("rec: %d\n", value);

	// printf("Change problem:\n");
	// int coin_list2[] = {1, 3, 4};
	// int coin_size2 = sizeof(coin_list2) / sizeof(int);
	// int number = change_problem(6, coin_list2, coin_size2, "dp");
	// printf("dp: %d\n", number);
	// number = change_problem(6, coin_list2, coin_size2, "rec");
	// printf("rec: %d\n", number);

	return 0;
}