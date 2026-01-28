#include <stdio.h>
#include <stdint.h>

#define ROUNDS 1

void make_round_keys(uint32_t key, uint32_t round_keys[ROUNDS])
{
    round_keys[0] = key;
}

uint32_t fiestel(uint32_t r, uint32_t k)
{
    uint32_t temp = r ^ k;
    return temp * temp;
}

uint32_t encrypt(uint32_t plaintext, uint32_t round_keys[ROUNDS])
{
    uint16_t left = plaintext >> 16;
    uint16_t right = plaintext & 0xFFFF; 
    uint16_t old_left = left;
    left = right;
    uint32_t f_result = fiestel(right, round_keys[0]);
    right = old_left ^ (f_result & 0xFFFF);
    return ((uint32_t)left << 16) | right;
}

uint32_t decrypt(uint32_t ciphertext, uint32_t round_keys[ROUNDS])
{
    uint16_t left = ciphertext >> 16;
    uint16_t right = ciphertext & 0xFFFF;

    uint16_t old_right = right;
    right = left;
    uint32_t f_result = fiestel(left, round_keys[0]);
    left = old_right ^ (f_result & 0xFFFF);

    return ((uint32_t)left << 16) | right;
}

int main()
{
    uint32_t plaintext = 0xDEADBEEF;
    uint32_t key = 0x12345678;

    printf("=== Simple Feistel Cipher ===\n");
    printf("Plaintext: 0x%08X\n", plaintext);
    printf("Key:       0x%08X\n\n", key);

    uint32_t round_keys[ROUNDS];
    make_round_keys(key, round_keys);

    uint32_t ciphertext = encrypt(plaintext, round_keys);
    printf("Encrypted: 0x%08X\n", ciphertext);

    uint32_t decrypted = decrypt(ciphertext, round_keys);
    printf("Decrypted: 0x%08X\n\n", decrypted);

    if (decrypted == plaintext)
    {
        printf("SUCCESS! Decryption worked correctly\n");
    }
    else
    {
        printf("FAILED! Something went wrong \n");
    }
    return 0;
}
