# Project: Zip Compression and Decompression

This project implements foundational algorithms for Zip file compression, including `myzip`, `myunzip`, and utilities for Huffman and LZ77 compression. The repository contains the following files:

## Files and Their Functionality
- **myzip0** and **myunzip0**: Implements a basic form of Zip compression (method 0), which stores files without compression.
- **inflate**: Decompresses files in Zip format using Huffman codes.
- **huffman**: Compresses a file using Huffman encoding and LZ77.
- **lz77**: Compresses files using the LZ77 algorithm.
- **deflate8** and **inflate8**: Performs deflation and inflation for files, supporting `myzip` and `myunzip`.
- **myzip** and **myunzip**: Compress and decompress files to and from the Zip format.

## Zip Compression Techniques
Zip files commonly use various compression techniques, including **Huffman Encoding** and **LZ77 Compression**:

- **Huffman Encoding**: A variable-length encoding technique that assigns shorter codes to more frequent data values, enabling efficient compression. It is often combined with LZ77 in Zip files to improve compression.
  
- **LZ77 Compression**: A lossless algorithm that reduces file size by replacing repeated sequences with pointers to previous occurrences. This technique is effective for data with recurring patterns and is often used in the Deflate algorithm alongside Huffman encoding.

For more information, refer to this link: https://www.ietf.org/rfc/rfc1951.txt

## Usage

To use the utilities for compressing and decompressing files:

### myzip and myunzip
These commands compress and decompress files using `deflate8` and `inflate8`.

```bash
# Compress a file
bin/myzip input_filename output_zipname

# Decompress a file
bin/myunzip input_zipped_file
```

### Inflate
Used to decompress files that were compressed with Huffman codes.

```bash
bin/inflate file.deflate
# Output: file (without deflate)
```

### Huffman
Compresses a file using Huffman encoding and LZ77.

```bash
bin/huffman file
# Output: file.deflate
```

## Building the Project with Makefile

This project includes a `Makefile` to streamline the build process. The available targets are:

- **bin/myunzip**: Builds the `myunzip` utility for decompressing files.
- **bin/myzip**: Builds the `myzip` utility for compressing files.
- **bin/mystats**: Example target (if added) for other utilities.
- **Pattern Rule (`bin/%`)**: Builds any `.cpp` file in the `src` directory to an executable in `bin/`.

### Building Targets

To build all executables at once:
```bash
make
```

To build specific targets:
```bash
make bin/myzip      # Builds myzip
make bin/myunzip    # Builds myunzip
make bin/huffman    # Builds huffman
make bin/inflate    # Builds inflate
```

Cleaning up:
```bash
make clean          # Clean all files in bin/ directory
```