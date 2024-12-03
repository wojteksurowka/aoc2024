#include <stdio.h>
#include <stdlib.h>

void first()
{
    FILE* fp = fopen("in03.txt", "r");
    int c;
    int state = 0;
    int total = 0;
    int l, r;
    while ((c = fgetc(fp)) != -1)
    {
        if (state == 0 && c == 'm')
            state = 'm';
        else if (state == 'm' && c == 'u')
            state = 'u';
        else if (state == 'u' && c == 'l')
            state = 'l';
        else if (state == 'l' && c == '(')
        {
            state = '(';
            l = 0;
        }
        else if (state == '(' && c >= '0' && c <= '9')
            l = l * 10 + c - '0';
        else if (state == '(' && c == ',')
        {
            state = ',';
            r = 0;
        }
        else if (state == ',' && c >= '0' && c <= '9')
            r = r * 10 + c - '0';
        else if (state == ',' && c == ')')
        {
            total += l * r;
            state = 0;
        }
        else
            state = 0;
    }
    printf("%d\n", total);
    fclose(fp);
}

void second()
{
    FILE* fp = fopen("in03.txt", "r");
    int c;
    int state = 0;
    int total = 0;
    int l, r;
    int enabled = 1;
    while ((c = fgetc(fp)) != -1)
    {
        if (state == 0 && c == 'm')
            state = 'm';
        else if (state == 'm' && c == 'u')
            state = 'u';
        else if (state == 'u' && c == 'l')
            state = 'l';
        else if (state == 'l' && c == '(')
        {
            state = '(';
            l = 0;
        }
        else if (state == '(' && c >= '0' && c <= '9')
            l = l * 10 + c - '0';
        else if (state == '(' && c == ',')
        {
            state = ',';
            r = 0;
        }
        else if (state == ',' && c >= '0' && c <= '9')
            r = r * 10 + c - '0';
        else if (state == ',' && c == ')')
        {
            if (enabled)
                total += l * r;
            state = 0;
        }
        else if (state == 0 && c == 'd')
            state = 'd';
        else if (state == 'd' && c == 'o')
            state = 'o';
        else if (state == 'o' && c == '(')
            state = '[';
        else if (state == '[' && c == ')')
        {
            enabled = 1;
            state = 0;
        }
        else if (state == 'o' && c == 'n')
            state = 'n';
        else if (state == 'n' && c == '\'')
            state = '\'';
        else if (state == '\'' && c == 't')
            state = 't';
        else if (state == 't' && c == '(')
            state = '{';
        else if (state == '{' && c == ')')
        {
            enabled = 0;
            state = 0;
        }
        else
            state = 0;
    }
    printf("%d\n", total);
    fclose(fp);
}

int main()
{
    second();
    return 0;
}