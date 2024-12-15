#include <stdio.h>
#include <stdlib.h>

size_t count_lines()
{
    FILE* fp = fopen("in13.txt", "r");
    size_t count = 0;
    int ax, ay, bx, by, px, py;
    while (fscanf(fp, "Button A: X+%d, Y+%d\nButton B: X+%d, Y+%d\nPrize: X=%d, Y=%d", &ax, &ay, &bx, &by, &px, &py) == 6)
    {
        ++count;
        fgetc(fp);
        fgetc(fp);
    }
    fclose(fp);
    return count;
}

void read(long long (*buf)[6])
{
    FILE* fp = fopen("in13.txt", "r");
    while (fscanf(fp, "Button A: X+%ld, Y+%ld\nButton B: X+%ld, Y+%ld\nPrize: X=%ld, Y=%lld", &buf[0][0], &buf[0][1], &buf[0][2], &buf[0][3], &buf[0][4], &buf[0][5]) == 6)
    {
        ++buf;
        fgetc(fp);
        fgetc(fp);
    }
    fclose(fp);
}

void first(long long (*buf)[6], size_t count)
{
    long long total = 0;
    for (size_t i = 0; i < count; ++i)
    {
        long long w = buf[i][0] * buf[i][3] - buf[i][2] * buf[i][1];
        long long wa = buf[i][4] * buf[i][3] - buf[i][2] * buf[i][5];
        long long wb = buf[i][0] * buf[i][5] - buf[i][4] * buf[i][1];
        if (wa % w == 0 && wb % w == 0)
        {
            long long a = wa / w;
            long long b = wb / w;
            total += a * 3 + b;
        }
    }
    printf("%lld", total);
}

void second(long long (*buf)[6], size_t count)
{
    long long total = 0;
    for (size_t i = 0; i < count; ++i)
    {
        long long w = buf[i][0] * buf[i][3] - buf[i][2] * buf[i][1];
        long long wa = (buf[i][4] + 10000000000000L) * buf[i][3] - buf[i][2] * (buf[i][5] + 10000000000000L);
        long long wb = buf[i][0] * (buf[i][5] + 10000000000000L) - (buf[i][4] + 10000000000000L) * buf[i][1];
        if (wa % w == 0 && wb % w == 0)
        {
            long long a = wa / w;
            long long b = wb / w;
            total += a * 3 + b;
        }
    }
    printf("%lld", total);
}

int main()
{
    size_t count = count_lines();
    long long (*buf)[6] = malloc(count * sizeof(long long) * 6);
    read(buf);
    second(buf, count);
    return 0;
}