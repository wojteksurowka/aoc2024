#include <stdio.h>
#include <stdlib.h>

int* read_file(const char* name)
{
    FILE* fp = fopen(name, "r");
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);
    int* buf = malloc(size * sizeof(int));
    int* ptr = buf;
    int c;
    while ((c = fgetc(fp)) != -1)
        *ptr++ = c;
    *ptr++ = 0;
    fclose(fp);
    return buf;
}

size_t count_rows(int* contents)
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

size_t get_width(int* contents)
{
    size_t chars = 0;
    int c;
    while ((c = *contents++) != '\n')
        ++chars;
    return chars;
}

void get_rows(int* contents, int** rows, size_t nrows, size_t width)
{
    while (nrows-- > 0)
    {
        *rows++ = contents;
        contents += width + 1;
    }
}

int fill_id(int** rows, size_t nrows, size_t width, int id, int ch, int r, int c)
{
    rows[r][c] = id;
    if (r > 0 && rows[r - 1][c] == ch)
        fill_id(rows, nrows, width, id, ch, r - 1, c);
    if (r < nrows - 1 && rows[r + 1][c] == ch)
        fill_id(rows, nrows, width, id, ch, r + 1, c);
    if (c > 0 && rows[r][c - 1] == ch)
        fill_id(rows, nrows, width, id, ch, r, c - 1);
    if (c < width - 1 && rows[r][c + 1] == ch)
        fill_id(rows, nrows, width, id, ch, r, c + 1);
}

void find(int** rows, size_t nrows, size_t width, int* r, int* c)
{
    *r = *c = -1;
    for (int i = 0; i < nrows; ++i)
        for (int j = 0; j < width; ++j)
            if (rows[i][j] >= 0 && rows[i][j] < 128)
            {
                *r = i;
                *c = j;
                return;
            }
}

int fill(int** rows, size_t nrows, size_t width)
{
    int r = 0;
    int c = 0;
    int id = 128;
    for (; r >= 0 && c >= 0; find(rows, nrows, width, &r, &c))
    {
        fill_id(rows, nrows, width, id++, rows[r][c], r, c);
    }
    return id - 128;
}

void first(int** rows, size_t nrows, size_t width)
{
    int ngroups = fill(rows, nrows, width);
    int total = 0;
    for (int g = 128; g < 128 + ngroups; ++g)
    {
        int area = 0;
        int perimeter = 0;
        for (int r = 0; r < nrows; ++r)
            for (int c = 0; c < width; ++c)
                if (rows[r][c] == g)
                {
                    ++area;
                    if (r == 0 || rows[r - 1][c] != g)
                        ++perimeter;
                    if (r == nrows - 1 || rows[r + 1][c] != g)
                        ++perimeter;
                    if (c == 0 || rows[r][c - 1] != g)
                        ++perimeter;
                    if (c == width - 1 || rows[r][c + 1] != g)
                        ++perimeter;
                }
        total += area * perimeter;
    }
    printf("%d\n", total);
}

int first_perimeter_hor(int** rows, size_t nrows, size_t width, int g, int r, int c, int pr)
{
    if (r < 0 || c - 1 < 0 || rows[r][c - 1] != g)
        return 1;
    return pr >= 0 && pr < nrows && rows[pr][c - 1] == g;
}

int first_perimeter_vert(int** rows, size_t nrows, size_t width, int g, int r, int c, int pc)
{
    if (r - 1 < 0 || c < 0 || rows[r - 1][c] != g)
        return 1;
    return pc >= 0 && pc < width && rows[r - 1][pc] == g;
}

void second(int** rows, size_t nrows, size_t width)
{
    int ngroups = fill(rows, nrows, width);
    int total = 0;
    for (int g = 128; g < 128 + ngroups; ++g)
    {
        int area = 0;
        int perimeter = 0;
        for (int r = 0; r < nrows; ++r)
            for (int c = 0; c < width; ++c)
                if (rows[r][c] == g)
                {
                    ++area;
                    if ((r == 0 || rows[r - 1][c] != g) && first_perimeter_hor(rows, nrows, width, g, r, c, r - 1))
                        ++perimeter;
                    if ((r == nrows - 1 || rows[r + 1][c] != g) && first_perimeter_hor(rows, nrows, width, g, r, c, r + 1))
                        ++perimeter;
                    if ((c == 0 || rows[r][c - 1] != g) && first_perimeter_vert(rows, nrows, width, g, r, c, c - 1))
                        ++perimeter;
                    if ((c == width - 1 || rows[r][c + 1] != g) && first_perimeter_vert(rows, nrows, width, g, r, c, c + 1))
                        ++perimeter;
                }
        total += area * perimeter;
    }
    printf("%d\n", total);
}

int main()
{
    int* contents = read_file("in12.txt");
    size_t nrows = count_rows(contents);
    size_t width = get_width(contents);
    int** rows = malloc(nrows * sizeof(int*));
    get_rows(contents, rows, nrows, width);
    second(rows, nrows, width);
    free(rows);
    free(contents);
    return 0;
}