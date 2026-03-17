#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int rows, cols;
    char empty, obs, full;
    char **map;
} t_bsq;