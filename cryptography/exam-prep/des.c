// DES COMPONENTS - S-Boxes and Feistel
#include <stdio.h>
#include <stdint.h>

// All 8 DES S-Boxes
int sbox[8][4][16] = {
    // S1
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    // S4
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    // S5
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    // S6
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    // S7
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    // S8
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 5, 10, 3, 6, 8, 13, 15},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

// DES S-Box Lookup
// input: 6-bit value (0-63)
// sbox_num: 1-8
int des_sbox(int input, int sbox_num)
{
    int row = ((input & 0x20) >> 4) | (input & 0x01); // First and last bit
    int col = (input & 0x1E) >> 1;                    // Middle 4 bits
    return sbox[sbox_num - 1][row][col];
}

// Feistel Round: (L, R) -> (R, L XOR F(R, K))
// You need to provide your own F function
uint32_t simple_f(uint32_t right, uint32_t key)
{
    // Example F function: just XOR with key (customize as needed)
    return right ^ key;
}

void feistel_round(uint32_t *left, uint32_t *right, uint32_t key)
{
    uint32_t new_left = *right;
    uint32_t new_right = *left ^ simple_f(*right, key);
    *left = new_left;
    *right = new_right;
}

// MAIN - MENU DRIVEN
int main()
{
    int choice;
    printf("=== DES OPERATIONS ===\n");
    printf("1. S-Box Lookup\n");
    printf("2. Feistel Round\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        int input, sbox_num;
        printf("Enter 6-bit input (decimal or binary 0b...): ");
        scanf("%i", &input);
        printf("Enter S-box number (1-8): ");
        scanf("%d", &sbox_num);
        printf("S-Box %d output: %d\n", sbox_num, des_sbox(input, sbox_num));
        break;
    }
    case 2:
    {
        uint32_t L, R, K;
        printf("Enter L (hex): ");
        scanf("%x", &L);
        printf("Enter R (hex): ");
        scanf("%x", &R);
        printf("Enter Key K (hex): ");
        scanf("%x", &K);
        printf("Before: L=0x%08X, R=0x%08X\n", L, R);
        feistel_round(&L, &R, K);
        printf("After:  L=0x%08X, R=0x%08X\n", L, R);
        break;
    }
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
