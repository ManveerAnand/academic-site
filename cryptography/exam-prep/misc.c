// MISCELLANEOUS - Birthday, Bayesian, LFSR
#include <stdio.h>
#include <math.h>

// Birthday Paradox: Find k people for given probability
// Formula: 1 - (n!/(n-k)!) / n^k ≥ probability
int birthday_paradox(int n, double prob)
{
    double p = 1.0;
    int k = 1;

    while (p > (1.0 - prob))
    {
        p *= (double)(n - k) / n;
        k++;
        if (k > n)
            break;
    }

    return k;
}

// Bayesian Probability: Pr[P=0 | C=1]
// Given: C = P XOR K, Pr[K=0], Pr[P=1]
// C=1 can happen when: (P=1, K=0) or (P=0, K=1)
double bayesian_prob(double pr_k0, double pr_p1)
{
    double pr_k1 = 1.0 - pr_k0;
    double pr_p0 = 1.0 - pr_p1;

    // Pr[C=1] = Pr[P=1,K=0] + Pr[P=0,K=1]
    double pr_c1 = pr_p1 * pr_k0 + pr_p0 * pr_k1;

    // Pr[P=0 | C=1] = Pr[P=0,K=1] / Pr[C=1]
    double result = (pr_p0 * pr_k1) / pr_c1;

    return result;
}

// LFSR Period Calculator - Standard Fibonacci LFSR
// Polynomial x^n + ... represents tap positions
// For x^8+x^4+x^3+x+1, the taps are at bit positions [8,4,3,1,0]
// Input as 0b100011011 (bit positions where coefficient = 1)
int lfsr_period(unsigned int poly, unsigned int init_state)
{
    if (init_state == 0)
        return 0;

    // Find the highest bit (degree)
    int n = 0;
    unsigned int temp = poly;
    while (temp >>= 1)
        n++;

    unsigned int start = init_state & ((1u << n) - 1); // Mask to n bits
    unsigned int lfsr = start;
    int period = 0;

    do
    {
        // Get LSB
        unsigned int lsb = lfsr & 1;

        // Shift right
        lfsr >>= 1;

        // If LSB was 1, XOR with polynomial (without the x^n term)
        if (lsb)
        {
            lfsr ^= (poly >> 1);
        }

        period++;

        if (period > 100000)
            break; // Safety limit

    } while (lfsr != start);

    return period;
}

// MAIN - MENU DRIVEN
int main()
{
    int choice;
    printf("=== MISC OPERATIONS ===\n");
    printf("1. Birthday Paradox\n");
    printf("2. Bayesian Probability\n");
    printf("3. LFSR Period\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        int n;
        double prob;
        printf("Enter n (e.g., 365 for days): ");
        scanf("%d", &n);
        printf("Enter probability (e.g., 0.5 for 50%%): ");
        scanf("%lf", &prob);
        int k = birthday_paradox(n, prob);
        printf("Minimum k = %d\n", k);
        break;
    }
    case 2:
    {
        double pr_k0, pr_p1;
        printf("Enter Pr[K=0]: ");
        scanf("%lf", &pr_k0);
        printf("Enter Pr[P=1]: ");
        scanf("%lf", &pr_p1);
        double result = bayesian_prob(pr_k0, pr_p1);
        printf("Pr[P=0|C=1] = %.6f\n", result);
        break;
    }
    case 3:
    {
        unsigned int poly, init_state;
        printf("Enter polynomial (decimal, e.g., 37 for x^5+x^2+1): ");
        scanf("%u", &poly);
        printf("Enter initial state (any non-zero, usually 1): ");
        scanf("%u", &init_state);
        int period = lfsr_period(poly, init_state);
        printf("LFSR Period: %d\n", period);
        break;
    }
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
