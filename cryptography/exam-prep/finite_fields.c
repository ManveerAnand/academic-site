// FINITE FIELDS - GF(2^n) Polynomial Operations
#include <stdio.h>
#include <stdint.h>

// Polynomial representation: coefficients as bits
// Example: x^3 + x + 1 = 0b1011 = 11

// GF(2) Polynomial Addition (XOR)
uint32_t gf2_poly_add(uint32_t a, uint32_t b)
{
    return a ^ b;
}

// GF(2) Polynomial Multiplication (without reduction)
uint32_t gf2_poly_mult_raw(uint32_t a, uint32_t b)
{
    uint32_t result = 0;
    while (b)
    {
        if (b & 1)
            result ^= a;
        a <<= 1;
        b >>= 1;
    }
    return result;
}

// GF(2) Polynomial Modulo
uint32_t gf2_poly_mod(uint32_t a, uint32_t mod)
{
    int deg_mod = 0;
    uint32_t temp = mod;
    while (temp >>= 1)
        deg_mod++;

    while (1)
    {
        int deg_a = 0;
        temp = a;
        while (temp >>= 1)
            deg_a++;

        if (deg_a < deg_mod)
            break;
        a ^= (mod << (deg_a - deg_mod));
    }
    return a;
}

// GF(2) Polynomial Multiplication with Reduction
uint32_t gf2_poly_mult(uint32_t a, uint32_t b, uint32_t mod)
{
    uint32_t result = gf2_poly_mult_raw(a, b);
    return gf2_poly_mod(result, mod);
}

// Extended Euclidean Algorithm for Polynomials over GF(2)
uint32_t gf2_extended_gcd(uint32_t a, uint32_t b, uint32_t *x, uint32_t *y)
{
    if (b == 0)
    {
        *x = 1;
        *y = 0;
        return a;
    }

    uint32_t x1, y1;
    uint32_t q = 0, r = a;

    // Simple division (subtract until smaller)
    while (r >= b)
    {
        int deg_r = 0, deg_b = 0;
        uint32_t temp = r;
        while (temp >>= 1)
            deg_r++;
        temp = b;
        while (temp >>= 1)
            deg_b++;

        if (deg_r < deg_b)
            break;
        r ^= (b << (deg_r - deg_b));
    }

    uint32_t g = gf2_extended_gcd(b, r, &x1, &y1);
    *x = y1;
    *y = x1 ^ gf2_poly_mult_raw(q, y1);
    return g;
}

// Polynomial Multiplicative Inverse in GF(2^n) - BRUTE FORCE (works for GF(2^8))
uint32_t gf2_poly_inverse(uint32_t poly, uint32_t mod)
{
    if (poly == 0)
        return 0; // No inverse for 0

    // Brute force: try all values 1 to 255 for GF(2^8)
    for (uint32_t i = 1; i < 256; i++)
    {
        if (gf2_poly_mult(poly, i, mod) == 1)
        {
            return i; // Found inverse!
        }
    }
    return 0; // No inverse found
}

// Print polynomial in readable form
void print_poly(uint32_t p)
{
    int first = 1;
    for (int i = 31; i >= 0; i--)
    {
        if (p & (1 << i))
        {
            if (!first)
                printf(" + ");
            if (i > 1)
                printf("x^%d", i);
            else if (i == 1)
                printf("x");
            else
                printf("1");
            first = 0;
        }
    }
    if (first)
        printf("0");
}

// MAIN - MENU DRIVEN
int main()
{
    int choice;
    printf("=== FINITE FIELD GF(2^n) OPERATIONS ===\n");
    printf("1. Polynomial Multiplication (with modulus)\n");
    printf("2. Polynomial Modular Inverse\n");
    printf("3. Print Polynomial (readable form)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    uint32_t aes_mod = 0b100011011; // Default: x^8 + x^4 + x^3 + x + 1

    switch (choice)
    {
    case 1:
    {
        unsigned int a, b;
        printf("Enter two polynomials (hex, e.g., 57 83): ");
        scanf("%x %x", &a, &b);
        printf("Use AES modulus? (1=yes, 0=enter custom): ");
        int use_aes;
        scanf("%d", &use_aes);
        uint32_t mod = aes_mod;
        if (!use_aes)
        {
            printf("Enter modulus (hex): ");
            scanf("%x", &mod);
        }
        uint32_t result = gf2_poly_mult(a, b, mod);
        printf("Result: 0x%02X\n", result);
        break;
    }
    case 2:
    {
        unsigned int poly;
        printf("Enter polynomial (hex): ");
        scanf("%x", &poly);
        printf("Use AES modulus? (1=yes, 0=enter custom): ");
        int use_aes;
        scanf("%d", &use_aes);
        uint32_t mod = aes_mod;
        if (!use_aes)
        {
            printf("Enter modulus (hex): ");
            scanf("%x", &mod);
        }
        uint32_t inv = gf2_poly_inverse(poly, mod);
        if (inv == 0)
            printf("No inverse exists!\n");
        else
            printf("Inverse: 0x%02X\n", inv);
        break;
    }
    case 3:
    {
        unsigned int poly;
        printf("Enter polynomial (hex): ");
        scanf("%x", &poly);
        printf("Polynomial: ");
        print_poly(poly);
        printf("\n");
        break;
    }
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
