#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av)
{
    if (ac != 4) return 1;
    int width = atoi(av[1]), height = atoi(av[2]), iterations = atoi(av[3]);
    char *map = malloc(width * height), *next_map = malloc(width * height);

    for (int i = 0; i < width * height; i++)
        map[i] = ' ';

    int cursor_x = 0, cursor_y = 0, draw_mode = 0;
    char input;
    while (read(0, &input, 1) > 0)
    {
        int valid_input = 1;
        if (input == 'w' && cursor_y > 0) cursor_y--;
        else if (input == 'a' && cursor_x > 0) cursor_x--;
        else if (input == 's' && cursor_y < height - 1) cursor_y++;
        else if (input == 'd' && cursor_x < width - 1 ) cursor_x++;
        else if (input == 'x') draw_mode = !draw_mode;
        else    valid_input = 0;

        if (valid_input && draw_mode) map[cursor_y * width + cursor_x] = 'O';
    }
    while (iterations-- > 0)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int neighbors = 0;
                for (int dy = -1; dy <= 1; dy++)
                {
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        if (!dx && !dy) continue;
                        int nx = x + dx, ny = y + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && \
                            ny < height && map[ny * width + nx] == 'O')
                            neighbors++;
                    }
                }
                next_map[y * width + x] = (neighbors == 3 || \
                (neighbors == 2 && map[y * width + x] == 'O')) ? 'O' : ' ';
            }
        }
        char *temp = map;
        map = next_map;
        next_map = temp;
    }
    for (int y = 0; y < height; y++)
    {
        write(1, map + y * width, width);
        write(1, "\n", 1);
    }
    free(map);
    free(next_map);
    return 0;
}