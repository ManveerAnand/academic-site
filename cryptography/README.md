# 🔐 Cryptography Course Repository

A comprehensive collection of cryptographic algorithms, implementations, and learning resources from my cryptography course.

## 📚 Repository Structure

```
cryptography/
├── assignments/        # Course assignments and solutions
├── labs/              # Laboratory exercises and experiments
├── exam-prep/         # Exam preparation materials and tools
└── resources/         # Additional learning resources
```

## 🎯 Topics Covered

### Classical Cryptography
- Caesar Cipher
- Playfair Cipher
- Vigenère Cipher
- Hill Cipher

### Modern Symmetric Cryptography
- **AES (Advanced Encryption Standard)**
  - SubBytes transformation
  - ShiftRows transformation
  - MixColumns transformation
  - Key expansion
  - GF(2^8) arithmetic
- **DES (Data Encryption Standard)**
  - Feistel structure
  - Round functions
  - Key scheduling

### Public Key Cryptography
- **RSA Algorithm**
  - Key generation
  - Encryption/Decryption
  - Digital signatures
- **Diffie-Hellman Key Exchange**
  - Public key calculation
  - Shared secret generation
- **Elliptic Curve Cryptography (ECC)**
  - Point operations
  - Scalar multiplication

### Number Theory & Mathematical Foundations
- Modular arithmetic
- Modular exponentiation (Square and Multiply)
- Extended Euclidean Algorithm
- Chinese Remainder Theorem (CRT)
- Primality testing
- Finite field arithmetic

## 🛠️ Quick Start

### Prerequisites
- GCC compiler
- Basic understanding of C programming
- Mathematics background (modular arithmetic, number theory)

### Compilation & Execution

#### For Individual Programs:
```bash
# Compile
gcc program_name.c -o program_name

# Run
./program_name
```

#### For Math-Heavy Programs:
```bash
# Compile with math library
gcc number_theory.c -o number_theory -lm

# Run
./number_theory
```

## 📂 Detailed Contents

### Assignments
- **202351080.c** - Main assignment implementation
- **assign1.c** - Assignment 1 solution
- **ASSIGN2.c** - Assignment 2 solution
- **LAB_A3_202351080.c** - Lab Assignment 3

### Labs
- **aes_R1.c** - AES Round 1 implementation
- **fiestel.c** - Feistel network structure
- **fiestel2.c** - Advanced Feistel implementation
- **mixcol.cpp** - MixColumns operation (C++)
- **square_n_mul.c** - Square and Multiply algorithm
- **alpine.c**, **as3.c**, **nipil.c** - Additional cryptographic experiments

### Exam Preparation Tools

Interactive menu-driven programs for quick calculations:

#### 🔷 AES Tool (`aes.c`)
```bash
gcc aes.c -o aes
./aes
```
**Options:**
1. SubBytes transformation
2. MixColumns operation ⭐ (Most Important!)
3. GF(2^8) Multiplication
4. Key Expansion (Round 1)

#### 🔷 RSA Tool (`rsa.c`)
```bash
gcc rsa.c -o rsa
./rsa
```
**Options:**
1. Key Generation (p, q, e → n, φ, d)
2. Encrypt message
3. Decrypt ciphertext
4. Recover p, q from n and φ(n)

#### 🔷 Number Theory Tool (`number_theory.c`)
```bash
gcc number_theory.c -o number_theory -lm
./number_theory
```
**Options:**
1. Modular Exponentiation (a^b mod n)
2. Modular Inverse (a^-1 mod n)
3. GCD calculation
4. Extended GCD
5. Chinese Remainder Theorem (2 equations)
6. Primality Test

#### 🔷 Diffie-Hellman Tool (`diffie_hellman.c`)
```bash
gcc diffie_hellman.c -o diffie_hellman
./diffie_hellman
```
**Options:**
1. Calculate Public Key (g^private mod p)
2. Calculate Shared Secret
3. Full DH Exchange simulation

#### 🔷 Additional Tools
- **des.c** - DES encryption/decryption
- **ecc.c** - Elliptic Curve operations
- **finite_fields.c** - Finite field arithmetic
- **classic_ciphers.c** - Classical cipher implementations
- **converter.c** - Format conversion utilities
- **misc.c** - Miscellaneous crypto utilities

## 🎓 Learning Resources

### Key Concepts

#### AES GF(2^8) Multiplication
The finite field GF(2^8) uses the irreducible polynomial:
```
m(x) = x^8 + x^4 + x^3 + x + 1 (0x11B in hex)
```

#### RSA Key Generation
1. Choose two large primes p and q
2. Compute n = p × q
3. Compute φ(n) = (p-1)(q-1)
4. Choose e such that gcd(e, φ(n)) = 1
5. Compute d ≡ e^(-1) mod φ(n)

#### Diffie-Hellman Protocol
1. Alice and Bob agree on public prime p and generator g
2. Alice chooses private a, computes A = g^a mod p
3. Bob chooses private b, computes B = g^b mod p
4. Alice computes s = B^a mod p
5. Bob computes s = A^b mod p
6. Both have shared secret s

## 🔬 Practice Problems

Common exam/assignment patterns:
1. **AES MixColumns**: Most frequently tested operation
2. **RSA Factor Recovery**: Given n and φ(n), find p and q
3. **Modular Exponentiation**: Use square-and-multiply for efficiency
4. **CRT**: Solve systems of modular equations
5. **GF(2^8) Operations**: Multiplication and inverse in finite fields

## 📝 Notes

- All implementations are for **educational purposes**
- Not optimized for production use
- Focus on understanding algorithms rather than efficiency
- Test with small values before using larger ones
- Refer to `MENU_OPTIONS.txt` in exam-prep for quick reference

## 🤝 Contributing

This is a personal learning repository, but suggestions and corrections are welcome!

## 📄 License

Educational use only. Refer to course materials for specific usage restrictions.

## 🔗 References

- Textbooks and course materials (see course syllabus)
- NIST standards for AES, DES
- RFC specifications for cryptographic protocols

---

**Course**: Cryptography  
**Student ID**: 202351080  
**Last Updated**: January 2026

---

*"In cryptography, we trust mathematics, not luck."* 🔐
