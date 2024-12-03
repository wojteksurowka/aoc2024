#include <stdio.h>
#include <stdlib.h>

size_t count_lines()
{
    FILE* fp = fopen("in01.txt", "r");
    size_t count = 0;
    int f, s;
    while (fscanf(fp, "%d %d", &f, &s) == 2)
        ++count;
    fclose(fp);
    return count;
}

void read(int* buf1, int* buf2, size_t count)
{
    FILE* fp = fopen("in01.txt", "r");
    while (count--)
        fscanf(fp, "%d %d", buf1++, buf2++);
    fclose(fp);
}

void sort(int* buf, size_t count)
{
    int any_swaps;
    do
    {
        any_swaps = 0;
        int* ptr = buf;
        int* end = buf + count - 1; 
        while (ptr < end)
        {
            if (ptr[0] > ptr[1])
            {
                int tmp = ptr[0];
                ptr[0] = ptr[1];
                ptr[1] = tmp;
                any_swaps = 1;
            }
            ++ptr;
        }
    } while (any_swaps);
}

void first(int* buf1, int* buf2, size_t count)
{
    sort(buf1, count);
    sort(buf2, count);
    int total = 0;
    for (size_t i = 0; i < count; ++i)
    {
        total += buf1[i] < buf2[i] ? buf2[i] - buf1[i] : buf1[i] - buf2[i];
    }
    printf("%d", total);
}

void second(int* buf1, int* buf2, size_t count)
{
    const size_t FIVEDIG = 100000;
    int* counts_2 = malloc(FIVEDIG * sizeof(int));
    for (size_t i = 0; i < FIVEDIG; ++i)
        counts_2[i] = 0;
    for (size_t i = 0; i < count; ++i)
        ++counts_2[buf2[i]];
    int total = 0;
    for (size_t i = 0; i < count; ++i)
        total += buf1[i] * counts_2[buf1[i]];
    printf("%d", total);
    free(counts_2);
}

int main()
{
    size_t count = count_lines();
    int* buf1 = malloc(count * sizeof(int));
    int* buf2 = malloc(count * sizeof(int));
    read(buf1, buf2, count);
    second(buf1, buf2, count);
    free(buf1);
    free(buf2);
    return 0;
}