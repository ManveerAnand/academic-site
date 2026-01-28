# Exam Preparation Tools

Interactive menu-driven programs designed for quick exam calculations and practice.

## 🎯 Quick Reference Guide

See **MENU_OPTIONS.txt** for complete menu listings and workflows!

## Available Tools

### 1. AES Tool (`aes.c`) ⭐ MOST IMPORTANT

**Compile & Run:**
```bash
gcc aes.c -o aes
./aes
```

**Menu Options:**
1. **SubBytes** - S-box substitution
2. **MixColumns** ⭐⭐⭐ Most frequently tested!
3. **GF(2^8) Multiplication** - Finite field arithmetic
4. **Key Expansion** - First round key generation

**When to use:**
- MixColumns problems (very common in exams!)
- Key schedule questions
- Finite field multiplication

---

### 2. RSA Tool (`rsa.c`)

**Compile & Run:**
```bash
gcc rsa.c -o rsa
./rsa
```

**Menu Options:**
1. **Key Generation** - Given p, q, e → Calculate n, φ(n), d
2. **Encrypt** - Given M, e, n → Calculate C
3. **Decrypt** - Given C, d, n → Calculate M
4. **Recover Primes** - Given n, φ(n) → Find p, q

**When to use:**
- RSA encryption/decryption problems
- Finding private key from public parameters
- Recovering prime factors

---

### 3. Number Theory Tool (`number_theory.c`)

**Compile & Run:**
```bash
gcc number_theory.c -o number_theory -lm
./number_theory
```

**Menu Options:**
1. **Modular Exponentiation** - Compute a^b mod n efficiently
2. **Modular Inverse** - Find a^(-1) mod n using Extended GCD
3. **GCD** - Greatest Common Divisor
4. **Extended GCD** - Find x, y such that ax + by = gcd(a,b)
5. **Chinese Remainder Theorem** - Solve system of 2 congruences
6. **Primality Test** - Check if number is prime

**When to use:**
- Any modular arithmetic problem
- Finding multiplicative inverses
- CRT problems
- Checking if numbers are coprime

---

### 4. Diffie-Hellman Tool (`diffie_hellman.c`)

**Compile & Run:**
```bash
gcc diffie_hellman.c -o diffie_hellman
./diffie_hellman
```

**Menu Options:**
1. **Calculate Public Key** - Given g, private, p → Compute g^private mod p
2. **Calculate Shared Secret** - Given other's public key and your private key
3. **Full DH Exchange** - Simulate complete key exchange

**When to use:**
- DH key exchange problems
- Shared secret calculations
- Understanding the protocol flow

---

### 5. DES Tool (`des.c`)

**Compile & Run:**
```bash
gcc des.c -o des
./des
```

**Features:**
- Complete DES encryption/decryption
- Round-by-round output
- Key schedule generation

---

### 6. ECC Tool (`ecc.c`)

**Compile & Run:**
```bash
gcc ecc.c -o ecc
./ecc
```

**Features:**
- Elliptic curve point operations
- Point addition and doubling
- Scalar multiplication

---

### 7. Finite Fields Tool (`finite_fields.c`)

**Compile & Run:**
```bash
gcc finite_fields.c -o finite_fields
./finite_fields
```

**Features:**
- GF(2^8) operations
- Polynomial arithmetic
- Field element multiplication

---

### 8. Classical Ciphers Tool (`classic_ciphers.c`)

**Compile & Run:**
```bash
gcc classic_ciphers.c -o classic_ciphers
./classic_ciphers
```

**Features:**
- Caesar cipher
- Playfair cipher
- Vigenère cipher
- Hill cipher

---

### 9. Converter Tool (`converter.c`)

**Compile & Run:**
```bash
gcc converter.c -o converter
./converter
```

**Features:**
- Hex ↔ Binary conversion
- ASCII ↔ Hex conversion
- Decimal ↔ Binary conversion

---

### 10. Miscellaneous Tool (`misc.c`)

**Compile & Run:**
```bash
gcc misc.c -o misc
./misc
```

**Features:**
- Various helper functions
- Quick calculations
- Utility operations

---

## 📋 Exam Workflow

1. **Identify problem type** (AES, RSA, Number Theory, etc.)
2. **Compile appropriate tool** (if not already compiled)
3. **Run the executable**
4. **Select menu option**
5. **Enter values from problem**
6. **Get instant answer!**

## 🎓 Most Common Exam Topics

### Very High Priority ⭐⭐⭐
1. **AES MixColumns** - Appears in almost every exam!
2. **RSA Encryption/Decryption** - Core public key concept
3. **Modular Exponentiation** - Needed for RSA and DH
4. **Modular Inverse** - Essential for RSA key generation

### High Priority ⭐⭐
5. **Diffie-Hellman** - Key exchange protocol
6. **Chinese Remainder Theorem** - Common in number theory sections
7. **GCD/Extended GCD** - Foundation for many algorithms
8. **AES Key Expansion** - Understanding round keys

### Medium Priority ⭐
9. **DES Operations** - Understanding Feistel structure
10. **Classical Ciphers** - Usually easier questions
11. **ECC Operations** - Advanced topic
12. **Primality Testing** - Number theory basics

## 💡 Exam Tips

### Before the Exam:
- ✅ Compile all tools in advance
- ✅ Test each tool with sample problems
- ✅ Practice without tools first
- ✅ Know which tool to use for each problem type
- ✅ Review MENU_OPTIONS.txt

### During the Exam:
- 🎯 Read the question carefully
- 🎯 Write down given values
- 🎯 Identify the operation needed
- 🎯 Double-check input to tool
- 🎯 Verify answer makes sense

### Time Management:
- ⏱️ Easy questions first (classical ciphers, basic modular arithmetic)
- ⏱️ Use tools for computational heavy problems
- ⏱️ Leave complex proofs/explanations for last
- ⏱️ Always show your work, even when using tools

## 🔍 Common Pitfalls

❌ **Wrong modulus** - Double check the 'n' value  
❌ **Public vs Private key confusion** - e vs d in RSA  
❌ **Incorrect finite field** - AES uses GF(2^8), not regular arithmetic  
❌ **Off-by-one errors** - Array indexing in S-box lookups  
❌ **Integer overflow** - Use appropriate data types

## 📝 Practice Strategy

1. **Week 1-2:** Implement algorithms from scratch
2. **Week 3:** Practice with tools on past exam problems
3. **Week 4:** Timed practice under exam conditions
4. **Day before exam:** Quick review with MENU_OPTIONS.txt

## 🆘 Quick Debugging

If a tool gives unexpected output:
1. Check input format (hex vs decimal)
2. Verify you're using correct menu option
3. Try a simple known example first
4. Compare intermediate steps with hand calculation

---

**Remember:** These tools are for verification and learning. Understanding the algorithms is more important than just getting answers!

*Good luck! 🍀*
