# Compilation & Execution Guide

Complete guide for compiling and running all cryptography programs.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Quick Start](#quick-start)
3. [Individual Programs](#individual-programs)
4. [Troubleshooting](#troubleshooting)
5. [Platform-Specific Notes](#platform-specific-notes)

---

## Prerequisites

### Required Software
- **GCC Compiler** (for C programs)
- **G++ Compiler** (for C++ programs)
- **Make** (optional, for batch compilation)

### Installation

#### Windows
```bash
# Install MinGW or use WSL
# Or install GCC through MSYS2
```

#### Linux/Unix
```bash
sudo apt-get update
sudo apt-get install build-essential
```

#### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install
```

---

## Quick Start

### Compile All Exam Tools
```bash
# Navigate to exam-prep directory
cd cryptography/exam-prep

# Compile all tools
gcc aes.c -o aes
gcc rsa.c -o rsa
gcc number_theory.c -o number_theory -lm
gcc diffie_hellman.c -o diffie_hellman
gcc des.c -o des
gcc ecc.c -o ecc
gcc finite_fields.c -o finite_fields
gcc classic_ciphers.c -o classic_ciphers
gcc converter.c -o converter
gcc misc.c -o misc
```

### Run a Program
```bash
# Simply execute the compiled binary
./aes          # Linux/macOS
aes.exe        # Windows
```

---

## Individual Programs

### Assignments

#### assign1.c
```bash
cd cryptography/assignments
gcc assign1.c -o assign1
./assign1
```

#### ASSIGN2.c
```bash
cd cryptography/assignments
gcc ASSIGN2.c -o assign2
./assign2
```

#### LAB_A3_202351080.c
```bash
cd cryptography/assignments
gcc LAB_A3_202351080.c -o lab_a3
./lab_a3
```

#### 202351080.c
```bash
cd cryptography/assignments
gcc 202351080.c -o main_assignment
./main_assignment
```

---

### Labs

#### AES Round 1
```bash
cd cryptography/labs
gcc aes_R1.c -o aes_r1
./aes_r1
```

#### Feistel Networks
```bash
cd cryptography/labs

# Basic Feistel
gcc fiestel.c -o fiestel
./fiestel

# Advanced Feistel
gcc fiestel2.c -o fiestel2
./fiestel2
```

#### MixColumns (C++)
```bash
cd cryptography/labs
g++ mixcol.cpp -o mixcol
./mixcol
```

#### Square and Multiply
```bash
cd cryptography/labs
gcc square_n_mul.c -o square_n_mul -lm
./square_n_mul
```

#### Other Labs
```bash
cd cryptography/labs

gcc alpine.c -o alpine
./alpine

gcc as3.c -o as3
./as3

gcc nipil.c -o nipil
./nipil
```

---

### Exam Preparation Tools

#### AES Tool
```bash
cd cryptography/exam-prep
gcc aes.c -o aes
./aes

# Interactive menu will appear
# Choose option 1-4
```

#### RSA Tool
```bash
cd cryptography/exam-prep
gcc rsa.c -o rsa
./rsa
```

#### Number Theory Tool
**Important:** Requires math library!
```bash
cd cryptography/exam-prep
gcc number_theory.c -o number_theory -lm
./number_theory
```

#### Diffie-Hellman Tool
```bash
cd cryptography/exam-prep
gcc diffie_hellman.c -o diffie_hellman
./diffie_hellman
```

#### DES Tool
```bash
cd cryptography/exam-prep
gcc des.c -o des
./des
```

#### ECC Tool
```bash
cd cryptography/exam-prep
gcc ecc.c -o ecc
./ecc
```

#### Finite Fields Tool
```bash
cd cryptography/exam-prep
gcc finite_fields.c -o finite_fields
./finite_fields
```

#### Classical Ciphers Tool
```bash
cd cryptography/exam-prep
gcc classic_ciphers.c -o classic_ciphers
./classic_ciphers
```

#### Converter Tool
```bash
cd cryptography/exam-prep
gcc converter.c -o converter
./converter
```

#### Miscellaneous Tool
```bash
cd cryptography/exam-prep
gcc misc.c -o misc
./misc
```

---

## Compilation Flags Explained

### Basic Compilation
```bash
gcc source.c -o output
```
- `gcc` - GNU C Compiler
- `source.c` - Input source file
- `-o output` - Output executable name

### With Math Library
```bash
gcc source.c -o output -lm
```
- `-lm` - Link math library (for sqrt, pow, etc.)

### With Warnings
```bash
gcc -Wall -Wextra source.c -o output
```
- `-Wall` - Enable all warnings
- `-Wextra` - Extra warnings

### With Debugging
```bash
gcc -g source.c -o output
```
- `-g` - Include debugging information

### Optimization
```bash
gcc -O2 source.c -o output
```
- `-O2` - Optimization level 2 (for speed)

### C++ Compilation
```bash
g++ source.cpp -o output
```
- `g++` - GNU C++ Compiler

---

## Batch Compilation Scripts

### Linux/macOS: compile_all.sh
```bash
#!/bin/bash

# Exam preparation tools
cd cryptography/exam-prep
gcc aes.c -o aes
gcc rsa.c -o rsa
gcc number_theory.c -o number_theory -lm
gcc diffie_hellman.c -o diffie_hellman
gcc des.c -o des
gcc ecc.c -o ecc
gcc finite_fields.c -o finite_fields
gcc classic_ciphers.c -o classic_ciphers
gcc converter.c -o converter
gcc misc.c -o misc

echo "All exam tools compiled successfully!"

# Labs
cd ../labs
gcc aes_R1.c -o aes_r1
gcc fiestel.c -o fiestel
gcc fiestel2.c -o fiestel2
g++ mixcol.cpp -o mixcol
gcc square_n_mul.c -o square_n_mul -lm
gcc alpine.c -o alpine
gcc as3.c -o as3
gcc nipil.c -o nipil

echo "All lab programs compiled successfully!"

# Assignments
cd ../assignments
gcc assign1.c -o assign1
gcc ASSIGN2.c -o assign2
gcc LAB_A3_202351080.c -o lab_a3
gcc 202351080.c -o main_assignment

echo "All assignments compiled successfully!"
```

**Usage:**
```bash
chmod +x compile_all.sh
./compile_all.sh
```

### Windows: compile_all.bat
```batch
@echo off

REM Exam preparation tools
cd cryptography\exam-prep
gcc aes.c -o aes.exe
gcc rsa.c -o rsa.exe
gcc number_theory.c -o number_theory.exe -lm
gcc diffie_hellman.c -o diffie_hellman.exe
gcc des.c -o des.exe
gcc ecc.c -o ecc.exe
gcc finite_fields.c -o finite_fields.exe
gcc classic_ciphers.c -o classic_ciphers.exe
gcc converter.c -o converter.exe
gcc misc.c -o misc.exe

echo All exam tools compiled successfully!

REM Labs
cd ..\labs
gcc aes_R1.c -o aes_r1.exe
gcc fiestel.c -o fiestel.exe
gcc fiestel2.c -o fiestel2.exe
g++ mixcol.cpp -o mixcol.exe
gcc square_n_mul.c -o square_n_mul.exe -lm
gcc alpine.c -o alpine.exe
gcc as3.c -o as3.exe
gcc nipil.c -o nipil.exe

echo All lab programs compiled successfully!

REM Assignments
cd ..\assignments
gcc assign1.c -o assign1.exe
gcc ASSIGN2.c -o assign2.exe
gcc LAB_A3_202351080.c -o lab_a3.exe
gcc 202351080.c -o main_assignment.exe

echo All assignments compiled successfully!

pause
```

**Usage:**
```batch
compile_all.bat
```

---

## Troubleshooting

### Error: "gcc: command not found"
**Solution:** GCC not installed or not in PATH
```bash
# Check if GCC is installed
gcc --version

# Install GCC (Ubuntu/Debian)
sudo apt-get install gcc

# Add to PATH if installed but not found
export PATH=$PATH:/path/to/gcc
```

### Error: "undefined reference to 'sqrt'" or math functions
**Solution:** Add `-lm` flag
```bash
gcc program.c -o program -lm
```

### Error: "permission denied"
**Solution:** Make executable or run with correct permissions
```bash
chmod +x program
./program
```

### Warning: "implicit declaration of function"
**Solution:** Add proper header includes
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
```

### Compilation succeeds but program crashes
**Debugging steps:**
```bash
# Compile with debugging symbols
gcc -g program.c -o program

# Run with debugger (gdb)
gdb ./program

# Inside gdb
run
backtrace  # After crash
```

---

## Platform-Specific Notes

### Windows
- Use `.exe` extension for executables
- Use backslash `\` for paths or forward slash `/` in Git Bash
- Consider using MinGW or WSL for better compatibility

### Linux
- No extension needed for executables
- Use forward slash `/` for paths
- May need `sudo` for system-wide installations

### macOS
- Similar to Linux
- May need Xcode Command Line Tools
- Use forward slash `/` for paths

---

## Performance Tips

### For Large Inputs
```bash
# Compile with optimization
gcc -O3 program.c -o program
```

### For Development
```bash
# Compile with warnings and debugging
gcc -Wall -Wextra -g program.c -o program
```

### For Release
```bash
# Optimize and strip symbols
gcc -O3 -s program.c -o program
```

---

## Additional Resources

- GCC Manual: https://gcc.gnu.org/onlinedocs/
- C Reference: https://en.cppreference.com/
- Debugging with GDB: https://www.gnu.org/software/gdb/documentation/

---

**Quick Reference:**
```bash
# Standard C program
gcc program.c -o program

# With math library
gcc program.c -o program -lm

# C++ program
g++ program.cpp -o program

# With all warnings
gcc -Wall -Wextra program.c -o program

# Run program
./program        # Linux/macOS
program.exe      # Windows
```

---

*Happy Coding! 🚀*
