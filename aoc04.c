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

size_t find(char** rows, size_t nrows, size_t width, size_t r, size_t c)
{
    size_t found = 0;
    if (rows[r][c] == 'X')
    {
        if (r >= 3 && rows[r - 1][c] == 'M' && rows[r - 2][c] == 'A' && rows[r - 3][c] == 'S')
            ++found;
        if (r >= 3 && c >= 3 && rows[r - 1][c - 1] == 'M' && rows[r - 2][c - 2] == 'A' && rows[r - 3][c - 3] == 'S')
            ++found;
        if (c >= 3 && rows[r][c - 1] == 'M' && rows[r][c - 2] == 'A' && rows[r][c - 3] == 'S')
            ++found;
        if (r + 3 < nrows && c >= 3 && rows[r + 1][c - 1] == 'M' && rows[r + 2][c - 2] == 'A' && rows[r + 3][c - 3] == 'S')
            ++found;
        if (r + 3 < nrows && rows[r + 1][c] == 'M' && rows[r + 2][c] == 'A' && rows[r + 3][c] == 'S')
            ++found;
        if (r + 3 < nrows && c + 3 < width && rows[r + 1][c + 1] == 'M' && rows[r + 2][c + 2] == 'A' && rows[r + 3][c + 3] == 'S')
            ++found;
        if (c + 3 < width && rows[r][c + 1] == 'M' && rows[r][c + 2] == 'A' && rows[r][c + 3] == 'S')
            ++found;
        if (r >= 3 && c + 3 < width && rows[r - 1][c + 1] == 'M' && rows[r - 2][c + 2] == 'A' && rows[r - 3][c + 3] == 'S')
            ++found;
    }
    return found;
}

void first(char** rows, size_t nrows, size_t width)
{
    size_t total = 0;
    for (size_t r = 0; r < nrows; ++r)
        for (size_t c = 0; c < width; ++c)
            total += find(rows, nrows, width, r, c);
    printf("%d\n", (int)total);
}

size_t find2nd(char** rows, size_t nrows, size_t width, size_t r, size_t c)
{
    if (rows[r][c] == 'A')
    {
        char* patterns[] = {"MMSS", "SSMM", "MSMS", "SMSM"};
        for (size_t i = 0; i < 4; ++i)
            if (rows[r - 1][c - 1] == patterns[i][0] && rows[r - 1][c + 1] == patterns[i][1] &&
                rows[r + 1][c - 1] == patterns[i][2] && rows[r + 1][c + 1] == patterns[i][3])
            {
                return 1;
            }
    }
    return 0;
}

void second(char** rows, size_t nrows, size_t width)
{
    size_t total = 0;
    for (size_t r = 1; r < nrows - 1; ++r)
        for (size_t c = 1; c < width - 1; ++c)
            total += find2nd(rows, nrows, width, r, c);
    printf("%d\n", (int)total);
}

int main()
{
    char* contents = read_file("in04.txt");
    size_t nrows = count_rows(contents);
    size_t width = get_width(contents);
    char** rows = malloc(nrows * sizeof(char*));
    get_rows(contents, rows, nrows, width);
    second(rows, nrows, width);
    free(rows);
    free(contents);
    return 0;
}