#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int rows, cols;
	char empty, obs, full;
	char **map;
} t_bsq;

int min3(int a, int b, int c) {
	int m = a < b ? a : b;
	return m < c ? m : c;
}

void solve(t_bsq *b) {
	int **dp = malloc(b->rows * sizeof(int *));
	int max = 0, max_r = 0, max_c = 0;

	for (int r = 0; r < b->rows; r++) {
		dp[r] = calloc(b->cols, sizeof(int));
		for (int c = 0; c < b->cols; c++) {
			if (b->map[r][c] == b->obs)
				dp[r][c] = 0;
			else if (r == 0 || c == 0)
				dp[r][c] = 1;
			else
				dp[r][c] = min3(dp[r-1][c], dp[r][c-1], dp[r-1][c-1]) + 1;

			if (dp[r][c] > max) {
				max = dp[r][c]; max_r = r; max_c = c;
			}
		}
	}

	for (int r = max_r - max + 1; r <= max_r; r++)
		for (int c = max_c - max + 1; c <= max_c; c++)
			b->map[r][c] = b->full;

	for (int r = 0; r < b->rows; r++) {
		for (int c = 0; c < b->cols; c++) putchar(b->map[r][c]);
		putchar('\n');
		free(dp[r]); free(b->map[r]);
	}
	free(dp);
	free(b->map);
}

void process(FILE *f) {
	t_bsq b = {0};
	char *line = NULL; size_t len = 0;

	if (fscanf(f, "%d %c %c %c\n", &b.rows, &b.empty, &b.obs, &b.full) != 4)
		return (void)fprintf(stderr, "map error\n");

	b.map = malloc(b.rows * sizeof(char *));
	for (int i = 0; i < b.rows; i++) {
		if (getline(&line, &len, f) == -1) return (void)fprintf(stderr, "map error\n");
		line[strcspn(line, "\n")] = 0; // Remove newline quickly
		if (i == 0) b.cols = strlen(line);
		b.map[i] = strdup(line); // strdup does malloc+strcpy
	}
	free(line);
	solve(&b);
}

int main(int ac, char **av) {
	if (ac == 1) process(stdin);
	for (int i = 1; i < ac; i++) {
		FILE *f = fopen(av[i], "r");
		if (f) { process(f); fclose(f); }
		else fprintf(stderr, "map error\n");
	}
	return 0;
}
