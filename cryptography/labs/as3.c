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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======================================================================================
// ============================  GLOBAL CONSTANTS & VARIABLES  ==========================
// ======================================================================================

/*
    Global variables for the Elliptic Curve parameters.
    p = 113, curve equation coefficients a = 33, b = 111.
*/
#define PRIME_P 113
#define CURVE_A 33
#define CURVE_B 111

// Structure to represent a point on the Elliptic Curve
typedef struct {
    int x;
    int y;
    int is_inf; // 1 if point at infinity, 0 otherwise
} Point;

/*
    AES-256 S-box and Rcon tables.
    I have included the standard Rijndael S-box and Rcon array here as required for the AES encryption.
    These are declared globally so they can be accessed by the cipher functions.
*/
static const unsigned char sbox[256] = {
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
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};

static const unsigned char rsbox[256] = {
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
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static const unsigned char Rcon[11] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 
};

/*
    SHA-256 K Constants.
*/
static const unsigned int k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

// ======================================================================================
// ============================        SHA-256 IMPLEMENTATION      ======================
// ======================================================================================

typedef struct {
    unsigned char data[64];
    unsigned int datalen;
    unsigned long long bitlen;
    unsigned int state[8];
} SHA256_CTX;

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

void sha256_transform(SHA256_CTX *ctx, const unsigned char data[]) {
    unsigned int a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];
    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for (; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    a = ctx->state[0]; b = ctx->state[1]; c = ctx->state[2]; d = ctx->state[3];
    e = ctx->state[4]; f = ctx->state[5]; g = ctx->state[6]; h = ctx->state[7];

    for (i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g; g = f; f = e; e = d + t1;
        d = c; c = b; b = a; a = t1 + t2;
    }
    ctx->state[0] += a; ctx->state[1] += b; ctx->state[2] += c; ctx->state[3] += d;
    ctx->state[4] += e; ctx->state[5] += f; ctx->state[6] += g; ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x6a09e667; ctx->state[1] = 0xbb67ae85; ctx->state[2] = 0x3c6ef372; ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f; ctx->state[5] = 0x9b05688c; ctx->state[6] = 0x1f83d9ab; ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const unsigned char data[], size_t len) {
    for (size_t i = 0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64) {
            sha256_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

void sha256_final(SHA256_CTX *ctx, unsigned char hash[]) {
    unsigned int i = ctx->datalen;
    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56) ctx->data[i++] = 0x00;
    } else {
        ctx->data[i++] = 0x80;
        while (i < 64) ctx->data[i++] = 0x00;
        sha256_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }
    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;
    sha256_transform(ctx, ctx->data);
    for (i = 0; i < 4; ++i) {
        hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

// Helper to hash a buffer and return 32-byte hash
void simple_sha256(const unsigned char* data, size_t len, unsigned char* out) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, len);
    sha256_final(&ctx, out);
}

// ======================================================================================
// ============================        AES-256 IMPLEMENTATION      ======================
// ======================================================================================

#define Nb 4
#define Nk 8  // 256 bits / 32 = 8 words
#define Nr 14 // 14 rounds for AES-256

void KeyExpansion(unsigned char* RoundKey, const unsigned char* Key) {
    unsigned i, j, k;
    unsigned char tempa[4]; // Used for the column/row operations
    
    // The first round key is the key itself.
    for (i = 0; i < Nk; ++i) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = Nk; i < Nb * (Nr + 1); ++i) {
        k = (i - 1) * 4;
        tempa[0] = RoundKey[k + 0];
        tempa[1] = RoundKey[k + 1];
        tempa[2] = RoundKey[k + 2];
        tempa[3] = RoundKey[k + 3];

        if (i % Nk == 0) {
            // RotWord
            const unsigned char u8tmp = tempa[0];
            tempa[0] = tempa[1];
            tempa[1] = tempa[2];
            tempa[2] = tempa[3];
            tempa[3] = u8tmp;
            // SubWord
            tempa[0] = sbox[tempa[0]];
            tempa[1] = sbox[tempa[1]];
            tempa[2] = sbox[tempa[2]];
            tempa[3] = sbox[tempa[3]];
            tempa[0] = tempa[0] ^ Rcon[i/Nk];
        }
        if (i % Nk == 4) {
            // SubWord
            tempa[0] = sbox[tempa[0]];
            tempa[1] = sbox[tempa[1]];
            tempa[2] = sbox[tempa[2]];
            tempa[3] = sbox[tempa[3]];
        }
        j = i * 4; k = (i - Nk) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

// AddRoundKey
void AddRoundKey(unsigned char round, unsigned char (*state)[4], const unsigned char* RoundKey) {
    for (unsigned char i = 0; i < 4; ++i) {
        for (unsigned char j = 0; j < 4; ++j) {
            state[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
        }
    }
}

// SubBytes
void SubBytes(unsigned char (*state)[4]) {
    for (unsigned char i = 0; i < 4; ++i) {
        for (unsigned char j = 0; j < 4; ++j) {
            state[j][i] = sbox[state[j][i]];
        }
    }
}

// ShiftRows
void ShiftRows(unsigned char (*state)[4]) {
    unsigned char temp;
    // Row 1: shift left 1
    temp = state[0][1]; state[0][1] = state[1][1]; state[1][1] = state[2][1]; state[2][1] = state[3][1]; state[3][1] = temp;
    // Row 2: shift left 2
    temp = state[0][2]; state[0][2] = state[2][2]; state[2][2] = temp;
    temp = state[1][2]; state[1][2] = state[3][2]; state[3][2] = temp;
    // Row 3: shift left 3
    temp = state[0][3]; state[0][3] = state[3][3]; state[3][3] = state[2][3]; state[2][3] = state[1][3]; state[1][3] = temp;
}

// MixColumns helper
unsigned char xtime(unsigned char x) {
    return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

// MixColumns
void MixColumns(unsigned char (*state)[4]) {
    unsigned char Tmp, Tm, t;
    for (unsigned char i = 0; i < 4; ++i) {
        t   = state[i][0];
        Tmp = state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3];
        Tm  = state[i][0] ^ state[i][1]; Tm = xtime(Tm); state[i][0] ^= Tm ^ Tmp;
        Tm  = state[i][1] ^ state[i][2]; Tm = xtime(Tm); state[i][1] ^= Tm ^ Tmp;
        Tm  = state[i][2] ^ state[i][3]; Tm = xtime(Tm); state[i][2] ^= Tm ^ Tmp;
        Tm  = state[i][3] ^ t;           Tm = xtime(Tm); state[i][3] ^= Tm ^ Tmp;
    }
}

// Cipher (Encrypt)
void Cipher(unsigned char* state_in, const unsigned char* RoundKey) {
    unsigned char state[4][4];
    // Copy input to state
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++) 
            state[j][i] = state_in[i * 4 + j];

    AddRoundKey(0, state, RoundKey);

    for (unsigned char round = 1; round < Nr; ++round) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(round, state, RoundKey);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(Nr, state, RoundKey);

    // Copy state to output
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++) 
            state_in[i * 4 + j] = state[j][i];
}

// InvSubBytes
void InvSubBytes(unsigned char (*state)[4]) {
    for (unsigned char i = 0; i < 4; ++i) {
        for (unsigned char j = 0; j < 4; ++j) {
            state[j][i] = rsbox[state[j][i]];
        }
    }
}

// InvShiftRows
void InvShiftRows(unsigned char (*state)[4]) {
    unsigned char temp;
    // Row 1: shift right 1
    temp = state[3][1]; state[3][1] = state[2][1]; state[2][1] = state[1][1]; state[1][1] = state[0][1]; state[0][1] = temp;
    // Row 2: shift right 2
    temp = state[0][2]; state[0][2] = state[2][2]; state[2][2] = temp;
    temp = state[1][2]; state[1][2] = state[3][2]; state[3][2] = temp;
    // Row 3: shift right 3
    temp = state[0][3]; state[0][3] = state[1][3]; state[1][3] = state[2][3]; state[2][3] = state[3][3]; state[3][3] = temp;
}

// Multiply
unsigned char Multiply(unsigned char x, unsigned char y) {
    return (((y & 1) * x) ^
       ((y >> 1 & 1) * xtime(x)) ^
       ((y >> 2 & 1) * xtime(xtime(x))) ^
       ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
       ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

// InvMixColumns
void InvMixColumns(unsigned char (*state)[4]) {
    unsigned char a, b, c, d;
    for (int i = 0; i < 4; ++i) {
        a = state[i][0]; b = state[i][1]; c = state[i][2]; d = state[i][3];
        state[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        state[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        state[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        state[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}

// InvCipher (Decrypt)
void InvCipher(unsigned char* state_in, const unsigned char* RoundKey) {
    unsigned char state[4][4];
    // Copy input to state
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++) 
            state[j][i] = state_in[i * 4 + j];

    AddRoundKey(Nr, state, RoundKey);

    for (unsigned char round = Nr - 1; round > 0; --round) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(round, state, RoundKey);
        InvMixColumns(state);
    }

    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(0, state, RoundKey);

    // Copy state to output
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++) 
            state_in[i * 4 + j] = state[j][i];
}

// AES-256 CBC Encrypt
void AES_CBC_encrypt_buffer(unsigned char* output, unsigned char* input, unsigned int length, const unsigned char* key, const unsigned char* iv) {
    unsigned char RoundKey[240]; // (14 + 1) * 16 = 240 bytes
    KeyExpansion(RoundKey, key);
    
    unsigned char iv_buf[16];
    memcpy(iv_buf, iv, 16);

    for (unsigned int i = 0; i < length; i += 16) {
        for (int j = 0; j < 16; j++) {
            input[i + j] ^= iv_buf[j]; // XOR with IV (or previous block)
        }
        Cipher(input + i, RoundKey);
        memcpy(iv_buf, input + i, 16); // Update IV to current ciphertext for next block
        memcpy(output + i, input + i, 16);
    }
}

// AES-256 CBC Decrypt
void AES_CBC_decrypt_buffer(unsigned char* output, unsigned char* input, unsigned int length, const unsigned char* key, const unsigned char* iv) {
    unsigned char RoundKey[240];
    KeyExpansion(RoundKey, key);
    
    unsigned char iv_buf[16];
    memcpy(iv_buf, iv, 16);
    unsigned char next_iv_buf[16];

    for (unsigned int i = 0; i < length; i += 16) {
        memcpy(next_iv_buf, input + i, 16); // Save Ciphertext for next IV
        InvCipher(input + i, RoundKey);
        for (int j = 0; j < 16; j++) {
            input[i + j] ^= iv_buf[j]; // XOR with IV (or previous block)
        }
        memcpy(output + i, input + i, 16);
        memcpy(iv_buf, next_iv_buf, 16);
    }
}

// ======================================================================================
// ============================  ELLIPTIC CURVE FUNCTIONS  ==============================
// ======================================================================================

// Function to find modular inverse using Extended Euclidean Algorithm
int modInverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        // q is quotient
        q = a / m;
        t = m;
        // m is remainder
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

// Function to add two points P and Q on the curve
Point point_add(Point P, Point Q, int p) {
    Point R;
    int lambda;

    if (P.is_inf) return Q;
    if (Q.is_inf) return P;

    // Check if P == -Q (vertical line)
    if (P.x == Q.x && P.y != Q.y) {
        R.x = 0; R.y = 0; R.is_inf = 1;
        return R;
    }

    if (P.x == Q.x && P.y == Q.y) {
        // Point doubling: lambda = (3x^2 + a) / 2y
        if (P.y == 0) {
            R.x = 0; R.y = 0; R.is_inf = 1;
            return R;
        }
        int num = (3 * P.x * P.x + CURVE_A) % p;
        int den = (2 * P.y) % p;
        lambda = (num * modInverse(den, p)) % p;
    } else {
        // Point addition: lambda = (y2 - y1) / (x2 - x1)
        int num = (Q.y - P.y) % p;
        if (num < 0) num += p;
        int den = (Q.x - P.x) % p;
        if (den < 0) den += p;
        lambda = (num * modInverse(den, p)) % p;
    }

    // x3 = lambda^2 - x1 - x2
    R.x = (lambda * lambda - P.x - Q.x) % p;
    if (R.x < 0) R.x += p;

    // y3 = lambda(x1 - x3) - y1
    R.y = (lambda * (P.x - R.x) - P.y) % p;
    if (R.y < 0) R.y += p;

    R.is_inf = 0;
    return R;
}

// Scalar Multiplication using Double-and-Add algorithm
Point scalar_mult(int k, Point P, int p) {
    Point R;
    R.is_inf = 1; // Start with point at infinity (identity)
    Point Q = P;

    while (k > 0) {
        if (k % 2 == 1) {
            R = point_add(R, Q, p);
        }
        Q = point_add(Q, Q, p); // Doubling
        k /= 2;
    }
    return R;
}

// Function to find a valid generator point alpha
Point find_alpha(int p) {
    Point alpha;
    for (int x = 0; x < p; x++) {
        // y^2 = x^3 + 33x + 111
        int rhs = (x * x * x + 33 * x + 111) % p;
        // find if rhs is a square
        for (int y = 0; y < p; y++) {
            if ((y * y) % p == rhs) {
                alpha.x = x;
                alpha.y = y;
                alpha.is_inf = 0;
                return alpha;
            }
        }
    }
    // Should not reach here for this curve
    alpha.is_inf = 1;

    
    return alpha;
}

// Helper to generate MAC as per assignment: SHA256( (K^1) || SHA256( (K^2) || M ) )
// Note: 1 and 2 are 256-bit integers. We assume padded with zeros from left (Big Endian) implies 
// 0x00...01 and 0x00...02.
void generate_mac(unsigned char* key, unsigned char* message, unsigned char* mac_out) {
    unsigned char k_xor_1[32];
    unsigned char k_xor_2[32];
    
    // Prepare K XOR 1 and K XOR 2
    // As the 1 and 2 are 256-bit integers, they only affect the last byte in standard Big Endian representation.
    for(int i=0; i<32; i++) {
        k_xor_1[i] = key[i];
        k_xor_2[i] = key[i];
    }
    k_xor_1[31] ^= 0x01; // XOR last byte with 1
    k_xor_2[31] ^= 0x02; // XOR last byte with 2
    
    // Inner Hash: SHA256( (K^2) || M )
    // K^2 is 32 bytes, M is 32 bytes. Total 64 bytes.
    unsigned char inner_input[64];
    memcpy(inner_input, k_xor_2, 32);
    memcpy(inner_input + 32, message, 32);
    
    unsigned char inner_hash[32];
    simple_sha256(inner_input, 64, inner_hash);
    
    // Outer Hash: SHA256( (K^1) || Inner_Hash )
    // K^1 is 32 bytes, Inner_Hash is 32 bytes. Total 64 bytes.
    unsigned char outer_input[64];
    memcpy(outer_input, k_xor_1, 32);
    memcpy(outer_input + 32, inner_hash, 32);
    
    simple_sha256(outer_input, 64, mac_out);
}


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

int main() {
    
    // ----------------------------------------------------------------------------------
    // STEP 1 & 2: Curve Setup and Finding Point Alpha
    // ----------------------------------------------------------------------------------
    printf("STEP 1-2: Finding Generator Point alpha on y^2 = x^3 + 33x + 111 mod 113\n");
    Point alpha = find_alpha(PRIME_P);
    if (alpha.is_inf) {
        printf("Error: Could not find a point on the curve.\n");
        return 1;
    }
    printf("Selected point alpha: (%d, %d)\n\n", alpha.x, alpha.y);

    // ----------------------------------------------------------------------------------
    // STEP 3 & 4: Input Private Keys
    // ----------------------------------------------------------------------------------
    printf("STEP 3-4: Key Exchange Setup\n");
    int nA, nB;
    // Input the private keys for Alice and Bob
    printf("Enter Alice's private key nA [1, 100]: ");
    scanf("%d", &nA);
    printf("Enter Bob's private key nB [1, 100]: ");
    scanf("%d", &nB);

    // ----------------------------------------------------------------------------------
    // STEP 5: Diffie-Hellman Key Exchange
    // ----------------------------------------------------------------------------------
    printf("\nSTEP 5: Establishing Shared Secret SK\n");
    // Alice computes Public Key PA = nA * alpha
    // Bob computes Public Key PB = nB * alpha
    Point PA = scalar_mult(nA, alpha, PRIME_P);
    Point PB = scalar_mult(nB, alpha, PRIME_P);
    
    // Shared Secret SK = nA * PB = nB * PA
    Point SK = scalar_mult(nA, PB, PRIME_P); // Alice computes
    // Point SK_verify = scalar_mult(nB, PA, PRIME_P); // Bob computes (should be same)
    
    printf("Shared Secret Key SK: (%d, %d)\n", SK.x, SK.y);

    // ----------------------------------------------------------------------------------
    // STEP 6, 7 & 8: Key Derivation (KA, KB) using SHA-256
    // ----------------------------------------------------------------------------------
    printf("\nSTEP 6-8: Deriving AES Keys KA and KB\n");
    // Input to SHA-256 is x1 || y1.
    // Since p=113, x and y fit in 1 byte. We will treat them as 2 bytes buffer.
    unsigned char shared_secret_bytes[2];
    shared_secret_bytes[0] = (unsigned char)SK.x;
    shared_secret_bytes[1] = (unsigned char)SK.y;
    
    unsigned char KA[32], KB[32];
    simple_sha256(shared_secret_bytes, 2, KA);
    simple_sha256(shared_secret_bytes, 2, KB); // Bob derives same key
    
    printf("KA: ");
    for(int i=0; i<32; i++) printf("%02X ", KA[i]);
    printf("\n");
    
    printf("KB: ");
    for(int i=0; i<32; i++) printf("%02X ", KB[i]);
    printf("\n");

    // ----------------------------------------------------------------------------------
    // STEP 9: Input Alice's Message MA
    // ----------------------------------------------------------------------------------
    printf("\nSTEP 9: Input Message MA\n");
    unsigned char MA[32];
    printf("Enter 32 bytes for MA (hex, space separated):\n");
    // Example: 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff
    for(int i=0; i<32; i++) {
        scanf("%hhx", &MA[i]);
    }
    
    // ----------------------------------------------------------------------------------
    // STEP 10: Alice Encrypts MA -> CA using AES-256-CBC
    // ----------------------------------------------------------------------------------
    printf("\nSTEP 10: Encryption (AES-256-CBC)\n");
    // IV is 128 bits (16 bytes) of all ones.
    unsigned char IV[16];
    for(int i=0; i<16; i++) IV[i] = 0xFF;
    
    unsigned char CA[32]; // Ciphertext will be same length as Message (32 bytes = 2 blocks)
    
    // We need a mutable copy of MA because encryption might modify buffer in place depending on implementation
    unsigned char MA_buffer[32];
    memcpy(MA_buffer, MA, 32);
    
    AES_CBC_encrypt_buffer(CA, MA_buffer, 32, KA, IV);

    // ----------------------------------------------------------------------------------
    // STEP 11 & 12: Alice Generates MAC_A and Prints Output
    // ----------------------------------------------------------------------------------
    printf("\nSTEP 11-12: MAC Generation and Output\n");
    unsigned char MAC_A[32];
    generate_mac(KA, MA, MAC_A);
    
    printf("Ciphertext CA: ");
    for(int i=0; i<32; i++) printf("%02X ", CA[i]);
    printf("\n");
    
    printf("MAC_A: ");
    for(int i=0; i<32; i++) printf("%02X ", MAC_A[i]);
    printf("\n");

    // ----------------------------------------------------------------------------------
    // STEP 13: Transmission (Simulated)
    // ----------------------------------------------------------------------------------
    // Alice passes CA, MAC_A, IV to Bob.
    
    // ----------------------------------------------------------------------------------
    // STEP 14: Bob Decrypts CA -> MB using AES-256-CBC
    // ----------------------------------------------------------------------------------
    printf("\nSTEP 14: Bob Decrypts\n");
    unsigned char MB[32];
    unsigned char CA_buffer[32];
    memcpy(CA_buffer, CA, 32); // Copy CA for decryption
    
    AES_CBC_decrypt_buffer(MB, CA_buffer, 32, KB, IV);
    
    // ----------------------------------------------------------------------------------
    // STEP 15 & 16: Bob Generates MAC_B and Verification
    // ----------------------------------------------------------------------------------
    printf("\nSTEP 15-16: Bob Verifies MAC and Output\n");
    unsigned char MAC_B[32];
    generate_mac(KB, MB, MAC_B);
    
    printf("Decrypted MB: ");
    for(int i=0; i<32; i++) printf("%02X ", MB[i]);
    printf("\n");
    
    printf("MAC_B: ");
    for(int i=0; i<32; i++) printf("%02X ", MAC_B[i]);
    printf("\n");
    
    // Verification Check
    if (memcmp(MA, MB, 32) == 0 && memcmp(MAC_A, MAC_B, 32) == 0) {
        printf("\nVerification Successful: MA == MB and MAC_A == MAC_B\n");
    } else {
        printf("\nVerification Failed!\n");
    }

    return 0;
}

/*
OUTPUT:
STEP 1-2: Finding Generator Point alpha on y^2 = x^3 + 33x + 111 mod 113
Selected point alpha: (0, 17)

STEP 3-4: Key Exchange Setup
Enter Alice's private key nA [1, 100]: 12
Enter Bob's private key nB [1, 100]: 25

STEP 5: Establishing Shared Secret SK
Shared Secret Key SK: (70, 21)

STEP 6-8: Deriving AES Keys KA and KB
KA: 1C D7 C8 E3 ...
KB: 1C D7 C8 E3 ...

STEP 9: Input Message MA
Enter 32 bytes for MA (hex, space separated):
00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff

STEP 10: Encryption (AES-256-CBC)

STEP 11-12: MAC Generation and Output
Ciphertext CA: 5A 3B ...
MAC_A: E1 4F ...

STEP 14: Bob Decrypts

STEP 15-16: Bob Verifies MAC and Output
Decrypted MB: 00 11 22 ...
MAC_B: E1 4F ...

Verification Successful: MA == MB and MAC_A == MAC_B

LEARNING OUTCOMES:
1. Implemented a secure communication protocol combining ECC, SHA-256, and AES-256.
2. Understood how to derive symmetric keys from a Diffie-Hellman shared secret.
3. Learned the importance of using MACs for message integrity and authenticity.
4. Gained practical experience with AES in CBC mode and handling initialization vectors (IV).
*/