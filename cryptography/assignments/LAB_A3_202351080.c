#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* LAB ASSIGNMENT - III (LAB-A3)
                                                       ------------------------------
    Name - Manveer Anand
    roll no. - 202351080
    section - 1

    In this assignment we have to implement a secure communication protocol using a combination of:
    1. Elliptic Curve Cryptography (ECC) for Diffie-Hellman Key Exchange.
    2. SHA-256 for Key Derivation and Message Authentication Code (MAC).
    3. AES-256 (CBC Mode) for encryption.

    The elliptic curve given is y^2 = x^3 + 33x + 111 over Z_p (p=113).
    The goal is to establish a shared secret between Alice and Bob, derive keys, encrypt a message,
    generate a MAC, and verify everything on the receiver's side.

    NOTE : *commented print statements were used for debugging purposes.
           *Standard implementations for SHA-256 and AES-256 are included to keep the file self-contained.
*/
// ======================================================================================
// ============================  GLOBAL CONSTANTS & VARIABLES  ==========================
// ======================================================================================

/*
    The main function orchestrates the entire protocol.
    High-level workflow:
    1. Curve Setup: Find a generator point alpha on the elliptic curve.
    2. Key Exchange: Alice and Bob exchange public keys to derive shared secret SK.
    3. Key Derivation: Both derive AES keys (KA, KB) from SK using SHA-256.
    4. Encryption: Alice encrypts message MA using AES-256-CBC to get CA.
    5. MAC Generation: Alice computes MAC_A.
    6. Transmission: Simulated transfer of CA, MAC_A, and IV to Bob.
    7. Decryption: Bob decrypts CA to get MB.
    8. Verification: Bob computes MAC_B and verifies integrity.
*/

#define p 113 // prime number
#define a 33  // curve parameter a
#define b 111 // curve parameter b

// ======================================================================================
// ===================  FUNCTION DEFINITIONS and structures =============================
// ======================================================================================

// square and multiply algorithm to compute (x^y)mod p
long long sq_mul(int x, int y)
{
    int result = 1;
    while (y > 0)
    {
        if (y & 1)
        { // if y is odd
            result = (result * x) % p;
        }
        x = (x * x) % p;
        y = y >> 1;
    }
    return result;
}

// 1 and 2
// point finding - to find a point on the curve we will be using brute force method to iterate over all possible x values and check if y^2 = x^3 + ax + b has a solution and return the first point we find.

typedef struct
{
    long long x;
    long long y;
} Point;
Point alpha; // global variable to store the point found on the curve

// ======================================================================================
// ============================     Q1 and Q2     =======================================
// ======================================================================================
Point alpha_finding()
{
    long long x, y;
    long long left, right;

    for (x = 0; x < p; x++)
    {
        right = (((sq_mul(x, 3) + (a * x)) % p) + b) % p; // x^3 + ax + b
        for (y = 0; y < p; y++)
        {
            left = (y * y) % p;

            // checking if y^2 == x^3 + ax + b
            if (left == right)
            {
                printf("Point found on the curve alpha : (%lld, %lld)\n", x, y);
                alpha.x = x;
                alpha.y = y;
                return alpha;
            }
        }
    }
    // Return point at infinity if no point found (shouldn't happen with valid curve parameters)
    Point infinity = {0, 0};
    return infinity;
}

// ======================================================================================
// ============================     Q3 and Q4     =======================================
// ======================================================================================

// Function to compute modular inverse using Fermat's Little Theorem as we are working in a prime field
// we can use the equation a^(p-1) ≡ 1 (mod p) which gives us a^(p-2) ≡ a^(-1) (mod p). [we thus don't need to implement extended euclidean algorithm here.]
long long modInverse(long long n)
{
    return sq_mul(n, p - 2);
}

Point point_double(Point P)
{
    Point R;
    long long lambda;
    // lambda = (3x1^2 + a) / (2y1) mod p
    long long num = (3 * sq_mul(P.x, 2) + a) % p;
    long long den = (2 * P.y) % p;
    lambda = (num * modInverse(den)) % p;

    // x3 = lambda^2 - 2x1
    R.x = (sq_mul(lambda, 2) - 2 * P.x) % p;
    if (R.x < 0)
        R.x += p;

    // y3 = lambda(x1 - x3) - y1
    R.y = (lambda * (P.x - R.x) - P.y) % p;
    if (R.y < 0)
        R.y += p;

    return R;
}

Point point_add(Point P, Point Q)
{
    Point R;
    long long lambda;
    if (P.x == 0 && P.y == 0)
        return Q;
    if (Q.x == 0 && Q.y == 0)
        return P;

    if (P.x == Q.x && P.y == Q.y)
    {
        return point_double(P);
    }
    // lambda = (y2 - y1) / (x2 - x1) mod p
    long long num = (Q.y - P.y) % p;
    if (num < 0)
        num += p;
    long long den = (Q.x - P.x) % p;
    if (den < 0)
        den += p;
    lambda = (num * modInverse(den)) % p;

    // x3 = lambda^2 - x1 - x2
    R.x = (sq_mul(lambda, 2) - P.x - Q.x) % p;
    if (R.x < 0)
        R.x += p;

    // y3 = lambda(x1 - x3) - y1
    R.y = (lambda * (P.x - R.x) - P.y) % p;
    if (R.y < 0)
        R.y += p;

    return R;
}

// Scalar Multiplication using Double-and-Add algorithm's logic is similar to square and multiply where we double the point for each bit and add when the bit is 1. So, calculating 13P is actually: 13P = 8P + 4P + 1P.

Point scalar_mult(int k, Point P)
{
    Point R;
    R.x = 0;
    R.y = 0;     // Point at infinity representation initially
    Point Q = P; // Copy of P to store intermediate results

    while (k > 0)
    {
        if (k % 2 == 1)
        {
            R = point_add(R, Q);
        }
        Q = point_double(Q); // Doubling
        // move to next bit
        k = k >> 1; // equivalent to k /= 2
    }
    return R;
}

// ======================================================================================
// ============================       Q6, Q7 and Q8       ===============================
// ======================================================================================

// ============================  CUSTOM SHA-256 LOGIC  ==================================
// ======================================================================================

// The 64 "Round Constants" (k) - specific random numbers defined by the SHA standard
static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

// Bitwise helper functions Macros they perform bitwise operations used in SHA-256
// ROTRIGHT(a, b) rotates the bits of 'a' to the right by 'b' positions
#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))

// The Logic Gates of SHA-256
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

// ======================================================================================
// ============================  SHA-256 FUNCTIONS  =====================================
// ======================================================================================

// 1. The Engine: Performs the raw SHA-256 mathematical transformation
// SHA-256 Engine (Handles inputs > 64 bytes)
void compute_sha256(const unsigned char *input, size_t len, unsigned char *output_digest)
{
    // Initialize State Variables
    uint32_t H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    // Calculate total padded length
    size_t padded_len = len + 1 + 8;
    padded_len = (padded_len + 63) / 64 * 64; // Round up to nearest 64

    unsigned char *buffer = (unsigned char *)calloc(padded_len, 1);
    memcpy(buffer, input, len);
    buffer[len] = 0x80; // Append '1' bit

    // Append length in bits at the very end (Big Endian)
    unsigned long long bit_len = len * 8;
    for (int i = 0; i < 8; i++)
    {
        buffer[padded_len - 1 - i] = (bit_len >> (i * 8)) & 0xFF;
    }

    // Process blocks of 64 bytes
    uint32_t w[64];
    for (size_t offset = 0; offset < padded_len; offset += 64)
    {
        const unsigned char *chunk = buffer + offset;

        // Prepare message schedule
        for (int i = 0; i < 16; i++)
        {
            w[i] = (chunk[i * 4] << 24) | (chunk[i * 4 + 1] << 16) |
                   (chunk[i * 4 + 2] << 8) | (chunk[i * 4 + 3]);
        }
        for (int i = 16; i < 64; i++)
        {
            w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
        }

        // Initialize working variables
        uint32_t wa = H[0], wb = H[1], wc = H[2], wd = H[3];
        uint32_t we = H[4], wf = H[5], wg = H[6], wh = H[7];

        // Compression loop
        for (int i = 0; i < 64; i++)
        {
            uint32_t temp1 = wh + EP1(we) + CH(we, wf, wg) + k[i] + w[i];
            uint32_t temp2 = EP0(wa) + MAJ(wa, wb, wc);
            wh = wg;
            wg = wf;
            wf = we;
            we = wd + temp1;
            wd = wc;
            wc = wb;
            wb = wa;
            wa = temp1 + temp2;
        }

        // Add to state
        H[0] += wa;
        H[1] += wb;
        H[2] += wc;
        H[3] += wd;
        H[4] += we;
        H[5] += wf;
        H[6] += wg;
        H[7] += wh;
    }

    free(buffer);

    // Output
    for (int i = 0; i < 8; i++)
    {
        output_digest[i * 4] = (H[i] >> 24) & 0xFF;
        output_digest[i * 4 + 1] = (H[i] >> 16) & 0xFF;
        output_digest[i * 4 + 2] = (H[i] >> 8) & 0xFF;
        output_digest[i * 4 + 3] = (H[i]) & 0xFF;
    }
}

// 2. The Wrapper: Converts Point SK -> Bytes -> Calls SHA Engine
void derive_session_key(Point SK, unsigned char *output_key)
{
    unsigned char point_data[16];

    // Flatten the Point (x,y) into a byte array
    // We copy 8 bytes of X, then 8 bytes of Y
    memcpy(point_data, &SK.x, 8);
    memcpy(point_data + 8, &SK.y, 8);

    // Calculate the hash
    compute_sha256(point_data, 16, output_key);
}

// Helper to print bytes in Hex
void print_hex(const char *label, unsigned char *data, int len)
{
    printf("%s", label);
    for (int i = 0; i < len; i++)
    {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

// ======================================================================================
// ============================       AES Q9, Q10       =================================
// ======================================================================================

// AES S-box
static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

static const uint8_t rcon[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// 2.
uint8_t x_mul(uint8_t val)
{
    uint8_t temp = val << 1;
    if (val & 0x80)
        temp ^= 0x1B;
    return temp;
}

uint8_t x_plus_mul(uint8_t val)
{
    return x_mul(val) ^ val;
}

// 3. SubBytes: Substitute every byte using the S-box
void sub_bytes(uint8_t state[4][4])
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            state[r][c] = sbox[state[r][c]];
        }
    }
}

// 4. ShiftRows: Rotate row 1 by 1, row 2 by 2, row 3 by 3
void shift_rows(uint8_t state[4][4])
{
    uint8_t temp;

    // Row 1: Shift left by 1
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    // Row 2: Shift left by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Row 3: Shift left by 3
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

// 5. MixColumns (Your logic, applied to the State)
void mix_columns(uint8_t state[4][4])
{
    uint8_t temp[4];
    for (int col = 0; col < 4; col++)
    {
        // Logic: 2*a0 ^ 3*a1 ^ 1*a2 ^ 1*a3
        // We use the column index 'col'
        for (int r = 0; r < 4; r++)
        {
            temp[r] = x_mul(state[r][col]) ^
                      x_plus_mul(state[(r + 1) % 4][col]) ^
                      state[(r + 2) % 4][col] ^
                      state[(r + 3) % 4][col];
        }
        // Copy back
        for (int r = 0; r < 4; r++)
        {
            state[r][col] = temp[r];
        }
    }
}

// 6. AddRoundKey: XOR the state with the Round Key
void add_round_key(uint8_t state[4][4], const uint8_t *round_key)
{
    for (int c = 0; c < 4; c++)
    {
        for (int r = 0; r < 4; r++)
        {
            // AES keys are stored Column-Major in the schedule
            state[r][c] ^= round_key[c * 4 + r];
        }
    }
}

// Key Expansion for AES-256
// This is strictly defined by the standard.
void key_expansion(const uint8_t *key, uint8_t *round_keys)
{
    uint8_t temp[4];
    int i = 0;

    // Copy original 32-byte key
    while (i < 32)
    {
        round_keys[i] = key[i];
        i++;
    }

    // Generate the rest (up to 240 bytes for 15 round keys)
    while (i < 240)
    {
        // Read previous word into temp
        for (int k = 0; k < 4; k++)
            temp[k] = round_keys[i - 4 + k];

        if (i % 32 == 0)
        {
            // RotWord: Rotate temp left
            uint8_t t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            // SubWord: S-box
            for (int k = 0; k < 4; k++)
                temp[k] = sbox[temp[k]];
            // XOR Rcon
            temp[0] ^= rcon[i / 32];
        }
        else if (i % 32 == 16)
        {
            // Special AES-256 step: SubWord only
            for (int k = 0; k < 4; k++)
                temp[k] = sbox[temp[k]];
        }

        // XOR with word 32 bytes ago
        for (int k = 0; k < 4; k++)
        {
            round_keys[i] = round_keys[i - 32] ^ temp[k];
            i++;
        }
    }
}

// 8. Main Encrypt Block Function
void aes256_encrypt_block(const uint8_t *input, uint8_t *output, const uint8_t *key)
{
    uint8_t state[4][4];
    uint8_t round_keys[240];

    // Prepare Keys
    key_expansion(key, round_keys);

    // Copy Input to State (Column-Major Order for AES standard)
    // Input: 0 1 2 3 ...
    // State:
    //  0  4  8 12
    //  1  5  9 13
    //  2  6 10 14
    //  3  7 11 15
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            state[r][c] = input[r + 4 * c];
        }
    }

    // --- ROUND 0 ---
    add_round_key(state, round_keys); // Use first 16 bytes of key

    // --- ROUNDS 1 to 13 ---
    for (int round = 1; round < 14; round++)
    {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state); // <-- Your function!
        add_round_key(state, round_keys + (round * 16));
    }

    // --- ROUND 14 (Final) ---
    sub_bytes(state);
    shift_rows(state);
    // NO MixColumns in final round
    add_round_key(state, round_keys + (14 * 16));

    // Copy State to Output
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            output[r + 4 * c] = state[r][c];
        }
    }
}

// ======================================================================================
// ===========================  14,15 :AES DECRYPTION ENGINE  ===========================
// ======================================================================================
// NOTE: Decryption requires the Inverse S-Box and Inverse transformations.
// It is mathematically different from Encryption.

// Inverse S-box (RSBOX): Used to reverse the SubBytes step
static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

// Helper: Inverse SubBytes
// Replaces each byte in the state using the Inverse S-Box
void inv_sub_bytes(uint8_t state[4][4])
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            state[r][c] = rsbox[state[r][c]];
    }
}

// Helper: Inverse ShiftRows
// Cyclically shifts rows to the RIGHT (opposite of Encryption which shifts Left)
void inv_shift_rows(uint8_t state[4][4])
{
    uint8_t temp;
    // Row 1: Shift Right by 1
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;
    // Row 2: Shift Right by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    // Row 3: Shift Right by 3
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

/* Inverse MixColumns Logic:
   In AES, the MixColumns matrix M has the property M^4 = Identity.
   Therefore, M^-1 = M^3.
   Running the forward MixColumns 3 times yields the inverse transformation.
   This avoids implementing multiplication by 9, 11, 13, and 14.
*/
void inv_mix_columns(uint8_t state[4][4])
{
    mix_columns(state);
    mix_columns(state);
    mix_columns(state);
}

// AES-256 Decrypt Block Function
// Decrypts a single 16-byte block of ciphertext into plaintext
void aes256_decrypt_block(const uint8_t *input, uint8_t *output, const uint8_t *key)
{
    uint8_t state[4][4];
    uint8_t round_keys[240];

    // 1. Expand the Key (Same as encryption)
    key_expansion(key, round_keys);

    // 2. Copy Input Ciphertext to State Array
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            state[r][c] = input[r + 4 * c];
    }

    // 3. Initial Round: AddRoundKey with the LAST Round Key (Round 14)
    add_round_key(state, round_keys + (14 * 16));

    // 4. Main Rounds: 13 down to 1 (Operations are reversed)
    for (int round = 13; round > 0; round--)
    {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, round_keys + (round * 16));
        inv_mix_columns(state);
    }

    // 5. Final Round (Round 0): No MixColumns
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, round_keys); // Round 0 Key

    // 6. Copy State to Output
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            output[r + 4 * c] = state[r][c];
    }
}

// ======================================================================================
// ============================         MAIN FUNCTION        ============================
// ======================================================================================

int main()
{
    // ----------------------------------------------------------------------------------
    // STEP 1 & 2: Curve Setup and Finding Point Alpha
    // ----------------------------------------------------------------------------------
    alpha = alpha_finding();
    printf("---------------------------------------------------------\n");
    // ----------------------------------------------------------------------------------
    // STEP 3 & 4: Input Private Keys and Generate Public Keys of Alice and Bob and Shared Secret Key SK.
    // ----------------------------------------------------------------------------------

    int na, nb;
    printf("Enter private key of Alice (na) range [1,100]: ");
    scanf("%d", &na);
    printf("Enter private key of Bob (nb) range [1,100]: ");
    scanf("%d", &nb);
    // Generate Public Keys of Alice and Bob:
    Point Pa = scalar_mult(na, alpha); // Public Key of Alice
    Point Pb = scalar_mult(nb, alpha); // Public Key of Bob
    // Generate Shared Secret Key SK:
    Point SK_Alice = scalar_mult(na, Pb); // Alice computes SK
    Point SK_Bob = scalar_mult(nb, Pa);   // Bob computes SK
    printf("Shared Secret Key SK computed by Alice: (%lld, %lld)\n", SK_Alice.x, SK_Alice.y);
    printf("Shared Secret Key SK computed by Bob: (%lld, %lld)\n", SK_Bob.x, SK_Bob.y);
    printf("---------------------------------------------------------\n");
    // ----------------------------------------------------------------------------------
    // STEP 6, 7 & 8: Key Derivation (KA, KB) using SHA-256
    // ----------------------------------------------------------------------------------
    unsigned char KA[32];             // AES Key for Alice
    unsigned char KB[32];             // AES Key for Bob
    derive_session_key(SK_Alice, KA); // Alice derives KA
    derive_session_key(SK_Bob, KB);   // Bob derives KB
    print_hex("KA: ", KA, 32);
    print_hex("KB: ", KB, 32);
    printf("---------------------------------------------------------\n");

    // ----------------------------------------------------------------------------------
    // STEP 9: Input Message MA
    // ----------------------------------------------------------------------------------
    unsigned char MA[32];
    printf("Enter 32 bytes(256 bits) for MA (hex, space separated): ");
    for (int i = 0; i < 32; i++)
    {
        scanf("%2hhx", &MA[i]);
    }
    //----------------------------------------------------------------------------------
    // STEP 10: AES-256-CBC Encryption of MA to get CA
    // ----------------------------------------------------------------------------------
    unsigned char CA[32]; // Ciphertext to store the final encrypted message
    // AES-CBC Encryption (2 blocks of 16 bytes each)
    unsigned char iv[16];
    memset(iv, 0xFF, 16); // Sets all bytes to 11111111
    // First Block
    unsigned char block1[16];
    for (int i = 0; i < 16; i++)
    {
        block1[i] = MA[i] ^ iv[i]; // XOR with IV
    }
    unsigned char enc_block1[16];
    aes256_encrypt_block(block1, enc_block1, KA);
    memcpy(CA, enc_block1, 16);

    // Second Block
    unsigned char block2[16];
    for (int i = 0; i < 16; i++)
    {
        block2[i] = MA[i + 16] ^ enc_block1[i]; // XOR with previous ciphertext block
    }
    unsigned char enc_block2[16];
    aes256_encrypt_block(block2, enc_block2, KA);
    memcpy(CA + 16, enc_block2, 16);

    print_hex("Ciphertext CA: ", CA, 32);

    // ----------------------------------------------------------------------------------
    // STEP 11 and 12: MAC Generation using SHA-256 on (CA || IV)
    // ----------------------------------------------------------------------------------

    unsigned char MAC_A[32];

    // 1. Prepare the XORed Keys
    // The assignment says "1, 2 are 256-bit integers".
    // In a byte array, this means we XOR the last byte with 0x01 and 0x02.
    unsigned char K_xor_1[32];
    unsigned char K_xor_2[32];

    memcpy(K_xor_1, KA, 32);
    memcpy(K_xor_2, KA, 32);

    K_xor_1[31] ^= 0x01;
    K_xor_2[31] ^= 0x02;

    // 2. INNER HASH: SHA-256( (KA ^ 2) || MA )
    // Input size: 32 bytes (Key) + 32 bytes (Message) = 64 bytes
    unsigned char inner_input[64];
    memcpy(inner_input, K_xor_2, 32); // First 32 bytes
    memcpy(inner_input + 32, MA, 32); // Next 32 bytes

    unsigned char inner_hash[32];
    compute_sha256(inner_input, 64, inner_hash);

    // 3. OUTER HASH: SHA-256( (KA ^ 1) || Inner_Hash )
    // Input size: 32 bytes (Key) + 32 bytes (Inner Hash) = 64 bytes
    unsigned char outer_input[64];
    memcpy(outer_input, K_xor_1, 32);         // First 32 bytes
    memcpy(outer_input + 32, inner_hash, 32); // Next 32 bytes

    // Final MAC Computation
    compute_sha256(outer_input, 64, MAC_A);

    // Print MAC
    print_hex("MAC_A: ", MAC_A, 32);
    printf("---------------------------------------------------------\n");

    // ----------------------------------------------------------------------------------
    // STEP 13, 14, 15, 16: Bob's Decryption and Verification
    // ----------------------------------------------------------------------------------
    unsigned char MB[32]; // Bob's Message Buffer
    unsigned char decrypted_block[16];

    // CBC Decryption Logic: Decrypt(Ciphertext) XOR Previous_Ciphertext

    // Block 1: XOR with IV
    aes256_decrypt_block(CA, decrypted_block, KB);
    for (int i = 0; i < 16; i++)
    {
        MB[i] = decrypted_block[i] ^ iv[i];
    }

    // Block 2: XOR with Block 1 of CA (which acts as the IV for the next block)
    aes256_decrypt_block(CA + 16, decrypted_block, KB);
    for (int i = 0; i < 16; i++)
    {
        MB[16 + i] = decrypted_block[i] ^ CA[i];
    }

    print_hex("Decrypted Message MB: ", MB, 32);

    printf("\n--- Step 15: Bob Generates MAC_B ---\n");

    unsigned char MAC_B[32];

    // Bob computes MAC using his Key (KB) and the message he received (MB)
    // We reuse the same Nested Hash logic (HMAC-style)

    // 1. Prepare XOR Keys
    unsigned char KB_xor_1[32];
    unsigned char KB_xor_2[32];
    memcpy(KB_xor_1, KB, 32);
    memcpy(KB_xor_2, KB, 32);
    KB_xor_1[31] ^= 0x01;
    KB_xor_2[31] ^= 0x02;

    // 2. Inner Hash: SHA( (KB^2) || MB )
    unsigned char b_inner_input[64];
    memcpy(b_inner_input, KB_xor_2, 32);
    memcpy(b_inner_input + 32, MB, 32);
    unsigned char b_inner_hash[32];
    compute_sha256(b_inner_input, 64, b_inner_hash);

    // 3. Outer Hash: SHA( (KB^1) || InnerHash )
    unsigned char b_outer_input[64];
    memcpy(b_outer_input, KB_xor_1, 32);
    memcpy(b_outer_input + 32, b_inner_hash, 32);
    compute_sha256(b_outer_input, 64, MAC_B);

    print_hex("MAC_B: ", MAC_B, 32);
    printf("---------------------------------------------------------\n");

    // Compare Memory Blocks
    int keys_match = (memcmp(KA, KB, 32) == 0);
    int msgs_match = (memcmp(MA, MB, 32) == 0);
    int macs_match = (memcmp(MAC_A, MAC_B, 32) == 0);

    if (keys_match && msgs_match && macs_match)
    {
        printf("[SUCCESS] VERIFICATION PASSED!\n");
        printf(" 1. Keys Match (KA == KB)\n");
        printf(" 2. Messages Match (MA == MB)\n");
        printf(" 3. MACs Match (MAC_A == MAC_B)\n");
    }
    else
    {
        printf("[FAILURE] VERIFICATION FAILED.\n");
        if (!keys_match)
            printf(" - Error: Keys do not match.\n");
        if (!msgs_match)
            printf(" - Error: Decryption failed (Messages differ).\n");
        if (!macs_match)
            printf(" - Error: MAC check failed (Integrity compromised).\n");
    }
    printf("---------------------------------------------------------\n");
    return 0;
}

/*
OUTPUT:

Point found on the curve alpha : (0, 26)
---------------------------------------------------------
Enter private key of Alice (na) range [1,100]: 5
Enter private key of Bob (nb) range [1,100]: 7
Shared Secret Key SK computed by Alice: (47, 13)
Shared Secret Key SK computed by Bob: (47, 13)
---------------------------------------------------------
KA: c3 7f d2 2c 51 09 b1 52 45 1a ae cd 1c ec 56 9e 6e c3 e9 84 51 1c 2d bc 21 77 ce e6 42 0f 0e 75
KB: c3 7f d2 2c 51 09 b1 52 45 1a ae cd 1c ec 56 9e 6e c3 e9 84 51 1c 2d bc 21 77 ce e6 42 0f 0e 75
---------------------------------------------------------
Enter 32 bytes(256 bits) for MA (hex, space separated): 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff
Ciphertext CA: fa 69 66 0f 59 2a 7e c3 d9 95 42 49 3c 06 6f 6d c6 6a 02 f7 17 32 46 a1 ba 24 1f ba f0 38 48 36
MAC_A: 8c 61 35 15 0e b7 89 52 be 52 ee 25 25 a5 8c c7 d8 25 1a 5d 1a 9f f9 bd 9b d3 2a f3 42 37 a9 8e
---------------------------------------------------------
Decrypted Message MB: 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff

--- Step 15: Bob Generates MAC_B ---
MAC_B: 8c 61 35 15 0e b7 89 52 be 52 ee 25 25 a5 8c c7 d8 25 1a 5d 1a 9f f9 bd 9b d3 2a f3 42 37 a9 8e
---------------------------------------------------------
[SUCCESS] VERIFICATION PASSED!
 1. Keys Match (KA == KB)
 2. Messages Match (MA == MB)
 3. MACs Match (MAC_A == MAC_B)
---------------------------------------------------------
*/
