// G = Zp* p is a large prime no. 
// g = 3 is a generator of G
// Ka = g^a mod p  ---> public key of alice
// Kb = g^b mod p  ---> public key of bob
// k = (Kb)^a mod p = (Ka)^b mod p
// a is alice's private key and b is bob's private key4
//A common practice is to use a safe prime, which is a prime number p of the form p=2q+1, where q is also a prime.
// 1<a<p−1
// 1<b<p−1

#include <stdio.h>


int p = 27443;
// square and multiply algorithm to compute (x^y)mod p
int pow(int x, int y){
    int result = 1;
    while(y > 0){
        if(y & 1){ // if y is odd
            result = (result * x) % p;
        }
        x = (x * x) % p; f
        y = y >> 1;
    }
    return result;
}

int main(){
    int g = 3;
    // input a and b
    int a, b;
    printf("Enter private key of Alice (a): ");
    scanf("%d", &a);
    printf("Enter private key of Bob (b): ");
    scanf("%d", &b);

// testing the pow fn 
int pow_test = pow(3, 2);
printf("3^2 mod %d = %d\n", p, pow_test);

    int Ka = pow(g, a); // public key of alice
    int Kb = pow(g, b); // public key of bob
    printf("Public key of Alice (Ka): %d\n", Ka);
    printf("Public key of Bob (Kb): %d\n", Kb);

    int k1 = pow(Kb, a); // shared secret key computed by alice
    int k2 = pow(Ka, b); // shared secret key computed by bob
    printf("Shared secret key computed by Alice: %d\n", k1);
    printf("Shared secret key computed by Bob: %d\n", k2);
}

