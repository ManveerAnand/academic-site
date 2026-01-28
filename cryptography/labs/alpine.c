#include <stdio.h>
#include <stdbool.h>
// checking if a is coprime with 26
bool is_coprime(int a)
{
    for (int i = 2; i <= a / 2; i++)
    {
        if (a % i == 0 && 26 % i == 0)
        {
            return false;
        }
    }
    return true;
}

// encryption algorithm
int mod_inverse(int a)
{
    for (int x = 1; x < 26; x++){
        if ((a * x) % 26 == 1){
            return x;
        }
    }
    return -1;
}

void encrypt(char *str, char *encrypted, int a, int b)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        encrypted[i] = ((a * (str[i] - 'A') + b) % 26) + 'A';
    }
}

void decrypt(char *encrypted, char *decrypted, int a, int b)
{
    int inv_a = mod_inverse(a);
    if (inv_a == -1) {
        printf("No modular inverse exists for a = %d\n", a);
        return;
    }
    for (int i = 0; encrypted[i] != '\0'; i++)
    {
        int y = encrypted[i] - 'A';
        int val = inv_a * (y - b);
        val = (val % 26 + 26) % 26;
        decrypted[i] = val + 'A';
    }
}
int main()
{
    // input a string
    char str[100];
    printf("Enter a string: ");
    scanf("%s", str);
    char encrypted[100];
    char decrypted[100];
    int a, b;
    printf("Enter the value of a and b :");
    scanf("%d %d", &a, &b);
    while (!is_coprime(a))
    {
        printf("%d must be co-prime with 26. Please enter a new value for a: ", a);
        scanf("%d", &a);
    }

    encrypt(str, encrypted, a, b);
    printf("Encrypted string: %s\n", encrypted);

    decrypt(encrypted, decrypted, a, b);
    printf("Decrypted string: %s\n", decrypted);
}