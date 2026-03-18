#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min3(int a, int b, int c) {
    int min = a < b ? a : b;
    return min < c ? min : c;
}

void solve(FILE *f) {
    int rows = 0, cols = 0, max = 0;
    int max_r = 0, max_c = 0, err = 0;
    char empty, obs, full;
    
    char **map = NULL;
    int **dp = NULL;
    char *line = NULL;
    size_t len = 0;

    if (fscanf(f, "%d %c %c %c\n", \
        &rows, &empty, &obs, &full) != 4) {
        err = 1; goto end;
    }

    map = calloc(rows, sizeof(char*));
    dp = calloc(rows, sizeof(int*));

    for (int r = 0; r < rows; r++) {
        if (getline(&line, &len, f) == -1)
            err = 1; goto end;
        line[strcspn(line, "\n")] = 0; 
        
        if (r == 0) cols = strlen(line);
        else if ((int)strlen(line) != cols)
            err = 1; goto end;

        map[r] = strdup(line);
        dp[r] = calloc(cols, sizeof(int));

        for (int c = 0; c < cols; c++) {
            if (map[r][c] != empty && map[r][c] != obs)
                err = 1; goto end;
            
            if (map[r][c] == obs) {
                dp[r][c] = 0;
            } else {
                dp[r][c] = (r == 0 || c == 0) ? 1 : \
                min3(dp[r-1][c], dp[r][c-1], dp[r-1][c-1]) + 1;
                if (dp[r][c] > max) {
                    max = dp[r][c];
                    max_r = r;
                    max_c = c;
                }
            }
        }
    }
    
end:
    if (err) fprintf(stderr, "map error\n");
    else {
        for (int r = max_r - max + 1; r <= max_r; r++) {
            for (int c = max_c - max + 1; c <= max_c; c++) {
                map[r][c] = full;
            }
        }
        for (int r = 0; r < rows; r++) puts(map[r]);
    }

    if (map) {
        for (int r = 0; r < rows; r++) free(map[r]);
        free(map);
    }
    if (dp) {
        for (int r = 0; r < rows; r++) free(dp[r]);
        free(dp);
    }
    free(line);
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
