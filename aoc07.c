#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

size_t read_longs(FILE* fp, long* buf)
{
    size_t count = 0;
    int inside = 0;
    *buf = 0;
    for (;;)
    {
        int c = fgetc(fp);
        if (c == '\n' || c == -1)
            return count;
        else if (c >= '0' && c <= '9')
        {
            if (!inside)
            {
                inside = 1;
                ++count;
            }
            *buf = *buf * 10 + c - '0';
        }
        else
        {
            inside = 0;
            *++buf = 0;
        }
    }
}

long concat(long l, long r)
{
    long factor = 1;
    long result = 0;
    while (r > 0)
    {
        result += (r % 10) * factor;
        r /= 10;
        factor *= 10;
    }
    while (l > 0)
    {
        result += (l % 10) * factor;
        l /= 10;
        factor *= 10;
    }
    return result;
}

int find(int use_concat, long result, long partial, long* rest, size_t count)
{
    if (count == 1)
    {
        if (partial * *rest == result)
            return 1;
        else if (partial + *rest == result)
            return 1;
        else if (use_concat && concat(partial, *rest) == result)
            return 1;
        else
            return 0;
    }
    else
    {
        if (find(use_concat, result, partial * *rest, rest + 1, count - 1))
            return 1;
        else if (find(use_concat, result, partial + *rest, rest + 1, count - 1))
            return 1;
        else if (use_concat && find(use_concat, result, concat(partial, *rest), rest + 1, count - 1))
            return 1;
        else
            return 0;
    }
}

void solve(int use_concat)
{
    FILE* fp = fopen("in07.txt", "r");
    long result;
    long total = 0;
    while (fscanf(fp, "%ld: ", &result) == 1)
    {
        long buf[20];
        size_t count = read_longs(fp, buf);
        if (find(use_concat, result, buf[0], buf + 1, count - 1))
            total += result;
    }
    printf("%ld\n", total);
    fclose(fp);
}

void first()
{
    solve(0);
}

void second()
{
    solve(1);
}

int main()
{
    second();
    return 0;
}