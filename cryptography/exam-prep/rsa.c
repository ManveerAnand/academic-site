// RSA - Complete Toolkit
#include <stdio.h>
#include <math.h>

// Helper: Modular Exponentiation
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

// Helper: Extended GCD
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

// Helper: Modular Inverse
long long mod_inverse(long long a, long long n)
{
    long long x, y;
    long long g = extended_gcd(a, n, &x, &y);
    if (g != 1)
        return -1;
    return (x % n + n) % n;
}

// RSA Key Generation
void rsa_keygen(long long p, long long q, long long e,
                long long *n, long long *phi, long long *d)
{
    *n = p * q;
    *phi = (p - 1) * (q - 1);
    *d = mod_inverse(e, *phi);
}

// RSA Encryption: C = M^e mod n
long long rsa_encrypt(long long M, long long e, long long n)
{
    return mod_exp(M, e, n);
}

// RSA Decryption: M = C^d mod n
long long rsa_decrypt(long long C, long long d, long long n)
{
    return mod_exp(C, d, n);
}

// Recover p and q from n and phi(n)
// Using: p + q = n - phi(n) + 1 and p * q = n
void recover_pq_from_phi(long long n, long long phi, long long *p, long long *q)
{
    long long sum = n - phi + 1;
    long long discriminant = sum * sum - 4 * n;

    // p = (sum + sqrt(discriminant)) / 2
    // q = (sum - sqrt(discriminant)) / 2
    long long sqrt_d = (long long)sqrt(discriminant);
    *p = (sum + sqrt_d) / 2;
    *q = (sum - sqrt_d) / 2;
}

// MAIN - MENU DRIVEN
int main()
{
    int choice;
    printf("=== RSA OPERATIONS ===\n");
    printf("1. Key Generation (p, q, e given)\n");
    printf("2. Encrypt (M, e, n given)\n");
    printf("3. Decrypt (C, d, n given)\n");
    printf("4. Recover p, q from n and phi(n)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        long long p, q, e, n, phi, d;
        printf("Enter p, q, e: ");
        scanf("%lld %lld %lld", &p, &q, &e);
        rsa_keygen(p, q, e, &n, &phi, &d);
        printf("n = %lld\n", n);
        printf("phi(n) = %lld\n", phi);
        printf("d = %lld\n", d);
        break;
    }
    case 2:
    {
        long long M, e, n;
        printf("Enter message M, public exponent e, modulus n: ");
        scanf("%lld %lld %lld", &M, &e, &n);
        long long C = rsa_encrypt(M, e, n);
        printf("Ciphertext C = %lld\n", C);
        break;
    }
    case 3:
    {
        long long C, d, n;
        printf("Enter ciphertext C, private exponent d, modulus n: ");
        scanf("%lld %lld %lld", &C, &d, &n);
        long long M = rsa_decrypt(C, d, n);
        printf("Plaintext M = %lld\n", M);
        break;
    }
    case 4:
    {
        long long n, phi, p, q;
        printf("Enter n and phi(n): ");
        scanf("%lld %lld", &n, &phi);
        recover_pq_from_phi(n, phi, &p, &q);
        printf("p = %lld, q = %lld\n", p, q);
        break;
    }
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
