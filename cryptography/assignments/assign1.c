
/*                                                          LAB ASSIGNMENT - I
                                                           ---------------------
    Name - Manveer Anand
    roll no. - 202351080
    section - 1

    In this assignment we have to implement a combination of three ciphers:
    Playfair cipher, Affine cipher, and Feistel cipher(part of AES).

    NOTE : *commented print statements were used for debugging purposes.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*

GLOBALLY DEFINED VARIABLES and TABLES

    1. I have included the AES S-box table here as required for the Feistel function.
        This is the standard substitution box used in AES encryption. I have copied this from the Assignment.
    2. I have decided to use a 4x4 Playfair matrix to work with hex nibbles(0-F).
    3. The value of a and b for the affine cipher are fixed as per the assignment requirements.
        a=3 and b=5
*/

// unsigned char is perfect here as we are dealing with values  (0-255)
unsigned char sbox[16][16] = {
    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
    {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
    {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
    {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
    {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
    {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
    {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
    {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
    {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
    {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
    {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
    {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
    {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
    {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
    {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
    {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}};

unsigned char pm[4][4] = {
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF}};

int a = 3, b = 5; // parameters for affine cipher

void playfair_matrix(unsigned char *key)
{
    unsigned char used[16] = {0};
    // adding the key elements to the matrix
    int index = 0;
    for (int i = 0; i < 4; i++)
    {
        unsigned char high = key[i] >> 4;  // first 4 bits 1011 0110 -> 1011
        unsigned char low = key[i] & 0x0F; // last 4 bits 1011 0110 -> 0110
        if (!used[high])
        {
            pm[index / 4][index % 4] = high;
            used[high] = 1;
            index++;
        }
        if (!used[low])
        {
            pm[index / 4][index % 4] = low;
            used[low] = 1;
            index++;
        }
    }

    // adding the remaining elements to the matrix
    for (int i = 0; i < 16; i++)
    {
        if (!used[i])
        {
            pm[index / 4][index % 4] = i;
            used[i] = 1;
            index++;
        }
    }
}

void print_playfair_matrix()
{
    printf("Playfair Matrix:\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%X ", pm[i][j]);
        printf("\n");
    }
    printf("\n");
}

/*
    I have implemented the standard Playfair encryption rules here:
    - Same row: move right (with wrap-around)
    - Same column: move down (with wrap-around)
    - Rectangle: swap columns
*/
unsigned char play_fair_encrypt(unsigned char a, unsigned char b)
{
    int row_a = 0, col_a = 0, row_b = 0, col_b = 0;
    unsigned char c = 0, d = 0;
    // Find positions of a and b in the Playfair matrix
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (pm[i][j] == a)
            {
                row_a = i;
                col_a = j;
            }
            if (pm[i][j] == b)
            {
                row_b = i;
                col_b = j;
            }
        }
    }
    // Apply the Playfair encryption rules
    if (row_a == row_b)
    {
        c = pm[row_a][(col_a + 1) % 4];
        d = pm[row_b][(col_b + 1) % 4];
    }
    else if (col_a == col_b)
    {
        c = pm[(row_a + 1) % 4][col_a];
        d = pm[(row_b + 1) % 4][col_b];
    }
    else
    {
        c = pm[row_a][col_b];
        d = pm[row_b][col_a];
    }
    return (c << 4) | d;
}

unsigned char affine_encrypt(int high, int low)
{
    return ((a * high + b) % 16 << 4) | ((a * low + b) % 16);
}

void print(unsigned char *p)
{
    for (int i = 0; i < 8; i++)
    {
        printf("%02X ", p[i]);
    }
    printf("\n");
}

unsigned char AES_sbox_byte(unsigned char byte)
{
    unsigned char high = byte >> 4;
    unsigned char low = byte & 0x0F;
    return sbox[high][low];
}

bool decryption_check(unsigned char *original, unsigned char *decrypted)
{
    for (int i = 0; i < 8; i++)
    {
        if (original[i] != decrypted[i])
        {
            return false;
        }
    }
    return true;
}

/*
    For decryption, I have reverse the Playfair rules:
    - Same row: move left (with wrap-around)
    - Same column: move up (with wrap-around)
    - Rectangle: swap columns (same as encryption)
*/
unsigned char play_fair_decrypt(unsigned char a, unsigned char b)
{
    int row_a = 0, col_a = 0, row_b = 0, col_b = 0;
    unsigned char c = 0, d = 0;
    // Find positions of a and b in the Playfair matrix
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (pm[i][j] == a)
            {
                row_a = i;
                col_a = j;
            }
            if (pm[i][j] == b)
            {
                row_b = i;
                col_b = j;
            }
        }
    }
    // Apply the Playfair decryption rules
    if (row_a == row_b)
    {
        c = pm[row_a][(col_a + 3) % 4]; // +3 is equivalent to -1 mod 4
        d = pm[row_b][(col_b + 3) % 4];
    }
    else if (col_a == col_b)
    {
        c = pm[(row_a + 3) % 4][col_a];
        d = pm[(row_b + 3) % 4][col_b];
    }
    else
    {
        c = pm[row_a][col_b];
        d = pm[row_b][col_a];
    }
    return (c << 4) | d;
}

/*
    This is my main function where I implement the complete three-layer cipher system.
    I follow the assignment steps exactly:
    1-2: Create Playfair matrix from K1
    3: Get plaintext P
    4: Apply Playfair encryption (P → C1)
    5-6: Apply Affine encryption (C1 → C2)
    7-9: Apply Feistel encryption (C2 → C3)
    Then I reverse the process for decryption verification.
*/
int main()
{

    // STEP 1 and 2:
    unsigned char k1[4];
    printf("K1 (32 bits in hex eg: AB 01 05 FE, space separated):");

    for (int i = 0; i < 4; i++)
    {
        unsigned char temp;
        scanf("%hhx", &temp); // takes only the first 2 hex digits
        k1[i] = temp;
    }
    printf("\nStep 1 and 2:\n");
    printf("Printing the playfair matrix:\n\n");
    playfair_matrix(k1);
    print_playfair_matrix();
    printf("\nStep 3:\n");
    // print k1
    //  printf("K1: ");
    //  for(int i=0;i<4;i++){
    //     printf("%02X ",k1[i]);
    // }
    // ---------------------------------------------------------------------------------------------------------------
    // STEP 3:
    unsigned char p[8];
    printf("Plaintext P (64 bits in hex eg: AB 01 05 FE 12 34 56 78, space separated): ");
    for (int i = 0; i < 8; i++)
    {
        unsigned char temp;
        scanf("%hhx", &temp);
        p[i] = temp;
    }
    // printf("p: ");
    // print(p);
    printf("\n Step 4:\n");
    // ---------------------------------------------------------------------------------------------------------------
    unsigned char c1[8] = {0};
    // STEP 4: Playfair Encryption
    for (int i = 0; i < 8; i++)
    {
        unsigned char a = p[i] >> 4;   // High nibble
        unsigned char b = p[i] & 0x0F; // Low nibble
        c1[i] = play_fair_encrypt(a, b);
    }
    printf("Ciphertext C1 after Playfair encryption: ");
    print(c1);
    // ----------------------------------------------------------------------------------------------------------------
    printf("\nStep 5 and 6:\n");
    // STEP 5 and 6: Affine Encryption
    unsigned char c2[8] = {0};
    for (int i = 0; i < 8; i++)
    {
        // affine encryption
        unsigned char high = c1[i] >> 4;
        unsigned char low = c1[i] & 0x0F;
        c2[i] = affine_encrypt(high, low);
    }
    printf("Ciphertext C2 after Affine encryption: ");
    print(c2);
    printf("\nstep 7,8 and 9:\n");
    // -------------------------------------------------------------------------------------------------------------------
    // STEP 7,8 and 9: AES S box Encryption:

    unsigned char k3[4];
    unsigned char c3[8] = {0};
    printf("K3 (32 bits in hex eg: AB 01 05 FE, space separated): ");
    for (int i = 0; i < 4; i++)
    {
        unsigned char temp;
        scanf("%hhx", &temp); // takes only the first 2 hex digits
        k3[i] = temp;
    }
    // determine c3[0..3] or L1 = R0
    for (int i = 0; i < 4; i++)
    {
        c3[i] = c2[i + 4];
    }
    // determine c3[4..7] or R1 = L0 XOR S_box(R0+K3)
    for (int i = 0; i < 4; i++)
    {
        c3[i + 4] = c2[i + 4] ^ k3[i];        // R0 XOR K3
        c3[i + 4] = AES_sbox_byte(c3[i + 4]); // S_box(R0 XOR K3)
        c3[i + 4] ^= c2[i];                   // L0 XOR S_box(R0 XOR K3)
    }
    printf("\nCiphertext C3 after Sbox encryption: ");
    print(c3);
    //--------------------------------------------------------------------------------------------------------------
    // DECRYPTION logic starting from C3

    unsigned char c3_decrypt[8] = {0};
    for (int i = 0; i < 4; i++)
    {
        c3_decrypt[i + 4] = c3[i]; // L1 = R0
    }
    // we now have R0 in c3_decrypt[4..7] and the key K3
    for (int i = 0; i < 4; i++)
    {
        c3_decrypt[i] = c3_decrypt[i + 4] ^ k3[i];    // R0 XOR K3
        c3_decrypt[i] = AES_sbox_byte(c3_decrypt[i]); // S_box(R0 XOR K3)
        c3_decrypt[i] ^= c3[i + 4];                   // L0 XOR S_box(R0 XOR K3)
    }

    printf("\n DECRYPTED C3: ");
    print(c3_decrypt);

    // -----------------------------------------------------------------------------------------------------------------
    // Affine Decryption
    unsigned char c2_decrypt[8] = {0};
    for (int i = 0; i < 8; i++)
    {
        unsigned char high = c3_decrypt[i] >> 4;
        unsigned char low = c3_decrypt[i] & 0x0F;
        high = (11 * (high - 5 + 16)) % 16; // multiplicative inverse of 3 mod 16 is 11 as the values were fixed in the start we didn't use euclid's algorithm
        low = (11 * (low - 5 + 16)) % 16;
        c2_decrypt[i] = (high << 4) | low;
    }
    printf("\n DECRYPTED C2: ");
    print(c2_decrypt);

    // ----------------------------------------------------------------------------------------------------------------
    // Playfair Decryption
    unsigned char c1_decrypt[8] = {0};
    for (int i = 0; i < 8; i++)
    {
        unsigned char a = c2_decrypt[i] >> 4;
        unsigned char b = c2_decrypt[i] & 0x0F;
        c1_decrypt[i] = play_fair_decrypt(a, b);
    }
    printf("\n DECRYPTED C1: ");
    print(c1_decrypt);

    if (decryption_check(c2, c3_decrypt))
    {
        printf("\nC2 and DECRYPTED C3 are same, decryption of cipher text C3 is successful\n");
    }
    if (decryption_check(c1, c2_decrypt))
    {
        printf("C1 and DECRYPTED C2 are same, decryption of cipher text C2 is successful\n");
    }
    if (decryption_check(p, c1_decrypt))
    {
        printf(" P and DECRYPTED C1 are same, decryption of cipher text C1 is successful\n");
        printf("\n----------------------Overall decryption successful, original plaintext P is recovered--------------------\n");
    }
    // -------------------------------------------------------------------------------------------------------------------------
    return 0;
}

/*
OUTPUT:
K1 (32 bits in hex eg: AB 01 05 FE, space separated):AB 01 05 FE

Step 1 and 2:
Printing the playfair matrix:

Playfair Matrix:
A B 0 1
5 F E 2
3 4 6 7
8 9 C D


Step 3:
Plaintext P (64 bits in hex eg: AB 01 05 FE 12 34 56 78, space separated): AB 01 05 FE 12 34 56 78

 Step 4:
Ciphertext C1 after Playfair encryption: B0 1A AE E2 27 46 E3 3D

Step 5 and 6:
Ciphertext C2 after Affine encryption: 65 83 3F FB BA 17 FE EC

step 7,8 and 9:
K3 (32 bits in hex eg: AB 01 05 FE, space separated): AB 01 05 FE

Ciphertext C3 after Sbox encryption: BA 17 FE EC E7 C4 30 32

 DECRYPTED C3: 65 83 3F FB BA 17 FE EC

 DECRYPTED C2: B0 1A AE E2 27 46 E3 3D

 DECRYPTED C1: AB 01 05 FE 12 34 56 78

C2 and DECRYPTED C3 are same, decryption of cipher text C3 is successful
C1 and DECRYPTED C2 are same, decryption of cipher text C2 is successful
 P and DECRYPTED C1 are same, decryption of cipher text C1 is successful

----------------------Overall decryption successful, original plaintext P is recovered--------------------

LEARNING OUTCOMES:
1. Dealing with bytes and nibbles (4 bits) is easier in C using bitwise operations.
2. Careful handling of hex input and output is crucial for correctness.
*/