# Project: SSH (Keygen)

This repository implements an RSA key generation algorithm used in SSH protocols. It includes two main components:
1. **Bigint Library**: A custom library for handling large integers, as standard C does not support integers larger than 64 bits.
2. **Keygen**: An RSA key generator that uses the bigint library to create secure SSH keys.

## Introduction

This project implemented RSA key generation for SSH protocols in C, which requires handling extremely large numbers beyond the 64-bit limit of standard C data types. To address this, we built a custom `bigint` library that allows for multiple-precision integer arithmetic. This library is used in the RSA key generator, enabling secure key creation for SSH communications.

## Bigint Library

The `bigint` library is designed to handle arbitrarily large integers through custom data structures and algorithms. It enables various arithmetic and modular operations required in cryptographic applications, particularly for RSA key generation. 

### Supported Operations

1. **Addition and Subtraction**: Basic arithmetic operations for multi-precision integers.
2. **Comparison**: To determine the relation (greater, less, equal) between two large numbers.
3. **Multiplication and Division**: Enables the bigint structure to perform multiplication and division, including by single-digit values.
4. **Greatest Common Divisor (GCD)**: A crucial operation for finding coprime numbers in RSA key generation.
5. **Inverse Modulo**: Used in calculating modular inverses, critical for the decryption process in RSA.
6. **Exponential Modulo**: Computes the exponentiation of large integers under a modulus, using efficient methods like *repeated squaring* and *Montgomery multiplication* for improved performance.

The bigint library supports these operations with efficiency and precision, enabling the keygen tool to manage the large numbers required in RSA encryption.

## RSA Algorithm

RSA (Rivest–Shamir–Adleman) is a widely used public-key cryptographic algorithm, essential for securing sensitive data during transmission. RSA relies on the mathematical difficulty of factoring large composite numbers, making it suitable for key exchange and encryption in SSH protocols.

### RSA Key Generation

An RSA key pair consists of a private key and a public key, with the following components:
- **Private Key**: (𝑛, 𝑝, 𝑞, 𝑒, 𝑑)
- **Public Key**: (𝑛, 𝑒)

The values in the keys have the following properties:
- 𝑛 = 𝑝𝑞
- 𝑒𝑑 ≡ 1 (mod 𝜙(𝑛)), where `𝜙(𝑛)` is the Euler’s totient function.
- gcd(𝑒, 𝜙(𝑛)) = 1 (to ensure that a multiplicative inverse for 𝑒 exists, allowing us to compute 𝑑).
  
The private exponent 𝑑 is calculated as 𝑒⁻¹ mod lcm(𝑝-1, 𝑞-1), which provides a slightly more optimal value for 𝑑 than using 𝜙(𝑛).

To ensure the security of the keys:
1. We enforce 𝑑 > 2ᵇⁱᵗˢ/2 to avoid small 𝑑 attacks.
2. We also enforce |𝑝 - 𝑞| > 2ᵇⁱᵗˢ/2 - 100 to prevent Fermat's factorization attack.

RSA key generation in this implementation involves generating random candidate values for 𝑝 and 𝑞 until they satisfy these conditions. 

### Use in SSH Protocols

In SSH, RSA keys authenticate clients and servers without the need to transmit passwords. RSA key pairs can be used to:
- Encrypt a message (using the public key) so only the holder of the private key can decrypt it.
- Sign a message (using the private key) so anyone with the public key can verify its authenticity.

## Building the Project with Makefile

This repository includes a `Makefile` to automate the build process, with several predefined targets. Here is an explanation of the main targets:

- **`bin/keygen`**: Builds the RSA key generator executable, which depends on the `bigint` library and other required object files.
- **`bin/libbigint.a`**: Creates a static library for bigint operations by archiving object files.
- **Object File Compilation (`out/%.o`)**: Compiles source files from `src-given` and `src` directories into object files, which are then linked to build the final executables.

### Usage of Makefile Targets

To build the RSA key generator, run the following command:
```bash
make bin/keygen
```

This command will:

- Compile source files from both src and src-given directories into the out directory.
- Archive the compiled bigint files into bin/libbigint.a.
- Link all necessary object files to produce the bin/keygen executable.

To clean up the generated files:
```bash
make clean
```

### Using the Keygen Executable
To generate an RSA key pair, use the keygen executable as follows:

```bash
./bin/keygen (number of bits) outputfile
```
- (number of bits): The bit length of the RSA key to be generated (e.g., 1024, 2048).
- outputfile: The name of the file to store the private key.

This command will:

- Write the RSA private key to outputfile.
- Write the RSA public key to a new file named outputfile.pub.