#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Parse binary string (allows optional 0b prefix)
unsigned long long parse_binary(const char *s)
{
    unsigned long long v = 0;
    if (!s)
        return 0;
    if (s[0] == '0' && (s[1] == 'b' || s[1] == 'B'))
        s += 2;
    while (*s)
    {
        if (*s == '0' || *s == '1')
        {
            v = (v << 1) + (*s - '0');
        }
        else
        {
            break; // stop at first non-binary char
        }
        s++;
    }
    return v;
}

// Parse input string according to chosen format
int parse_input(int fmt, const char *s, unsigned long long *out)
{
    char *endptr = NULL;
    if (!s || !out)
        return 0;
    switch (fmt)
    {
    case 1: // decimal
        *out = strtoull(s, &endptr, 10);
        return (*endptr == '\0');
    case 2: // hex
        // allow with or without 0x
        if (strlen(s) > 1 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
            *out = strtoull(s, &endptr, 0);
        else
            *out = strtoull(s, &endptr, 16);
        return (*endptr == '\0');
    case 3: // octal
        *out = strtoull(s, &endptr, 8);
        return (*endptr == '\0');
    case 4: // binary
        *out = parse_binary(s);
        // verify that at least one valid binary digit existed
        return 1;
    default:
        return 0;
    }
}

// Print as binary with 0b prefix
void print_binary(unsigned long long v)
{
    if (v == 0)
    {
        printf("0b0\n");
        return;
    }
    char buf[65];
    int i = 0;
    while (v)
    {
        buf[i++] = (v & 1) ? '1' : '0';
        v >>= 1;
    }
    printf("0b");
    for (int j = i - 1; j >= 0; --j)
        putchar(buf[j]);
    putchar('\n');
}

int main()
{
    int in_fmt, out_fmt;
    char input[256];
    unsigned long long value;

    printf("=== FORMAT CONVERTER ===\n");
    printf("Input format (enter number):\n");
    printf(" 1) Decimal\n 2) Hex (with or without 0x)\n 3) Octal\n 4) Binary (0b prefix optional)\n");
    printf("Choose input format: ");
    if (scanf("%d", &in_fmt) != 1)
        return 0;
    // eat newline
    getchar();

    printf("Enter value: ");
    if (!fgets(input, sizeof(input), stdin))
        return 0;
    // trim newline and spaces
    char *p = input;
    while (*p && isspace((unsigned char)*p))
        p++;
    char *end = input + strlen(input) - 1;
    while (end >= input && isspace((unsigned char)*end))
    {
        *end = '\0';
        --end;
    }

    if (!parse_input(in_fmt, p, &value))
    {
        printf("Error: invalid input format or value.\n");
        return 0;
    }

    printf("Output format (enter number):\n");
    printf(" 1) Decimal\n 2) Hex (0x...)\n 3) Octal\n 4) Binary (0b...)\n");
    printf("Choose output format: ");
    if (scanf("%d", &out_fmt) != 1)
        return 0;

    printf("Value parsed: %llu (decimal)\n", value);
    switch (out_fmt)
    {
    case 1:
        printf("Decimal: %llu\n", value);
        break;
    case 2:
        printf("Hex: 0x%llX\n", value);
        break;
    case 3:
        printf("Octal: 0%llo\n", value);
        break;
    case 4:
        print_binary(value);
        break;
    default:
        printf("Invalid output choice\n");
    }

    return 0;
}
