// DIFFIE-HELLMAN Key Exchange
#include <stdio.h>

// Modular Exponentiation
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

// Calculate public key: g^private mod p
long long dh_public_key(long long g, long long private_key, long long p)
{
    return mod_exp(g, private_key, p);
}

// Calculate shared secret: other_public^private mod p
long long dh_shared_secret(long long other_public, long long private_key, long long p)
{
    return mod_exp(other_public, private_key, p);
}

// Full DH Exchange Simulation
void dh_full_exchange(long long p, long long g, long long a, long long b)
{
    // Alice's side
    long long A = dh_public_key(g, a, p);
    printf("Alice private key: %lld\n", a);
    printf("Alice public key:  A = %lld^%lld mod %lld = %lld\n", g, a, p, A);

    // Bob's side
    long long B = dh_public_key(g, b, p);
    printf("\nBob private key:   %lld\n", b);
    printf("Bob public key:    B = %lld^%lld mod %lld = %lld\n", g, b, p, B);

    // Shared secrets
    long long secret_alice = dh_shared_secret(B, a, p);
    long long secret_bob = dh_shared_secret(A, b, p);
    printf("\nAlice computes:    %lld^%lld mod %lld = %lld\n", B, a, p, secret_alice);
    printf("Bob computes:      %lld^%lld mod %lld = %lld\n", A, b, p, secret_bob);
    printf("\nShared secret: %lld\n", secret_alice);
}

// MAIN - MENU DRIVEN
int main()
{
    int choice;
    printf("=== DIFFIE-HELLMAN OPERATIONS ===\n");
    printf("1. Calculate Public Key (g^private mod p)\n");
    printf("2. Calculate Shared Secret\n");
    printf("3. Full DH Exchange (see both sides)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        long long g, priv, p;
        printf("Enter g, private_key, p: ");
        scanf("%lld %lld %lld", &g, &priv, &p);
        long long pub = dh_public_key(g, priv, p);
        printf("Public Key = %lld\n", pub);
        break;
    }
    case 2:
    {
        long long other_pub, my_priv, p;
        printf("Enter other's_public_key, my_private_key, p: ");
        scanf("%lld %lld %lld", &other_pub, &my_priv, &p);
        long long shared = dh_shared_secret(other_pub, my_priv, p);
        printf("Shared Secret = %lld\n", shared);
        break;
    }
    case 3:
    {
        long long p, g, a, b;
        printf("Enter p, g, Alice's_private, Bob's_private: ");
        scanf("%lld %lld %lld %lld", &p, &g, &a, &b);
        dh_full_exchange(p, g, a, b);
        break;
    }
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
