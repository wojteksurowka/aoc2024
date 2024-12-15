#include <stdio.h>
#include <stdlib.h>

size_t count_lines()
{
    FILE* fp = fopen("in14.txt", "r");
    size_t count = 0;
    int px, py, vx, vy;
//    while (fscanf(fp, "p=%d,", &px) == 1)
    while (fscanf(fp, "p=%d,%d v=%d,%d", &px, &py, &vx, &vy) == 4)
    {
        ++count;
        fgetc(fp);
    }
    fclose(fp);
    return count;
}

void read(int (*buf)[4])
{
    FILE* fp = fopen("in14.txt", "r");
    while (fscanf(fp, "p=%d, %d v=%d,%d", &buf[0][0], &buf[0][1], &buf[0][2], &buf[0][3]) == 4)
    {
        ++buf;
        fgetc(fp);
    }
    fclose(fp);
}

void move(int buf[4], size_t steps, size_t width, size_t height)
{
    buf[0] += buf[2] * steps;
    buf[1] += buf[3] * steps;
    if (buf[0] < 0)
    {
        buf[0] = ((-buf[0] / width) + 1) * width + buf[0];
        if (buf[0] == width)
            buf[0] = 0;
    }
    else
        buf[0] %= width;
    if (buf[1] < 0)
    {
        buf[1] = ((-buf[1] / height) + 1) * height + buf[1];
        if (buf[1] == height)
            buf[1] = 0;
    }
    else
        buf[1] %= height;
}

void first(int (*buf)[4], size_t count)
{
    size_t width = 101;
    size_t height = 103;
    int quadrants[4] = {0, 0, 0, 0};
    for (size_t i = 0; i < count; ++i)
    {
        move(buf[i], 100, width, height);
        if (buf[i][0] < width / 2 && buf[i][1] < height / 2)
            ++quadrants[0];
        else if (buf[i][0] > width / 2 && buf[i][1] < height / 2)
            ++quadrants[1];
        else if (buf[i][0] < width / 2 && buf[i][1] > height / 2)
            ++quadrants[2];
        else if (buf[i][0] > width / 2 && buf[i][1] > height / 2)
            ++quadrants[3];
    }
    printf("%d\n", quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]);
}

void second(int (*buf)[4], size_t count)
{
    for (size_t steps = 1; steps < 10000; ++steps)
    {
        read(buf);
        size_t width = 101;
        size_t height = 103;
        size_t hor[height];
        for (size_t i = 0; i < height; ++i)
            hor[i] = 0;
        size_t max_hor = 0;
        size_t ver[width];
        for (size_t i = 0; i < width; ++i)
            ver[i] = 0;
        size_t max_ver = 0;
        for (size_t i = 0; i < count; ++i)
        {
            move(buf[i], steps, width, height);
            ++hor[buf[i][1]];
            if (hor[buf[i][1]] > max_hor)
                max_hor = hor[buf[i][1]];
            ++ver[buf[i][0]];
            if (ver[buf[i][0]] > max_ver)
                max_ver = ver[buf[i][0]];
        }
        if (max_hor * max_ver > 500)
            printf("%lu %lu\n", steps, max_hor * max_ver);
        if (max_hor * max_ver == 1122)
        {
            char picture[height][width];
            for (size_t y = 0; y < height; ++y)
                for (size_t x = 0; x < width; ++x)
                    picture[y][x] = ' ';
            printf("%lu\n", steps);
            for (size_t i = 0; i < count; ++i)
                picture[buf[i][1]][buf[i][0]] = 'X';
            for (size_t y = 0; y < height; ++y)
            {
                for (size_t x = 0; x < width; ++x)
                    printf("%c", picture[y][x]);
                printf("\n");
            }
        }
    }
}

int main()
{
    size_t count = count_lines();
    int (*buf)[4] = malloc(count * sizeof(int) * 4);
    second(buf, count);
    return 0;
}