/*                                                          LAB ASSIGNMENT - II (LAB-A2)
                                                           ------------------------------
    Name - Manveer Anand
    roll no. - 202351080
    section - 1

    In this assignment we have to implement a differential cryptanalysis attack on a simplified SPN cipher.
    The SPN cipher uses a 32-bit key and operates on 16-bit plaintexts
    The SPN structure consists of 4 rounds of substitution and permutation, followed by a final round of substitution and key mixing.
    The S-box and P-box used in the SPN are provided in the assignment.
    The differential characteristic used for the attack is also provided.
    The goal of the attack is to recover the last round key (K5) used in the SPN cipher.
    The attack involves generating a large number of plaintext pairs with a specific input difference, encrypting them, and analyzing the output differences to gain information about the key.

    NOTE : *commented print statements were used for debugging purposes.
*/



#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // for rand() and srand() function that generate random numbers(plain texts)
#include <time.h> // for seeding the random number generator
#include <string.h> 

#define N 10000 // number of plaintext pairs to be generated.

unsigned char sbox[16] = {
    0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};

unsigned char inv_sbox[16] = {
    0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};

unsigned char p_box[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

// this function applies the permutation box on a 16 bit input and returns the permuted output.
unsigned short pbox(unsigned short v)
{
    unsigned short output = 0;
    for (int i = 0; i < 16; i++)
    {
        unsigned short bit = (v >> i) & 1; // extract the i-th bit
        output |= (bit << p_box[i]);       // set the bit at the new position
    }
    return output;
}


/*
    This function implements the Simplified SPN (Substitution-Permutation Network) cipher.
    It takes a 16-bit plaintext 'x', a 32-bit key, and a flag to indicate whether to generate round keys. 
    As the round keys will be fixed for a particular key i have declared it globally, and we are only generating them once.
    The function performs 4 rounds of substitution and permutation, followed by a final round of substitution.
    and key mixing, returning the resulting 16-bit ciphertext.
*/

unsigned short round_key[5];
unsigned short SPN(unsigned short x, unsigned int key, int flag)
{
    // as this will be fixed for a particular key i have declared it globally.
    // unsigned short round_key[5];
    if (flag == 0)
    { // does this only once for a particular key.
        for (int i = 0; i < 5; i++)
        {
            round_key[i] = (unsigned short)((key >> (16 - 4 * i)) & 0xFFFF);
        }
    }
    unsigned short u[5]; // u1 to u4
    unsigned short w[5]; // w1 to w4
    unsigned short v[5]; // v1 to v4
    u[0] = x;
    w[0] = x;
    // initial key mixing
    for (int i = 1; i <= 3; i++)
    {
        u[i] = w[i - 1] ^ round_key[i - 1];
        uint8_t n1 = (u[i] >> 12) & 0xF;
        uint8_t n2 = (u[i] >> 8) & 0xF;
        uint8_t n3 = (u[i] >> 4) & 0xF;
        uint8_t n4 = (u[i] >> 0) & 0xF;
        n1 = sbox[n1];
        n2 = sbox[n2];
        n3 = sbox[n3];
        n4 = sbox[n4];
        v[i] = (n1 << 12) | (n2 << 8) | (n3 << 4) | (n4 << 0);
        w[i] = pbox(v[i]);
    }
    // final round of key mixing basically the 4th round.
    u[4] = w[3] ^ round_key[3];
    uint8_t n1 = (u[4] >> 12) & 0xF;
    uint8_t n2 = (u[4] >> 8) & 0xF;
    uint8_t n3 = (u[4] >> 4) & 0xF;
    uint8_t n4 = (u[4] >> 0) & 0xF;
    n1 = sbox[n1];
    n2 = sbox[n2];
    n3 = sbox[n3];
    n4 = sbox[n4];
    v[4] = (n1 << 12) | (n2 << 8) | (n3 << 4) | (n4 << 0);
    unsigned short y = v[4] ^ round_key[4];
    return y;
}

/*
    The main function orchestrates the differential cryptanalysis experiment.
    High‑level workflow:
    1. Initialization
       - Seed the PRNG with current time (srand(time(NULL))).
       - Define the fixed input difference delta_x = 0x0B00 as given in the assignment.
       - Declare arrays:
         * x[]      : original randomly generated plaintexts (size N).
         * x_star[] : paired plaintexts where x_star[i] = x[i] ^ delta_x.
         * y[]      : ciphertexts of x[i].
         * y_star[] : ciphertexts of x_star[i].
       - Read a 32‑bit master key (hex) from the user (scanf).

    2. Encryption phase (data generation)
       - Generate N random 16‑bit plaintexts x[i].
       - Encrypt each x[i] with SPN():
         * First call supplies flag = 0 to derive and cache round_key[0..4].
         * Subsequent calls use flag = 1 (reuse already expanded round keys).
       - For each x[i], form its paired plaintext x_star[i] = x[i] ^ delta_x.
       - Encrypt each x_star[i] → y_star[i] (flag = 1).

    3. Key hypothesis space construction (targeting 8 bits of K5)
       - Build brute_key_S2[256] containing all 8‑bit candidates (two 4‑bit nibbles)
         corresponding to the 2nd and 4th S-box positions of the last round.
         (Only those two nibbles are attacked because the chosen input difference
          propagates so that only those S-boxes are active in the last round differential.)

    4. Differential filtering / counting
       - For every 8‑bit subkey guess (L1 || L2):
         * Extract the 2nd and 4th ciphertext nibbles from y[i] and y_star[i].
         * Partially decrypt those nibbles through: (cipher_nibble ^ subkey_nibble) then inverse S-box.
         * Compute the nibble differences (n1 ^ n1_star) and (n2 ^ n2_star).
         * If both equal the expected output difference 0x6 (from the provided characteristic),
           increment the count for that subkey guess.
       - cnt[256] accumulates how many plaintext/ciphertext pairs support each guess.

    5. Result analysis / selection
       - Identify the subkey guess with maximum supporting count (argmax over cnt[]).
       - Print:
         * Actual stored round_key[4] (full 16 bits of K5) for verification.
         * Most probable 8‑bit recovered portion (the attacked two nibbles) and its count.
         * Full table of all 256 guesses and their supporting counts (for inspection).

    6. Program termination
       - Return 0 to signal successful completion.

    Notes:
    - Only part of K5 is recovered (two active S-box nibbles) because the differential
      characteristic activates those positions; extending the attack would require
      crafting characteristics covering other nibbles.
    - The first SPN() call with flag = 0 expands the master key once; subsequent calls
      avoid recomputation for efficiency.
    - N (=10000) controls statistical confidence; larger N generally improves bias visibility. but we tried for 100 plain text as mentioned in the question.
*/

int main()
{
    srand(time(NULL)); // seed for random number generation.
    //------------------------------------------------------------------------------------------------------
    unsigned short delta_x = 0x0B00; // 0000 1011 0000 0000 given in the question.
    unsigned short x[N];             // it is 16 bit number.
    unsigned short x_star[N];
    unsigned short y[N];
    unsigned short y_star[N];
    //--------------------------------------------------------------------------------------------------------
    unsigned int key; // it is 32 bit key. which is the main or the master key used for generation of round keys in SPN.
    // input the key from the user.
    printf("Enter the 32 bit key in hex format (eg: 01234567): ");
    scanf("%x", &key);

    // generating N plaintexts 'x' basically these are the input for SPN.
    for (int i = 0; i < N; i++)
    {
        x[i] = (unsigned short)(rand() % 65536);
        if (i == 0)
            y[i] = SPN(x[i], key, 0); // getting the corresponding ciphertexts 'y' using SPN.
        else
            y[i] = SPN(x[i], key, 1);
    }

    // let's compute the x* based on the logic of the problem statement.
    // x* = x ^ delta(x).
    for (int i = 0; i < N; i++)
    {
        x_star[i] = x[i] ^ delta_x;
        y_star[i] = SPN(x_star[i], key, 1); // getting the corresponding ciphertexts 'y*' using SPN.
    }
    //-------------------------------------------------------------------------------------------------------------------------

    //====================== DESIGNING the differential attack model to find the key ===========================================
    unsigned char brute_key_S2[256]; // to store all the possible keys for S2. generate all possible 8 bit keys for S2. which is 2 nibbles or 2 hex digits.
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            brute_key_S2[i * 16 + j] = (i << 4) | j; // 8 bit key for S2.
        }
    }

    int cnt[256] = {0}; // to count the number of times each key is valid.

    // now we will iterate over all the possible keys for S2.
    for (int k = 0; k < 256; k++)
    {
        unsigned char L1 = (brute_key_S2[k] >> 4) & 0xF;
        unsigned char L2 = brute_key_S2[k] & 0xF;
        // backtrack to find v4 and v4*
        for (int i = 0; i < N; i++)
        {
            unsigned char n1 = (y[i] >> 8) & 0xF;           // 2nd nibble
            unsigned char n2 = y[i] & 0xF;                  // 4th nibble.
            unsigned char n1_star = (y_star[i] >> 8) & 0xF; // 2nd nibble
            unsigned char n2_star = y_star[i] & 0xF;        // 4th nibble.

            n1 = inv_sbox[n1 ^ L1];
            n2 = inv_sbox[n2 ^ L2];

            n1_star = inv_sbox[n1_star ^ L1];
            n2_star = inv_sbox[n2_star ^ L2];

            if ((n1 ^ n1_star) == 0x6 && (n2 ^ n2_star) == 0x6)
                cnt[k]++;
        }
    }

    //====================== FINAL RESULTS ===============================================

    printf("Original value of K5 is: %04X\n", round_key[4]);

    printf("\nDifferential Cryptanalysis Attack Results:\n");

    // Find the key guess with the maximum count

    int max_count = -1;
    int key_index = -1;

    for (int i = 0; i < 256; i++)
    {
        if (cnt[i] > max_count)
        {
            max_count = cnt[i];
            key_index = i;
        }
    }

    // Print the final winning key and its count
    printf("Attack complete.\n");
    printf("The most probable 8-bit key is: 0x%02X\n", key_index);
    printf("This key was found with a count of: %d\n", max_count);

    printf("\nFull table of key guesses and their counts:\n");
    printf("------------------------------------------\n");
    printf("     Key Guess (Hex) | Count\n");
    printf("------------------------------------------\n");

    // Loop through all 256 possible key guesses to print the full table
    for (int i = 0; i < 256; i++)
    {
        // Print the 8-bit key guess (as two 4-bit nibbles) and its count
        printf("\t%02X\t\t| %d\n", i, cnt[i]);
    }
    printf("------------------------------------------\n\n");
    return 0; // End of the main function
}
