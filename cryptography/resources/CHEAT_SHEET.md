# Cryptography Quick Reference Cheat Sheet

## AES (Advanced Encryption Standard)

### Key Parameters
- Block size: 128 bits (16 bytes)
- Key sizes: 128, 192, or 256 bits
- Rounds: 10 (AES-128), 12 (AES-192), 14 (AES-256)

### Operations

#### SubBytes
```
Replace each byte with corresponding S-box value
Input: State matrix (4x4 bytes)
Output: Substituted state matrix
```

#### ShiftRows
```
Row 0: No shift
Row 1: Shift left by 1
Row 2: Shift left by 2
Row 3: Shift left by 3
```

#### MixColumns ⭐ MOST TESTED!
```
Matrix multiplication in GF(2^8):
| 02 03 01 01 |   | s0 |
| 01 02 03 01 | × | s1 |
| 01 01 02 03 |   | s2 |
| 03 01 01 02 |   | s3 |

Irreducible polynomial: m(x) = x^8 + x^4 + x^3 + x + 1 (0x11B)
```

#### GF(2^8) Multiplication Rules
```
× 01: Identity (no change)
× 02: Left shift, XOR with 0x1B if overflow
× 03: (× 02) XOR (× 01)
```

---

## RSA (Rivest-Shamir-Adleman)

### Key Generation
```
1. Choose primes: p, q
2. n = p × q
3. φ(n) = (p - 1)(q - 1)
4. Choose e: gcd(e, φ(n)) = 1, typically e = 65537
5. Find d: d ≡ e^(-1) mod φ(n)

Public key: (e, n)
Private key: (d, n)
```

### Encryption/Decryption
```
Encryption: C ≡ M^e mod n
Decryption: M ≡ C^d mod n
```

### Common Exam Problems
```
1. Given p, q, e → Find n, φ(n), d
2. Given M, e, n → Find C
3. Given C, d, n → Find M
4. Given n, φ(n) → Recover p, q
   Formula: p + q = n - φ(n) + 1
            p × q = n
   Solve quadratic: t^2 - (p+q)t + pq = 0
```

---

## Diffie-Hellman Key Exchange

### Protocol
```
Public: prime p, generator g
Alice: private a, public A = g^a mod p
Bob: private b, public B = g^b mod p
Shared secret: s = B^a mod p = A^b mod p
```

### Security
```
Difficulty: Discrete logarithm problem
Given g, p, A → Finding a is hard
```

---

## Number Theory Essentials

### Modular Exponentiation (Square and Multiply)
```
To compute a^b mod n:
1. Convert b to binary
2. Square for each bit
3. Multiply by a for each '1' bit
4. Take mod n at each step

Example: 7^5 mod 13
5 = 101₂
7^1 = 7
7^2 = 49 ≡ 10 (mod 13)
7^4 = 10^2 ≡ 9 (mod 13)
7^5 = 9 × 7 ≡ 11 (mod 13)
```

### Modular Inverse
```
Find a^(-1) mod n using Extended GCD:
Extended GCD gives: ax + ny = gcd(a,n)
If gcd(a,n) = 1: a^(-1) ≡ x mod n
```

### Extended GCD
```
Extended Euclidean Algorithm finds x, y:
ax + by = gcd(a,b)

Useful for:
- Modular inverse
- Solving linear Diophantine equations
- RSA key generation
```

### Chinese Remainder Theorem (CRT)
```
Solve system:
x ≡ a₁ (mod n₁)
x ≡ a₂ (mod n₂)

Where gcd(n₁, n₂) = 1

Solution:
x = a₁M₁y₁ + a₂M₂y₂ (mod N)
N = n₁ × n₂
M₁ = N/n₁, M₂ = N/n₂
y₁ = M₁^(-1) mod n₁
y₂ = M₂^(-1) mod n₂
```

### GCD (Greatest Common Divisor)
```
Euclidean Algorithm:
gcd(a, b) = gcd(b, a mod b)
Continue until b = 0
```

### Primality Testing
```
Trial Division: Test divisors up to √n
Fermat's Test: If p prime, a^(p-1) ≡ 1 (mod p)
Miller-Rabin: More sophisticated probabilistic test
```

---

## DES (Data Encryption Standard)

### Parameters
```
Block size: 64 bits
Key size: 56 bits (64 with parity)
Rounds: 16
Structure: Feistel network
```

### Feistel Structure
```
Split input into L₀, R₀
For round i:
  L_i = R_(i-1)
  R_i = L_(i-1) XOR f(R_(i-1), K_i)
```

---

## Classical Ciphers

### Caesar Cipher
```
Encryption: C = (P + k) mod 26
Decryption: P = (C - k) mod 26
```

### Vigenère Cipher
```
Key repeats to match plaintext length
C_i = (P_i + K_(i mod len(K))) mod 26
```

### Playfair Cipher
```
1. Create 5×5 key matrix (I/J combined)
2. Split plaintext into digraphs
3. Apply rules for same row/column/rectangle
```

### Hill Cipher
```
Matrix multiplication mod 26
C = K × P mod 26
Decryption needs K^(-1) mod 26
```

---

## Elliptic Curve Cryptography (ECC)

### Curve Equation
```
y^2 = x^3 + ax + b (mod p)
```

### Point Operations
```
Point Addition: P + Q
Point Doubling: P + P = 2P
Scalar Multiplication: kP = P + P + ... (k times)
```

---

## Common Conversions

### Hex ↔ Binary
```
0 = 0000    8 = 1000
1 = 0001    9 = 1001
2 = 0010    A = 1010
3 = 0011    B = 1011
4 = 0100    C = 1100
5 = 0101    D = 1101
6 = 0110    E = 1110
7 = 0111    F = 1111
```

### Powers of 2
```
2^0 = 1      2^8 = 256
2^1 = 2      2^10 = 1024
2^2 = 4      2^16 = 65536
2^3 = 8      2^32 = 4294967296
2^4 = 16     2^64 = 18446744073709551616
2^5 = 32
2^6 = 64
2^7 = 128
```

---

## Exam Formula Sheet

### RSA Recovery Formulas
```
Given n and φ(n), find p and q:
Sum: p + q = n - φ(n) + 1
Product: p × q = n
Solve: t^2 - (sum)t + product = 0
```

### Euler's Totient Function
```
φ(p) = p - 1 (if p prime)
φ(p^k) = p^k - p^(k-1)
φ(mn) = φ(m)φ(n) if gcd(m,n) = 1
φ(pq) = (p-1)(q-1) if p,q prime
```

### Fermat's Little Theorem
```
If p is prime and gcd(a,p) = 1:
a^(p-1) ≡ 1 (mod p)
a^p ≡ a (mod p)
```

### Euler's Theorem
```
If gcd(a,n) = 1:
a^φ(n) ≡ 1 (mod n)
```

---

## Quick Tips

### ✅ Always Remember
- Check if modular inverse exists (gcd must be 1)
- AES uses XOR, not regular addition
- RSA: e for encryption, d for decryption
- In Diffie-Hellman, shared secret = (other's public)^(your private) mod p

### ❌ Common Mistakes
- Confusing public and private keys
- Forgetting to reduce modulo at each step
- Using regular multiplication instead of GF(2^8) in AES
- Wrong irreducible polynomial in finite fields

### 🎯 Exam Strategy
1. MixColumns appears most frequently
2. Practice modular exponentiation by hand
3. Know Extended GCD algorithm
4. Understand RSA factor recovery
5. CRT is usually a 5-mark question

---

**Last Updated:** January 2026  
**For:** Cryptography Course Final Exam
