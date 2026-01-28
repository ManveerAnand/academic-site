#include <stdio.h>
ECC
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

// Helper: Modular Inverse
long long mod_inverse(long long a, long long p)
{
    return mod_exp(a, p - 2, p); // Using Fermat's little theorem
}

// ECC Point Addition: P + Q
// Curve: y^2 = x^3 + ax + b (mod p)
void ecc_point_add(long long x1, long long y1, long long x2, long long y2,
                   long long a, long long p, long long *x3, long long *y3)
{

    if (x1 == x2 && y1 == y2)
    {
        // Point doubling case
        long long s = (3 * x1 * x1 + a) % p;
        s = (s * mod_inverse(2 * y1, p)) % p;
        s = (s + p) % p;

        *x3 = (s * s - 2 * x1) % p;
        *x3 = (*x3 + p) % p;

        *y3 = (s * (x1 - *x3) - y1) % p;
        *y3 = (*y3 + p) % p;
    }
    else
    {
        // Point addition case
        long long dy = (y2 - y1 + p) % p;
        long long dx = (x2 - x1 + p) % p;
        long long s = (dy * mod_inverse(dx, p)) % p;

        *x3 = (s * s - x1 - x2) % p;
        *x3 = (*x3 + p) % p;

        *y3 = (s * (x1 - *x3) - y1) % p;
        *y3 = (*y3 + p) % p;
    }
}

// ECC Point Doubling: 2P
void ecc_point_double(long long x, long long y, long long a, long long p,
                      long long *x2, long long *y2)
{
    ecc_point_add(x, y, x, y, a, p, x2, y2);
}

// MAIN - MENU DRIVEN
int main()
{
    int choice;
    printf("=== ECC OPERATIONS ===\n");
    printf("Curve: y^2 = x^3 + ax + b (mod p)\n");
    printf("1. Point Addition (P + Q)\n");
    printf("2. Point Doubling (2P)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        long long x1, y1, x2, y2, a, p, x3, y3;
        printf("Enter a, p for curve: ");
        scanf("%lld %lld", &a, &p);
        printf("Enter point P (x1, y1): ");
        scanf("%lld %lld", &x1, &y1);
        printf("Enter point Q (x2, y2): ");
        scanf("%lld %lld", &x2, &y2);
        ecc_point_add(x1, y1, x2, y2, a, p, &x3, &y3);
        printf("P + Q = (%lld, %lld)\n", x3, y3);
        break;
    }
    case 2:
    {
        long long x1, y1, a, p, x3, y3;
        printf("Enter a, p for curve: ");
        scanf("%lld %lld", &a, &p);
        printf("Enter point P (x, y): ");
        scanf("%lld %lld", &x1, &y1);
        ecc_point_double(x1, y1, a, p, &x3, &y3);
        printf("2P = (%lld, %lld)\n", x3, y3);
        break;
    }
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
