#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* name)
{
    FILE* fp = fopen(name, "r");
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);
    char* buf = malloc(size + 1);
    char* ptr = buf;
    int c;
    while ((c = fgetc(fp)) != -1)
        *ptr++ = c == '\n' ? c : c - '0';
    *ptr++ = -1;
    fclose(fp);
    return buf;
}

size_t count_rows(char* contents)
{
    size_t rows = 0;
    int new_row = 1;
    int c;
    while ((c = *contents++) != -1)
    {
        if (new_row)
        {
            new_row = 0;
            ++rows;
        }
        if (c == '\n')
            new_row = 1;
    }
    return rows;
}

size_t get_width(char* contents)
{
    size_t chars = 0;
    int c;
    while ((c = *contents++) != '\n')
        ++chars;
    return chars;
}

void get_rows(char* contents, char** rows, size_t nrows, size_t width)
{
    while (nrows-- > 0)
    {
        *rows++ = contents;
        contents += width + 1;
    }
}

void add_found(int r, int c, size_t* count, int found[][2])
{
    for (size_t i = 0; i < *count; ++i)
        if (found[i][0] == r && found[i][1] == c)
            return;
    found[*count][0] = r;
    found[*count][1] = c;
    ++*count;
}

void find(int level, int r, int c, char** rows, size_t nrows, size_t width, size_t* count, int found[][2])
{
    if (r > 0 && rows[r - 1][c] == level)
    {
        if (level == 9)
            add_found(r - 1, c, count, found);
        else
            find(level + 1, r - 1, c, rows, nrows, width, count, found);
    }
    if (c > 0 && rows[r][c - 1] == level)
    {
        if (level == 9)
            add_found(r, c - 1, count, found);
        else
            find(level + 1, r, c - 1, rows, nrows, width, count, found);
    }
    if (r + 1 < nrows && rows[r + 1][c] == level)
    {
        if (level == 9)
            add_found(r + 1, c, count, found);
        else
            find(level + 1, r + 1, c, rows, nrows, width, count, found);
    }
    if (c + 1 < width && rows[r][c + 1] == level)
    {
        if (level == 9)
            add_found(r, c + 1, count, found);
        else
            find(level + 1, r, c + 1, rows, nrows, width, count, found);
    }
}

void first(char** rows, size_t nrows, size_t width)
{
    size_t total = 0;
    for (size_t r = 0; r < nrows; ++r)
    {
        for (size_t c = 0; c < width; ++c)
        {
            if (rows[r][c] == 0)
            {
                size_t count = 0;
                int found[1024][2];
                find(1, r, c, rows, nrows, width, &count, found);
                total += count;
            }
        }
    }
    printf("%lu", total);
}

size_t find2nd(int level, int r, int c, char** rows, size_t nrows, size_t width)
{
    /*printf("%d %d %d %c %c %c %c\n", level, r, c,
        r > 0 ? rows[r - 1][c] + '0' : 'X',
        c > 0 ? rows[r][c - 1] + '0' : 'X',
        r + 1 < nrows ? rows[r + 1][c] + '0' : 'X',
        c + 1 < width ? rows[r][c + 1] + '0' : 'X');*/
    size_t result = 0;
    if (r > 0 && rows[r - 1][c] == level)
        result += level == 9 ? 1 : find2nd(level + 1, r - 1, c, rows, nrows, width);
    if (c > 0 && rows[r][c - 1] == level)
        result += level == 9 ? 1 : find2nd(level + 1, r, c - 1, rows, nrows, width);
    if (r + 1 < nrows && rows[r + 1][c] == level)
        result += level == 9 ? 1 : find2nd(level + 1, r + 1, c, rows, nrows, width);
    if (c + 1 < width && rows[r][c + 1] == level)
        result += level == 9 ? 1 : find2nd(level + 1, r, c + 1, rows, nrows, width);
    return result;
}

void second(char** rows, size_t nrows, size_t width)
{
    size_t total = 0;
    for (size_t r = 0; r < nrows; ++r)
    {
        for (size_t c = 0; c < width; ++c)
        {
            if (rows[r][c] == 0)
            {
                total += find2nd(1, r, c, rows, nrows, width);
            }
        }
    }
    printf("%lu", total);
}

int main()
{
    char* contents = read_file("in10.txt");
    size_t nrows = count_rows(contents);
    size_t width = get_width(contents);
    char** rows = malloc(nrows * sizeof(char*));
    get_rows(contents, rows, nrows, width);
    second(rows, nrows, width);
    free(rows);
    free(contents);
    return 0;
}