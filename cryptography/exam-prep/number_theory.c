// NUMBER THEORY - Foundation for RSA, DH, etc.
#include <stdio.h>

// Modular Exponentiation: (base^exp) % n
long long mod_exp(long long base, long long exp, long long n)
{
    long long result = 1;
    base = base % n;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = (result * base) % n;
        exp = exp >> 1;
        base = (base * base) % n;
    }
    return result;
}

// GCD using Euclidean algorithm
long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Extended Euclidean Algorithm: ax + by = gcd(a,b)
long long extended_gcd(long long a, long long b, long long *x, long long *y)
{
    if (b == 0)
    {
        *x = 1;
        *y = 0;
        return a;
    }
    long long x1, y1;
    long long g = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

// Modular Inverse: a^(-1) mod n
long long mod_inverse(long long a, long long n)
{
    long long x, y;
    long long g = extended_gcd(a, n, &x, &y);
    if (g != 1)
        return -1; // No inverse exists
    return (x % n + n) % n;
}

// Chinese Remainder Theorem: Solve x ≡ a1 (mod n1) and x ≡ a2 (mod n2)
long long crt(long long a1, long long n1, long long a2, long long n2)
{
    long long x, y;
    extended_gcd(n1, n2, &x, &y);
    long long result = (a1 * n2 * y + a2 * n1 * x);
    long long n = n1 * n2;
    return ((result % n) + n) % n;
}

// Miller-Rabin Primality Test (simplified)
int is_prime(long long n)
{
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0)
        return 0;

    // Write n-1 as 2^r * d
    long long d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    // Test with a few witnesses
    long long witnesses[] = {2, 3, 5, 7, 11, 13, 17};
    for (int i = 0; i < 7; i++)
    {
        long long a = witnesses[i];
        if (a >= n)
            continue;

        long long x = mod_exp(a, d, n);
        if (x == 1 || x == n - 1)
            continue;

        int composite = 1;
        long long temp = d;
        while (temp != n - 1)
        {
            x = (x * x) % n;
            temp *= 2;
            if (x == n - 1)
            {
                composite = 0;
                break;
            }
        }
        if (composite)
            return 0;
    }
    return 1;
}

// MAIN - MENU DRIVEN
int main()
{
    int choice;
    printf("=== NUMBER THEORY OPERATIONS ===\n");
    printf("1. Modular Exponentiation (a^b mod n)\n");
    printf("2. Modular Inverse (a^-1 mod n)\n");
    printf("3. GCD\n");
    printf("4. Extended GCD\n");
    printf("5. CRT (2 equations)\n");
    printf("6. Primality Test\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        long long a, b, n;
        printf("Enter base, exponent, modulus: ");
        scanf("%lld %lld %lld", &a, &b, &n);
        printf("%lld^%lld mod %lld = %lld\n", a, b, n, mod_exp(a, b, n));
        break;
    }
    case 2:
    {
        long long a, n;
        printf("Enter a and modulus n: ");
        scanf("%lld %lld", &a, &n);
        long long inv = mod_inverse(a, n);
        if (inv == -1)
            printf("No inverse exists!\n");
        else
            printf("%lld^-1 mod %lld = %lld\n", a, n, inv);
        break;
    }
    case 3:
    {
        long long a, b;
        printf("Enter two numbers: ");
        scanf("%lld %lld", &a, &b);
        printf("GCD(%lld, %lld) = %lld\n", a, b, gcd(a, b));
        break;
    }
    case 4:
    {
        long long a, b, x, y;
        printf("Enter two numbers: ");
        scanf("%lld %lld", &a, &b);
        long long g = extended_gcd(a, b, &x, &y);
        printf("GCD = %lld\n", g);
        printf("%lld*(%lld) + %lld*(%lld) = %lld\n", a, x, b, y, g);
        break;
    }
    case 5:
    {
        long long a1, n1, a2, n2;
        printf("Enter a1, n1, a2, n2 for:\n");
        printf("x ≡ a1 (mod n1)\n");
        printf("x ≡ a2 (mod n2)\n");
        scanf("%lld %lld %lld %lld", &a1, &n1, &a2, &n2);
        printf("x = %lld\n", crt(a1, n1, a2, n2));
        break;
    }
    case 6:
    {
        long long n;
        printf("Enter number to test: ");
        scanf("%lld", &n);
        printf("%lld is %s\n", n, is_prime(n) ? "PRIME" : "COMPOSITE");
        break;
    }
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
