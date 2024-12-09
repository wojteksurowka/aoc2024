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

void get_counts_coords(size_t counts[], size_t coords[][4][2], char** rows, size_t nrows, size_t width)
{
    for (size_t r = 0; r < nrows; ++r)
        for (size_t c = 0; c < width; ++c)
            if (rows[r][c] != '.')
            {
                int ch = rows[r][c];
                coords[ch][counts[ch]][0] = r;
                coords[ch][counts[ch]][1] = c;
                ++counts[ch];
            }
}

int left(size_t nrows, size_t width, int r1, int c1, int r2, int c2, int* rl, int* cl)
{
    *rl = r1 <= r2 ? r1 - (r2 - r1) : r1 + (r1 - r2);
    *cl = c1 <= c2 ? c1 - (c2 - c1) : c1 + (c1 - c2);
    return *rl >= 0 && *rl < nrows && *cl >= 0 && *cl < width;
}

int right(size_t nrows, size_t width, int r1, int c1, int r2, int c2, int* rr, int* cr)
{
    *rr = r1 <= r2 ? r2 + (r2 - r1) : r2 - (r1 - r2);
    *cr = c1 <= c2 ? c2 + (c2 - c1) : c2 - (c1 - c2);
    return *rr >= 0 && *rr < nrows && *cr >= 0 && *cr < width;
}

void first(size_t counts[], size_t coords[][4][2], char** rows, size_t nrows, size_t width)
{
    size_t* result_contents = malloc(sizeof(size_t) * nrows * width);
    for (size_t i = 0; i < nrows * width; ++i)
        result_contents[i] = 0;
    size_t** result_rows = malloc(nrows * sizeof(size_t*));
    for (size_t i = 0; i < nrows; ++i)
        result_rows[i] = result_contents + i * width;
    for (size_t ch =-0; ch < 256; ++ch)
    {
        if (counts[ch] > 1)
        {
            for (size_t i = 0; i + 1 < counts[ch]; ++i)
            {
                for (size_t j = i + 1; j < counts[ch]; ++j)
                {
                    int r1 = coords[ch][i][0];
                    int c1 = coords[ch][i][1];
                    int r2 = coords[ch][j][0];
                    int c2 = coords[ch][j][1];
                    int r, c;
                    if (left(nrows, width, r1, c1, r2, c2, &r, &c))
                        ++result_rows[r][c];
                    if (right(nrows, width, r1, c1, r2, c2, &r, &c))
                        ++result_rows[r][c];
                }
            }
        }
    }
    size_t total = 0;
    for (size_t r = 0; r < nrows; ++r)
        for (size_t c = 0; c < width; ++c)
            if (result_rows[r][c])
                ++total;
    printf("%lu\n", total);
    free(result_rows);
    free(result_contents);
}

void second(size_t counts[], size_t coords[][4][2], char** rows, size_t nrows, size_t width)
{
    size_t* result_contents = malloc(sizeof(size_t) * nrows * width);
    for (size_t i = 0; i < nrows * width; ++i)
        result_contents[i] = 0;
    size_t** result_rows = malloc(nrows * sizeof(size_t*));
    for (size_t i = 0; i < nrows; ++i)
        result_rows[i] = result_contents + i * width;
    for (size_t ch =-0; ch < 256; ++ch)
    {
        if (counts[ch] > 1)
        {
            for (size_t i = 0; i + 1 < counts[ch]; ++i)
            {
                for (size_t j = i + 1; j < counts[ch]; ++j)
                {
                    int r1 = coords[ch][i][0];
                    int c1 = coords[ch][i][1];
                    int r2 = coords[ch][j][0];
                    int c2 = coords[ch][j][1];
                    ++result_rows[r1][c1];
                    ++result_rows[r2][c2];
                    int r, c;
                    while (left(nrows, width, r1, c1, r2, c2, &r, &c))
                    {
                        ++result_rows[r][c];
                        r2 = r1;
                        c2 = c1;
                        r1 = r;
                        c1 = c;
                    }
                    r1 = coords[ch][i][0];
                    c1 = coords[ch][i][1];
                    r2 = coords[ch][j][0];
                    c2 = coords[ch][j][1];
                    while (right(nrows, width, r1, c1, r2, c2, &r, &c))
                    {
                        ++result_rows[r][c];
                        r1 = r2;
                        c1 = c2;
                        r2 = r;
                        c2 = c;
                    }
                }
            }
        }
    }
    size_t total = 0;
    for (size_t r = 0; r < nrows; ++r)
        for (size_t c = 0; c < width; ++c)
            if (result_rows[r][c])
                ++total;
    printf("%lu\n", total);
    free(result_rows);
    free(result_contents);
}

int main()
{
    char* contents = read_file("in08.txt");
    size_t nrows = count_rows(contents);
    size_t width = get_width(contents);
    char** rows = malloc(nrows * sizeof(char*));
    get_rows(contents, rows, nrows, width);
    size_t counts[256] = { 0 };
    size_t coords[256][4][2];
    get_counts_coords(counts, coords, rows, nrows, width);
    second(counts, coords, rows, nrows, width);
    free(rows);
    free(contents);
    return 0;
}