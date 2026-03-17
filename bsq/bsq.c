#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int m3(int a, int b, int c) {
    int m = a < b ? a : b;
    return m < c ? m : c;
}

void solve(FILE *f) {
    int rows = 0, cols = 0, max = 0, mr = 0, mc = 0, err = 0;
    char e, o, x;
    char **map = NULL; int **dp = NULL; char *line = NULL; size_t len = 0;

    if (fscanf(f, "%d %c %c %c\n", &rows, &e, &o, &x) != 4) { err = 1; goto end; }

    // Use calloc so uninitialized rows start as NULL (safe to free later)
    map = calloc(rows, sizeof(char*));
    dp = calloc(rows, sizeof(int*));

    for (int r = 0; r < rows; r++) {
        if (getline(&line, &len, f) == -1) { err = 1; goto end; }
        line[strcspn(line, "\n")] = 0; 
        
        if (r == 0) cols = strlen(line);
        else if ((int)strlen(line) != cols) { err = 1; goto end; }

        map[r] = strdup(line);
        dp[r] = calloc(cols, sizeof(int));

        for (int c = 0; c < cols; c++) {
            if (map[r][c] != e && map[r][c] != o) { err = 1; goto end; }
            if (map[r][c] == o) dp[r][c] = 0;
            else {
                dp[r][c] = (r == 0 || c == 0) ? 1 : m3(dp[r-1][c], dp[r][c-1], dp[r-1][c-1]) + 1;
                if (dp[r][c] > max) { max = dp[r][c]; mr = r; mc = c; }
            }
        }
    }
    
end:
    if (err) fprintf(stderr, "map error\n");
    else {
        for (int r = mr - max + 1; r <= mr; r++)
            for (int c = mc - max + 1; c <= mc; c++) map[r][c] = x;
        for (int r = 0; r < rows; r++) puts(map[r]);
    }

    // --- LEAK CLEANUP ---
    if (map) {
        for (int r = 0; r < rows; r++) free(map[r]);
        free(map);
    }
    if (dp) {
        for (int r = 0; r < rows; r++) free(dp[r]);
        free(dp);
    }
    free(line); // getline allocates 'line' even if it crashes
}

int main(int ac, char **av) {
    if (ac == 1) solve(stdin);
    for (int i = 1; i < ac; i++) {
        FILE *f = fopen(av[i], "r");
        if (!f) fprintf(stderr, "map error\n");
        else { solve(f); fclose(f); }
    }
    return 0;
}
