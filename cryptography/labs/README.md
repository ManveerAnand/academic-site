# Laboratory Exercises

Hands-on implementations and experiments with cryptographic algorithms.

## Files Overview

### AES Implementation
- **aes_R1.c** - AES Round 1 step-by-step implementation
  - SubBytes, ShiftRows, MixColumns operations
  - First round key expansion

### Feistel Networks
- **fiestel.c** - Basic Feistel network structure
- **fiestel2.c** - Advanced Feistel implementation with variations

### MixColumns Operation
- **mixcol.cpp** - Dedicated MixColumns implementation (C++)
  - Matrix multiplication in GF(2^8)
  - Most commonly tested AES operation

### Number Theory
- **square_n_mul.c** - Square and Multiply algorithm
  - Efficient modular exponentiation
  - Essential for RSA and DH

### Additional Experiments
- **alpine.c** - Alpine cipher experimentation
- **as3.c** - Assignment 3 related implementations
- **nipil.c** - Additional cryptographic primitives

## Quick Compilation Guide

### C Files:
```bash
gcc filename.c -o output_name
./output_name
```

### C++ Files (mixcol):
```bash
g++ mixcol.cpp -o mixcol
./mixcol
```

### With Math Library:
```bash
gcc square_n_mul.c -o square_n_mul -lm
./square_n_mul
```

## Lab Objectives

1. **Understand algorithmic flow** - Step through each operation
2. **Debug implementations** - Identify and fix errors
3. **Test with examples** - Verify with known outputs
4. **Optimize code** - Improve efficiency where possible
5. **Document findings** - Comment your code thoroughly

## Common Lab Exercises

### Lab 1: Classical Ciphers
- Implement Caesar, Playfair, Vigenère
- Encrypt and decrypt sample messages

### Lab 2: AES Operations
- SubBytes using S-box
- MixColumns matrix multiplication
- Key expansion for multiple rounds

### Lab 3: Feistel Networks
- Design custom Feistel cipher
- Test with different round functions
- Analyze security properties

### Lab 4: Number Theory
- Implement extended GCD
- Modular inverse calculation
- Chinese Remainder Theorem

## Tips for Success

✅ **Start with small test cases**  
✅ **Verify intermediate results**  
✅ **Use print statements for debugging**  
✅ **Compare with reference implementations**  
✅ **Test edge cases (zero, one, max values)**

## Resources

- Lecture slides for algorithm pseudocode
- Online AES calculators for verification
- Number theory tools for checking modular arithmetic

---

*Remember: Understanding > Memorization*
