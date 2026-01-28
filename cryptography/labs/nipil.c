// Your Name
// Your Roll Number
// attack_guess.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

unsigned char S[16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
unsigned char Sinv[16] = {0};

/* Function to apply the bit permutation P */
uint16_t apply_perm(uint16_t in) {
    uint16_t out = 0;
    int p[16] = {1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
    for (int z = 1; z <= 16; z++) {
        int input_pos = 16 - z; /* z=1 -> bit15 (MSB) ... z=16 -> bit0 (LSB) */
        uint16_t bit = (in >> input_pos) & 1u;
        int output_pos = 16 - p[z-1];
        out |= (uint16_t)(bit << output_pos);
    }
    return out;
}

/* Derive round keys K1..K5 (each 16-bit) from 32-bit master key. Kr[r] for r=1..5. */
void derive_round_keys(uint32_t master_key, uint16_t Kr[6]) {
    for (int r = 1; r <= 5; ++r) {
        int start_k = 4 * r - 4; /* 0-based from MSB */
        uint16_t val = 0;
        for (int i = 0; i < 16; ++i) {
            int bit_index_from_MSB = start_k + i; /* 0..31 */
            int from_bitpos = 31 - bit_index_from_MSB;
            uint16_t bit = (master_key >> from_bitpos) & 1u;
            val = (uint16_t)((val << 1) | bit);
        }
        Kr[r] = val;
    }
}

/* Apply S-layer to a 16-bit word (apply 4-bit S-box to each nibble) */
uint16_t apply_slayer(uint16_t state) {
    uint16_t out = 0;
    for (int i = 0; i < 4; ++i) {
        uint16_t nib = (state >> (12 - 4 * i)) & 0xF;
        uint16_t s = S[nib] & 0xF;
        out |= (uint16_t)(s << (12 - 4 * i));
    }
    return out;
}

/* Encrypt 16-bit plaintext with the SPN using the pre-derived round keys Kr[1..5] */
uint16_t encrypt_spn(uint16_t pt, uint16_t Kr[6]) {
    uint16_t state = pt;
    for (int r = 1; r <= 3; ++r) {
        state ^= Kr[r];
        state = apply_slayer(state);
        state = apply_perm(state);
    }
    state ^= Kr[4];
    state = apply_slayer(state);
    uint16_t y = state ^ Kr[5];
    return y;
}

int main() {
    /* build inverse S-box */
    for (int i = 0; i < 16; ++i) Sinv[i] = 0;
    for (int i = 0; i < 16; ++i) Sinv[S[i]] = (unsigned char)i;

    uint32_t key;
    printf("Enter the 32-bit master key in hexadecimal (e.g. ABCD1234): ");
    if (scanf("%x", &key) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    uint16_t Kr[6] = {0};
    derive_round_keys(key, Kr);
    uint16_t k5 = Kr[5];
    printf("Last round key K5 (derived) : %04X\n\n", k5);

    /* Counts for each possible 8-bit key guess (block2 and block4 nibbles) */
    int counts[256] = {0};

    /* Generate chosen plaintext pairs with Δx = 0x0B00 */
    srand((unsigned)time(NULL));
    const uint16_t DX = 0x0B00;
    const int NUM_PAIRS = 100;

    for (int i = 0; i < NUM_PAIRS; ++i) {
        uint16_t x = (uint16_t)(rand() & 0xFFFF);
        uint16_t xp = x ^ DX;
        uint16_t y = encrypt_spn(x, Kr);
        uint16_t yp = encrypt_spn(xp, Kr);
        uint16_t dy = y ^ yp;

        /* Check Δy for block1 (MS nibble) and block3 (3rd nibble) are zero (per the trail) */
        uint16_t dy_block1 = (dy >> 12) & 0xF;
        uint16_t dy_block3 = (dy >> 4) & 0xF;
        if (dy_block1 != 0 || dy_block3 != 0) continue;

        /* Try all possible 8-bit key guesses */
        for (int guess = 0; guess < 256; ++guess) {
            unsigned char k_block2 = (guess >> 4) & 0xF;
            unsigned char k_block4 = guess & 0xF;

            unsigned char y_block2 = (y >> 8) & 0xF;
            unsigned char v2 = (unsigned char)(y_block2 ^ k_block2);
            unsigned char u2 = Sinv[v2];

            unsigned char yp_block2 = (yp >> 8) & 0xF;
            unsigned char vp2 = (unsigned char)(yp_block2 ^ k_block2);
            unsigned char up2 = Sinv[vp2];

            unsigned char du2 = (unsigned char)(u2 ^ up2);

            unsigned char y_block4 = y & 0xF;
            unsigned char v4 = (unsigned char)(y_block4 ^ k_block4);
            unsigned char u4 = Sinv[v4];

            unsigned char yp_block4 = yp & 0xF;
            unsigned char vp4 = (unsigned char)(yp_block4 ^ k_block4);
            unsigned char up4 = Sinv[vp4];

            unsigned char du4 = (unsigned char)(u4 ^ up4);

            /* Check the expected difference (0x6 for both targeted nibbles) */
            if (du2 == 0x6 && du4 == 0x6) counts[guess]++;
        }
    }

    /* Print the table */
    printf("Key (hex byte)  Count\n");
    printf("---------------------\n");
    for (int g = 0; g < 256; ++g) {
        printf("0x%02X           %d\n", g, counts[g]);
    }

    /* Find the maximum count and collect best candidates (handles ties) */
    int maxc = 0;
    for (int g = 0; g < 256; ++g) if (counts[g] > maxc) maxc = counts[g];

    if (maxc == 0) {
        printf("\nNo candidate produced any matches (max count = 0). Try increasing NUM_PAIRS or checking trail assumptions.\n");
        return 0;
    }

    printf("\nBest candidate count = %d\n", maxc);
    printf("Guessed candidate(s):\n");
    for (int g = 0; g < 256; ++g) {
        if (counts[g] == maxc) {
            unsigned char k2 = (g >> 4) & 0xF;
            unsigned char k4 = g & 0xF;
            /* Partial K5 pattern: nibble1 = ?, nibble2 = k2, nibble3 = ?, nibble4 = k4
               Show a concrete K5 assuming unknown nibbles = 0x0 for quick testing. */
            uint16_t K5_assume_zero = ((uint16_t)k2 << 8) | (uint16_t)k4; /* nib1=0,nib3=0 -> (k2<<8)|k4 */
            printf(" - candidate byte 0x%02X : nibble2=0x%X, nibble4=0x%X; partial K5 pattern = ? %X ? %X; "
                   "K5 if other nibbles = 0x0 -> 0x%04X\n",
                   g, k2, k4, k2, k4, K5_assume_zero);
        }
    }

    /* Also print the actual derived K5 (for verification if you provided the real master key) */
    printf("\n(Actual derived K5 = 0x%04X)\n", k5);

    return 0;
}