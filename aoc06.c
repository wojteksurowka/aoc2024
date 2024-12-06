#include <stdio.h>
#include <stdlib.h>

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

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
        *ptr++ = c;
    *ptr++ = 0;
    fclose(fp);
    return buf;
}

size_t count_rows(char* contents)
{
    size_t rows = 0;
    int new_row = 1;
    int c;
    while ((c = *contents++) != 0)
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

void find_start(char** rows, size_t nrows, size_t width, int* r, int* c)
{
    for (*r = 0; *r < nrows; ++*r)
        for (*c = 0; *c < width; ++*c)
            if (rows[*r][*c] == '^')
                return;
}

int next(char** rows, size_t nrows, size_t width, int* dir, int* r, int* c)
{
    int tr = *r;
    int tc = *c;
    if (*dir == UP)
        --tr;
    else if (*dir == RIGHT)
        ++tc;
    else if (*dir == DOWN)
        ++tr;
    else if (*dir == LEFT)
        --tc;
    if (tr < 0 || tr >= nrows || tc < 0 || tc >= width)
        return 0;
    if (rows[tr][tc] == '#')
    {
        *dir = *dir == LEFT ? UP : *dir + 1;
        return next(rows, nrows, width, dir, r, c);
    }
    else
    {
        *r = tr;
        *c = tc;
        return 1;
    }
}

void first(char** rows, size_t nrows, size_t width)
{
    int dir = UP;
    int r, c;
    find_start(rows, nrows, width, &r, &c);
    rows[r][c] = 'X';
    while (next(rows, nrows, width, &dir, &r, &c))
        rows[r][c] = 'X';
    int total = 0;
    for (r = 0; r < nrows; ++r)
        for (c = 0; c < width; ++c)
            if (rows[r][c] == 'X')
                ++total;
    printf("%d\n", total);
}

int is_loop(char** rows, size_t nrows, size_t width, int r, int c)
{
    int dir = UP;
    rows[r][c] = 1;
    int loop = 0;
    while (next(rows, nrows, width, &dir, &r, &c))
    {
        if (rows[r][c] == 4)
        {
            loop = 1;
            break;
        }
        else if (rows[r][c] == '.')
            rows[r][c] = 1;
        else
            ++rows[r][c];
    }
    for (r = 0; r < nrows; ++r)
        for (c = 0; c < width; ++c)
            if (rows[r][c] < 5)
                rows[r][c] = '.';
    return loop;
}

void second(char** rows, size_t nrows, size_t width)
{
    int sr, sc;
    find_start(rows, nrows, width, &sr, &sc);
    int total = 0;
    for (int r = 0; r < nrows; ++r)
        for (int c = 0; c < width; ++c)
            if ((r != sr || c != sc) && rows[r][c] == '.')
            {
                rows[r][c] = '#';
                total += is_loop(rows, nrows, width, sr, sc);
                rows[r][c] = '.';
            }
    printf("%d\n", total);
}

int main()
{
    char* contents = read_file("in06.txt");
     size_t nrows = count_rows(contents);
    size_t width = get_width(contents);
    char** rows = malloc(nrows * sizeof(char*));
    get_rows(contents, rows, nrows, width);
    second(rows, nrows, width);
    free(contents);
    return 0;
}