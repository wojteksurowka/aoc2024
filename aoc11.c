#include <stdio.h>
#include <stdlib.h>

long* read_file(const char* name, size_t* count)
{
    FILE* fp = fopen(name, "r");
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);
    long* buf = malloc(size * sizeof(long));
    long* ptr = buf;
    *count = 0;
    while (fscanf(fp, "%ld", ptr++) == 1)
        ++*count;
    fclose(fp);
    return buf;
}

int split(long* l, long* r, long x)
{
    int buf[64];
    size_t count = 0;
    while (x > 0)
    {
        buf[count++] = x % 10;
        x /= 10;
    }
    if (count % 2 == 0)
    {
        *l = *r = 0;
        for (size_t i = count - 1; i >= count / 2; --i)
            *l = *l * 10 + buf[i];
        for (size_t i = count / 2; i > 0; --i)
            *r = *r * 10 + buf[i - 1];
        return 1;
    }
    else
        return 0;
}

long step(long n, size_t level, long memo[][10000])
{
    if (level == 0)
        return 1;
    else
    {
        if (n < 10000 && memo[level][n])
            return memo[level][n];
        long l, r;
        long result;
        if (n == 0)
            result = step(1, level - 1, memo);
        else if (split(&l, &r, n))
            result = step(l, level - 1, memo) + step(r, level - 1, memo);
        else
            result = step(n * 2024, level - 1, memo);
        if (n < 10000)
            memo[level][n] = result;
        return result;
    }
}

void solve(long* buf, size_t count, size_t level)
{
    long total = 0;
    long memo[76][10000] = { 0 };
    for (size_t i = 0; i < count; ++i)
    {
        printf("%ld\n", buf[i]);
        total += step(buf[i], level, memo);
    }
    printf("%ld\n", total);
}

void first(long* buf, size_t count)
{
    solve(buf, count, 25);
}

void second(long* buf, size_t count)
{
    solve(buf, count, 75);
}

int main()
{
    size_t count;
    long* buf = read_file("in11.txt", &count);
    second(buf, count);
    free(buf);
    return 0;
}
