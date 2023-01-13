#include <stdlib.h>
#include <stdbool.h>

#include "sas.h"

bool is_alphanumeric(int c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int itos(int n, char* buffer)
{
    int i, sign;

    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do
        buffer[i++] = abs(n % 10) + '0';
    while (n /= 10);
    if (sign < 0)
        buffer[i++] = '-';
    buffer[i] = '\0';
    for (int j = 0; j < i / 2; j++)
    {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
    return i;
}