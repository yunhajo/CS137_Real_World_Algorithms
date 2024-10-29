# Project Repository: SSH Keygen, ZIP Compression, and JPEG Encoding

This repository features three core projects: **SSH Key Generation**, **ZIP Compression and Decompression**, and **JPEG Image Compression and Decompression**. Each project demonstrates foundational algorithms in cryptography, file compression, and image processing.

## Projects Overview

### 1. SSH Key Generation (RSA)
The **SSH Key Generation** project implements the RSA algorithm for generating public and private key pairs. Since C has limitations with handling large integers, this project includes a custom-built multiple-precision integer (bigint) library, enabling essential operations like modular exponentiation and GCD calculations. The generated keys are suitable for use with SSH protocols.

### 2. ZIP Compression and Decompression
The **ZIP Compression** project implements compression and decompression utilities inspired by the ZIP file format. Using **LZ77 compression** and **Huffman encoding**, the utility compresses data efficiently by identifying repetitive patterns and assigning shorter codes to frequent sequences. This project demonstrates foundational data compression techniques.

### 3. JPEG Image Compression
The **JPEG Image Compression** project is a lossy compression tool for reducing image file sizes while maintaining visual quality. It follows the JPEG standard with steps like **Discrete Cosine Transform (DCT)**, quantization, and **Huffman encoding**. The project supports conversion from PPM to JPEG formats and back, demonstrating key image compression techniques used in digital media.

## Getting Started

Clone the repository and navigate to each project directory for specific build and run instructions:
```bash
git clone https://github.com/yunhajo/CS137_Real_World_Algorithms.git
```
For detailed usage and additional project information, see the README file within each project folder.