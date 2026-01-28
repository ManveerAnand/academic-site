// CLASSIC CIPHERS - Affine and Playfair
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Helper: Modular Inverse
long long mod_inverse(long long a, long long n)
{
    for (long long x = 1; x < n; x++)
    {
        if ((a * x) % n == 1)
            return x;
    }
    return -1;
}

// Affine Cipher Encryption: E(x) = (ax + b) mod 26
void affine_encrypt(char *plaintext, int a, int b, char *ciphertext)
{
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        if (isalpha(plaintext[i]))
        {
            int x = toupper(plaintext[i]) - 'A';
            int y = (a * x + b) % 26;
            ciphertext[i] = 'A' + y;
        }
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

// Affine Cipher Decryption: D(y) = a^(-1)(y - b) mod 26
void affine_decrypt(char *ciphertext, int a, int b, char *plaintext)
{
    int a_inv = mod_inverse(a, 26);
    for (int i = 0; ciphertext[i] != '\0'; i++)
    {
        if (isalpha(ciphertext[i]))
        {
            int y = toupper(ciphertext[i]) - 'A';
            int x = (a_inv * (y - b + 26)) % 26;
            plaintext[i] = 'A' + x;
        }
        else
        {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

// Find Affine Decryption Key: (a', b') from (a, b)
void affine_find_decrypt_key(int a, int b, int *a_inv, int *b_inv)
{
    *a_inv = mod_inverse(a, 26);
    *b_inv = (-b * (*a_inv)) % 26;
    if (*b_inv < 0)
        *b_inv += 26;
}

// Playfair Cipher - Generate 5x5 key matrix
void playfair_generate_matrix(char *keyword, char matrix[5][5])
{
    int used[26] = {0};
    int idx = 0;

    // Add keyword
    for (int i = 0; keyword[i] != '\0'; i++)
    {
        char c = toupper(keyword[i]);
        if (c == 'J')
            c = 'I';
        if (isalpha(c) && !used[c - 'A'])
        {
            matrix[idx / 5][idx % 5] = c;
            used[c - 'A'] = 1;
            idx++;
        }
    }

    // Fill rest with alphabet
    for (char c = 'A'; c <= 'Z'; c++)
    {
        if (c == 'J')
            continue;
        if (!used[c - 'A'])
        {
            matrix[idx / 5][idx % 5] = c;
            idx++;
        }
    }
}

// Playfair Encryption
void playfair_encrypt(char *plaintext, char *keyword, char *ciphertext)
{
    char matrix[5][5];
    playfair_generate_matrix(keyword, matrix);

    // Find position in matrix
    int pos[26][2];
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            pos[matrix[i][j] - 'A'][0] = i;
            pos[matrix[i][j] - 'A'][1] = j;
        }
    }

    int idx = 0;
    for (int i = 0; plaintext[i] != '\0'; i += 2)
    {
        char a = toupper(plaintext[i]);
        char b = (plaintext[i + 1] != '\0') ? toupper(plaintext[i + 1]) : 'X';

        if (a == 'J')
            a = 'I';
        if (b == 'J')
            b = 'I';
        if (a == b)
            b = 'X';

        int r1 = pos[a - 'A'][0], c1 = pos[a - 'A'][1];
        int r2 = pos[b - 'A'][0], c2 = pos[b - 'A'][1];

        if (r1 == r2)
        {
            ciphertext[idx++] = matrix[r1][(c1 + 1) % 5];
            ciphertext[idx++] = matrix[r2][(c2 + 1) % 5];
        }
        else if (c1 == c2)
        {
            ciphertext[idx++] = matrix[(r1 + 1) % 5][c1];
            ciphertext[idx++] = matrix[(r2 + 1) % 5][c2];
        }
        else
        {
            ciphertext[idx++] = matrix[r1][c2];
            ciphertext[idx++] = matrix[r2][c1];
        }
    }
    ciphertext[idx] = '\0';
}

// MAIN - MENU DRIVEN
int main()
{
    int choice;
    printf("=== CLASSIC CIPHERS ===\n");
    printf("1. Affine Encrypt\n");
    printf("2. Affine Decrypt\n");
    printf("3. Find Affine Decrypt Key\n");
    printf("4. Playfair Encrypt\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        char plain[100], cipher[100];
        int a, b;
        printf("Enter plaintext (uppercase): ");
        scanf("%s", plain);
        printf("Enter a, b: ");
        scanf("%d %d", &a, &b);
        affine_encrypt(plain, a, b, cipher);
        printf("Encrypted: %s\n", cipher);
        break;
    }
    case 2:
    {
        char cipher[100], plain[100];
        int a, b;
        printf("Enter ciphertext: ");
        scanf("%s", cipher);
        printf("Enter a, b: ");
        scanf("%d %d", &a, &b);
        affine_decrypt(cipher, a, b, plain);
        printf("Decrypted: %s\n", plain);
        break;
    }
    case 3:
    {
        int a, b, a_inv, b_inv;
        printf("Enter encrypt key a, b: ");
        scanf("%d %d", &a, &b);
        affine_find_decrypt_key(a, b, &a_inv, &b_inv);
        printf("Decrypt key: a'=%d, b'=%d\n", a_inv, b_inv);
        break;
    }
    case 4:
    {
        char plain[100], keyword[100], cipher[100];
        printf("Enter plaintext (uppercase, no J): ");
        scanf("%s", plain);
        printf("Enter keyword: ");
        scanf("%s", keyword);
        playfair_encrypt(plain, keyword, cipher);
        printf("Encrypted: %s\n", cipher);
        break;
    }
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
