#include <stdio.h>
#include <stdlib.h>

size_t read_ints(FILE* fp, int* buf)
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

int valid(int value, int next, int decreasing)
{
    if ((value > next) != decreasing)
        return 0;
    int diff = decreasing ? value - next : next - value;
    return diff >= 1 && diff <= 3;
}

int safe(int* buf, size_t count)
{
    int decreasing = buf[0] > buf[1];
    for (size_t i = 0; i < count - 1; ++i)
    {
        if (!valid(buf[i], buf[i + 1], decreasing))
            return 0;
    }
    return 1;
}

int safe_2nd(int* buf, size_t count)
{
    int tmp[10];
    for (size_t s = 0; s < count; ++s)
    {
        for (size_t i = 0; i < count - 1; ++i)
        {
            tmp[i] = buf[i >= s ? i + 1 : i];
        }
        if (safe(tmp, count - 1))
            return 1;
    }
    return 0;
}

void first()
{
    FILE* fp = fopen("in02.txt", "r");
    int buf[10];
    size_t count;
    size_t total = 0;
    while ((count = read_ints(fp, buf)) > 0)
    {
        if (safe(buf, count))
            ++total;
    }
    printf("%d\n", total);
    fclose(fp);
}

void second()
{
    FILE* fp = fopen("in02.txt", "r");
    int buf[10];
    size_t count;
    size_t total = 0;
    while ((count = read_ints(fp, buf)) > 0)
    {
        if (safe_2nd(buf, count))
            ++total;
    }
    printf("%d\n", total);
    fclose(fp);
}

int main()
{
    second();
    return 0;
}