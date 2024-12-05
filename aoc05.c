#include <stdio.h>
#include <stdlib.h>

void read_pairs(FILE* fp, char pairs[][100])
{
    for (size_t i = 0; i < 100; ++i)
        for (size_t j = 0; j < 100; ++j)
            pairs[i][j] = 0;
    int l, r;
    while (fscanf(fp, "%d|%d", &l, &r) == 2)
    {
        pairs[l][r] = 1;
        int c = fgetc(fp);
        if (c != '\n')
            break;
        c = fgetc(fp);
        if (c == '\n')
            break;
        else
            ungetc(c, fp);
    }
}

size_t read_sequence(FILE *fp, int* buf)
{
    int n;
    size_t count = 0;
    while (fscanf(fp, "%d", &n) == 1)
    {
        *buf++ = n;
        ++count;
        int c = fgetc(fp);
        if (c == '\n')
            break;
    }
    return count;
}

int ordered(char pairs[][100], int seq[], size_t count)
{
    for (size_t i = 0; i < count - 1; ++i)
    {
        for (size_t j = i + 1; j < count; ++j)
        {
            if (!pairs[seq[i]][seq[j]])
                return 0;
        }
    }
    return 1;
}

void first()
{
    FILE* fp = fopen("in05.txt", "r");
    char pairs[100][100];
    read_pairs(fp, pairs);
    int seq[100];
    size_t count;
    int total = 0;
    while ((count = read_sequence(fp, seq)) > 0)
    {
        if (ordered(pairs, seq, count))
            total += seq[count / 2];
    }
    printf("%d\n", total);
    fclose(fp);
}

void sort(char pairs[][100], int* buf, size_t count)
{
    int any_swaps;
    do
    {
        any_swaps = 0;
        int* ptr = buf;
        int* end = buf + count - 1; 
        while (ptr < end)
        {
            if (!pairs[ptr[0]][ptr[1]])
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

void second()
{
    FILE* fp = fopen("in05.txt", "r");
    char pairs[100][100];
    read_pairs(fp, pairs);
    int seq[100];
    size_t count;
    int total = 0;
    while ((count = read_sequence(fp, seq)) > 0)
    {
        if (!ordered(pairs, seq, count))
        {
            sort(pairs, seq, count);
            total += seq[count / 2];
        }
    }
    printf("%d\n", total);
    fclose(fp);
}

int main()
{
    second();
    return 0;
}