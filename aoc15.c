#include <stdio.h>
#include <stdlib.h>

size_t count_lines()
{
    FILE* fp = fopen("in15.txt", "r");
    size_t count = 0;
    int c;
    while ((c = fgetc(fp)) != -1)
    {
        if (c == '\n')
        {
            ++count;
            if (fgetc(fp) == '\n')
                break;
        }
    }
    fclose(fp);
    return count;
}

char* read_file()
{
    FILE* fp = fopen("in15.txt", "r");
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

size_t get_width(char* contents)
{
    size_t chars = 0;
    int c;
    while ((c = *contents++) != '\n')
        ++chars;
    return chars;
}

char* find_ops(char* contents)
{
    for (;; ++contents)
    {
        if (contents[0] == '\n' && contents[1] == '\n')
            return contents + 2;
    }
    return 0;
}

void find_at(char** rows, size_t nrows, size_t width, int* r, int* c)
{
    for (*r = 0; *r < nrows; ++(*r))
        for (*c = 0; *c < width; ++(*c))
            if (rows[*r][*c] == '@')
                return;
}

void get_rows(char* contents, char** rows, size_t nrows, size_t width)
{
    while (nrows-- > 0)
    {
        *rows++ = contents;
        contents += width + 1;
    }
}

int can_move_block_vertically(char** rows, int r, int c, int offsetR)
{
    if (rows[r][c] == ']')
        return can_move_block_vertically(rows, r, c - 1, offsetR);
    if (rows[r + offsetR][c] == '.' && rows[r + offsetR][c + 1] == '.')
        return 1;
    else if (rows[r + offsetR][c] == '.' && rows[r + offsetR][c + 1] == '[')
        return can_move_block_vertically(rows, r + offsetR, c + 1, offsetR);
    else if (rows[r + offsetR][c] == '[')
        return can_move_block_vertically(rows, r + offsetR, c, offsetR);
    else if (rows[r + offsetR][c] == ']')
    {
        if (can_move_block_vertically(rows, r + offsetR, c - 1, offsetR))
        {
            if (rows[r + offsetR][c + 1] == '.')
                return 1;
            else if (rows[r + offsetR][c + 1] == '[')
                return can_move_block_vertically(rows, r + offsetR, c + 1, offsetR);
        }
    }
    return 0;
}

void move_block_vertically(char** rows, int r, int c, int offsetR)
{
    if (rows[r][c] == ']')
    {
        move_block_vertically(rows, r, c - 1, offsetR);
        return;
    }
    if (rows[r + offsetR][c] == '[')
        move_block_vertically(rows, r + offsetR, c, offsetR);
    else if (rows[r + offsetR][c] == ']')
    {
        move_block_vertically(rows, r + offsetR, c - 1, offsetR);
        if (rows[r + offsetR][c + 1] == '[')
            move_block_vertically(rows, r + offsetR, c + 1, offsetR);
    }
    else if (rows[r + offsetR][c] == '.' && rows[r + offsetR][c + 1] == '[')
        move_block_vertically(rows, r + offsetR, c + 1, offsetR);
    rows[r + offsetR][c] = '[';
    rows[r + offsetR][c + 1] = ']';
    rows[r][c] = '.';
    rows[r][c + 1] = '.';
}

void move(char** rows, char op, int *r, int *c)
{
    int offsetR = 0;
    int offsetC = 0;
    if (op == '^')
        offsetR = -1;
    else if (op == '>')
        offsetC = 1;
    else if (op == 'v')
        offsetR = 1;
    else if (op == '<')
        offsetC = -1;
    else
        return;
    if (rows[*r + offsetR][*c + offsetC] == '.')
    {
        rows[*r][*c] = '.';
        *r = *r + offsetR;
        *c = *c + offsetC;
        rows[*r][*c] = '@';
    }
    else if (rows[*r + offsetR][*c + offsetC] == 'O')
    {
        int or = *r + offsetR;
        int oc = *c + offsetC;
        while (rows[or][oc] == 'O')
        {
            or += offsetR;
            oc += offsetC;
        }
        if (rows[or][oc] == '.')
        {
            rows[or][oc] = 'O';
            rows[*r][*c] = '.';
            *r = *r + offsetR;
            *c = *c + offsetC;
            rows[*r][*c] = '@';
        }
    }
    else if ((rows[*r + offsetR][*c + offsetC] == '[' || rows[*r + offsetR][*c + offsetC] == ']'))
    {
        if (offsetC)
        {
            int oc = *c + offsetC;
            while (rows[*r][oc] == '[' || rows[*r][oc] == ']')
            {
                oc += offsetC;
            }
            if (rows[*r][oc] == '.')
            {
                while (oc != *c)
                {
                    rows[*r][oc] = rows[*r][oc - offsetC];
                    oc -= offsetC;
                }
                rows[*r][*c] = '.';
                *r = *r + offsetR;
                *c = *c + offsetC;
                rows[*r][*c] = '@';
            }
        }
        else
        {
            if (can_move_block_vertically(rows, *r + offsetR, *c, offsetR))
            {
                move_block_vertically(rows, *r + offsetR, *c, offsetR);
                rows[*r][*c] = '.';
                *r = *r + offsetR;
                *c = *c + offsetC;
                rows[*r][*c] = '@';
            }
        }
    }
}

void first(char** rows, size_t nrows, size_t width, char* ops)
{
    int r, c;
    find_at(rows, nrows, width, &r, &c);
    while (*ops)
        move(rows, *ops++, &r, &c);
    size_t total = 0;
    for (size_t i = 0; i < nrows; ++i)
        for (size_t j = 0; j < width; ++j)
            if (rows[i][j] == 'O')
                total += i * 100 + j;
    printf("%lu\n", total);
}

char* widen(char** rows, size_t nrows, size_t width)
{
    char* contents = malloc((nrows + 1) * width * 2 + 1);
    char* ptr = contents;
    for (size_t r = 0; r < nrows; ++r)
    {
        for (size_t c = 0; c < width; ++c)
        {
            if (rows[r][c] == '#')
            {
                *ptr++ = '#';
                *ptr++ = '#';
            }
            else if (rows[r][c] == 'O')
            {
                *ptr++ = '[';
                *ptr++ = ']';
            }
            else if (rows[r][c] == '.')
            {
                *ptr++ = '.';
                *ptr++ = '.';
            }
            else if (rows[r][c] == '@')
            {
                *ptr++ = '@';
                *ptr++ = '.';
            }
        }
        *ptr++ = '\n';
    }
    *ptr++ = 0;
    return contents;
}

void second(char** rows, size_t nrows, size_t width, char* ops)
{
    char* contents = widen(rows, nrows, width);
    char** new_rows = malloc(nrows * sizeof(char*));
    get_rows(contents, new_rows, nrows, 2 * width);
    int r, c;
    find_at(new_rows, nrows, 2 * width, &r, &c);
    int n = 0;
    while (*ops)
        move(new_rows, *ops++, &r, &c);
    size_t total = 0;
    for (size_t i = 0; i < nrows; ++i)
        for (size_t j = 0; j < 2 * width; ++j)
            if (new_rows[i][j] == '[')
                total += i * 100 + j;
    printf("%lu\n", total);
    free(new_rows);
    free(contents);
}

int main()
{
    char* contents = read_file();
    size_t nrows = count_lines();
    size_t width = get_width(contents);
    char** rows = malloc(nrows * sizeof(char*));
    get_rows(contents, rows, nrows, width);
    char* ops = find_ops(contents);
    second(rows, nrows, width, ops);
    free(rows);
    free(contents);
    return 0;
}