# JPEG (Joint Photographic Experts Group) Extension

## Background

This project implements a JPEG compression and decompression algorithm. The JPEG format is a widely-used lossy image compression format that reduces file size by selectively discarding data that is less perceptible to the human eye. This project consists of two main components: 
1. `JPEG_encoder` - which takes a PPM (Portable Pixmap) image and compresses it into a JPEG file.
2. `JPEG_decoder` - which decompresses a JPEG file back into a PPM image.

The compression algorithm applies several image processing techniques, including the discrete cosine transform (DCT), quantization, run-length encoding, and Huffman encoding to achieve efficient file compression.

## Algorithm

### Compression

To compress a PPM image into a JPEG image, the following steps are performed:

1. **Image Conversion**: The PPM image's RGB pixels are converted to YCbCr color space.
2. **Level Shifting**: Pixel values are shifted from a range of 0-255 to -128 to 127.
3. **MCU Formation**: Pixels are grouped into 8x8 Minimum Coded Units (MCUs).
4. **Discrete Cosine Transform (DCT)**: Each 8x8 MCU is transformed to the frequency domain using DCT.
5. **Quantization**: The transformed MCU values are quantized to reduce precision.
6. **Zigzag Encoding**: The quantized MCU is ordered in a zigzag pattern to prepare for run-length encoding.
7. **Run-Length Encoding**: Consecutive zeroes in the zigzag order are encoded to compress the data further.
8. **Huffman Encoding**: The result is Huffman encoded. Note that separate Huffman tables are used for DC and AC coefficients.
9. **File Output**: The compressed data is written to an output JPEG file.

### Decompression (Inflation)

To decompress a JPEG image back into a PPM format, the steps are reversed:

1. **Header Parsing**: The JPEG headers are read, and image dimensions are extracted.
2. **Huffman Decoding**: Huffman decoding is applied using the predefined tables from the JPEG specification.
3. **Run-Length Decoding**: The encoded data is expanded back to the original MCU format.
4. **Inverse Zigzag and Quantization**: The MCU data is rearranged and dequantized.
5. **Inverse DCT**: The frequency-domain data is transformed back to the spatial domain.
6. **Level Shifting Reversal**: Pixel values are shifted back to the 0-255 range.
7. **Color Space Conversion**: YCbCr pixels are converted back to RGB.
8. **File Output**: The decompressed data is saved as a PPM file.

## Results

The effectiveness of JPEG compression is measured by comparing the file sizes of the original and compressed images. The following results were obtained with varying quality levels:

- **Quality = 50**: Compression Ratio: 11.2135
- **Quality = 20**: Compression Ratio: 13.0416
- **Quality = 80**: Compression Ratio: 9.27626

As the quality parameter decreases, the compression ratio increases, resulting in smaller file sizes but also lower image quality. Larger files generally achieve more efficient compression ratios.

### Example Compressed Images

- **Image with Quality = 50**
  ![Compressed Image with q = 50](Images/boxes_1_50.jpeg)

- **Image with Quality = 20**
  ![Compressed Image with q = 20](Images/boxes_1_20.jpeg)

- **Image with Quality = 80**
  ![Compressed Image with q = 80](Images/boxes_1_80.jpeg)

For the original uncompressed images, refer to the `Images` folder containing `boxes_1_20copy.ppm`, `boxes_1_50copy.ppm`, and `boxes_1_80copy.ppm`.

## Reflection

While the JPEG encoder achieves substantial compression, there are limitations in its compliance with JPEG standards, which affects compatibility with standard JPEG viewers. Future improvements include refining the MCU processing order and implementing custom Huffman tables.

## Makefile Usage

The project includes a `Makefile` to build the encoder and decoder executables efficiently.

### Targets

- **`all`**: Builds both `bin/to_ppm` (JPEG decoder) and `bin/to_jpeg` (JPEG encoder).
- **`bin/to_ppm`**: Compiles `JPEG_decoder.cpp` to produce the `to_ppm` executable, which converts JPEG images to PPM format.
- **`bin/to_jpeg`**: Compiles `JPEG_encoder.cpp` to produce the `to_jpeg` executable, which converts PPM images to JPEG format.
- **`clean`**: Removes all compiled binaries.

### Build Instructions

To compile both the encoder and decoder, run:
```bash
make all
```

### Running the Encoder and Decoder
After building the project, you can use the executables as follows:

JPEG Encoder: Convert a PPM image to JPEG format.

```bash
./bin/to_jpeg input.ppm
```

This command will output input.jpeg.

JPEG Encoder: Convert a JPEG image back to PPM format.

```bash
./bin/to_ppm input.jpeg
```

This command will output inputcopy.ppm.

### References

References
JPEG Standard: https://www.w3.org/Graphics/JPEG/itu-t81.pdf
Sample Images: https://www.cs.cornell.edu/courses/cs664/2003fa/images/