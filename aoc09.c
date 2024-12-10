#include <stdio.h>
#include <stdlib.h>

size_t count_blocks()
{
    FILE* fp = fopen("in09.txt", "r");
    size_t count = 0;
    int c;
    while ((c = fgetc(fp)) != -1)
        count += c - '0';
    fclose(fp);
    return count;
}

void read(int* buf)
{
    FILE* fp = fopen("in09.txt", "r");
    int on = 1;
    int id = 0;
    int c;
    while ((c = fgetc(fp)) != -1)
    {
        int how_many = c - '0';
        while (how_many-- > 0)
            *buf++ = on ? id : -1;
        if (on)
            on = 0;
        else
        {
            on = 1;
            ++id;
        }
    }
    fclose(fp);
}

void first(int* buf, size_t count)
{
    size_t gap = 0;
    while (count > 0)
    {
        --count;
        if (buf[count] != -1)
        {
            while (gap < count)
            {
                if (buf[gap] == -1)
                    break;
                else
                    ++gap;
            }
            if (gap < count)
            {
                buf[gap] = buf[count];
                buf[count] = -1;
            }
            else
                break;
        }
    }
    long i = 0;
    long total = 0;
    while (buf[i] != -1)
    {
        total += i * buf[i];
        i++;
    }
    printf("%ld\n", total);
}

void second(int* buf, size_t count)
{
    size_t count_org = count;
    int id = count;
    while (count > 0)
    {
        --count;
        if (buf[count] != -1 && buf[count] < id)
        {
            id = buf[count];
            int len = 1;
            while (count > 0 && buf[count - 1] == id)
            {
                --count;
                len++;
            }
            size_t gap = 0;
            size_t start_gap;
            while (gap < count)
            {
                if (buf[gap] == -1)
                {
                    start_gap = gap;
                    while (gap - start_gap < len && buf[gap] == -1)
                        ++gap;
                    if (gap - start_gap == len)
                    {
                        for (size_t i = 0; i < len; ++i)
                        {
                            buf[start_gap + i] = id;
                            buf[count + i] = -1;
                        }
                        break;
                    }
                }
                else
                    ++gap;
            }
        }
    }
    long i = 0;
    long total = 0;
    for (size_t i = 0; i <count_org; ++i)
    {
        if (buf[i] != -1)
            total += i * buf[i];
    }
    printf("%lld\n", total);
}

int main()
{
    size_t count = count_blocks();
    int* buf = malloc(count * sizeof(int));
    read(buf);
    second(buf, count);
    free(buf);
    return 0;
}