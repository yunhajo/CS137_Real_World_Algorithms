#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
// #include <bits/stdc++.h>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <cmath>
#include <bitset>
#include <map>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;

double pi = 3.142857;

typedef vector <tuple<int, int>> rl_elem;

uint8_t byteStringDClum[] = {0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 
                            0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t huffmancodelum[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};

uint8_t byteStringDCchr[] = {0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t byteStringAClum[] = {
    0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 
    0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D
};

uint8_t huffmancodeAClum[] = {
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
    0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
    0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA
};

int zigzag[8][8] = {
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

uint8_t byteStringACchr[] = {
    0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 
    0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77
};

uint8_t huffmancodeACchr[] = {
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0,
    0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26,
    0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5,
    0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3,
    0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA,
    0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA
};

int Tb[8][8];

map<string, int> DCluminance;
map<string, int> DCchrominance;
map<string, int> ACluminance;
map<string, int> ACchrominance;

struct yCbCr_data {
    int width;
    int height;
    double **y_data;
    double **cb_data;
    double ** cr_data;
};

// Finds nearest multiple of 8
int findNearestEight(int num) {
    int remainder = num % 8;
    if (remainder == 0) {
        return num;
    }
    return num + 8 - remainder;
}

// Converts decimal into binary string of given length
string decToBinary(int n, int length)
{
    string result = "";

    int i = 0;
    while (n > 0) {
        result += to_string(n % 2);
        n = n / 2;
        i++;
    }

    for (int j = 0; j < length - i; j += 1) {
        result.push_back('0');
    }
    reverse(result.begin(), result.end());
    return result;
}

string getBinaryString(string filename) {
    ifstream f(filename, ios::binary | ios::in);
    string result = "";
    char c;
    while (f.get(c))
    {
        for (int i = 0; i < 8; i++) 
            result += to_string(((c >> i) & 1));
    }
    return result;
}

// Defines Huffman tables for DC, AC luminance and chrominance
int defineHuffmanTables(uint8_t byteStringLength[], uint8_t huffmancode[], int lengthHuff, map<string,int> &map) {

    vector<int> lengthCount(17);
    lengthCount[0] = 0;
    for(size_t i = 1; i < 17; i++) {
        lengthCount[i] = static_cast<int>(byteStringLength[i - 1]);
    }

    vector<int> nextCode(17);
    int code = 0;
    for (size_t i = 1; i < 17; i++) {
        code = (code + lengthCount[i - 1]) << 1;
        nextCode[i] = code;
    }

    size_t arraySize = lengthHuff / sizeof(uint8_t);

    vector<int> huffmanCodeNum(arraySize);
    for(size_t i = 0; i < arraySize; i++) {
        huffmanCodeNum[i] = static_cast<int>(huffmancode[i]);
    }

    int length = 0;
    int count = 0;
    int index = 0;
    for(size_t i = 1; i < 17; i++) {
        if (lengthCount[i] != 0) {
            for (size_t j = 0; j < lengthCount[i]; j++) {
                string huffmanCoded = decToBinary(nextCode[i], i);
                nextCode[i] += 1;
                map[huffmanCoded] = huffmanCodeNum[index];
                index++;
            }
        }
    }
    return 0;
}

// Populates all of tables
void createDCACTables() {
    defineHuffmanTables(byteStringDClum, huffmancodelum, sizeof(huffmancodelum), DCluminance);
    defineHuffmanTables(byteStringDCchr, huffmancodelum, sizeof(huffmancodelum), DCchrominance);
    defineHuffmanTables(byteStringAClum, huffmancodeAClum, sizeof(huffmancodeAClum), ACluminance); 
    defineHuffmanTables(byteStringACchr, huffmancodeACchr, sizeof(huffmancodeACchr), ACchrominance);
}

// Used for InverseDCT function
float C(int i)
{
  if(i == 0)
    return (1.0 / sqrt(8.0));
  else
    return (1.0 / 2.0);
}

// Computes Inverse DCT of given matrix
void inverseDCT(int dct[8][8], double matrix[8][8]) {
    float sum;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            sum = 0.0;
            for(int u = 0; u < 8; u++) {
                for(int v = 0; v < 8; v++) {
                    sum += C(u) * C(v) * float(dct[u][v]) * cos((2.0 * float(i) + 1.0) 
                    * float(u) * 3.14 / 16.0) * cos((2.0 * float(j) + 1.0) * float(v) * 3.14/16.0);
                }
                matrix[i][j] = int(sum);
            }
        }
    }
}

// Reverts Image back from Quantization
void undoQuantization(int quantized[8][8], int matrix[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matrix[i][j] = quantized[i][j] * Tb[i][j];
        }
    }
}

// Undoes Zigzag encoding of given image
void undoZigZag(int matrix[64], int output[8][8]) {
    int row = 0, col = 0;

    bool up = true;

    for (int i = 0; i < 64; ++i) {
        output[row][col] = matrix[i];

        if (up) {
            if (col == 7) {
                row++;
                up = false;
            } else if (row == 0) {
                col++;
                up = false;
            } else {
                row--;
                col++;
            }
        } 
        
        else {
            if (row == 7) {
                col++;
                up = true;
            } else if (col == 0) {
                row++;
                up = true;
            } else {
                row++;
                col--;
            }
        }
    }
}

// Given category and bit string of number, converts
// into actual integer
int getIntegerFromString(int category, string bits) {
    unsigned long long value = stoull(bits, 0, 2);
    int lowerRange = 1 << (category - 1);
    int upperRange = (1 << category) - 1;
    if (value >= lowerRange) {
        return value;
    }
    else {
        return value - upperRange;
    }
}

// Undoes Run Length encoding of matrix
void decodeRLE(rl_elem rle, int matrix[64]) {

    int index = 1;

    for (auto it = rle.begin(); it != rle.end(); ++it) {
        int run, number;
        tie(run, number) = *it;

        for (int i = 0; i < run; i++) {
            matrix[index] = 0;
            index += 1;
        }
        matrix[index] = number;
        index += 1;
    }

    for (int i = index; i < 64; i++) {
        matrix[i] = 0;
    }
}

// Transforms Huffman decoded matrix by 
// Undoing zigzag, quantization, and applying inverse DCT
void transformMatrix(int matrix[64], double output[8][8]) {
    int zigzag[8][8];
    undoZigZag(matrix, zigzag);
    int quant[8][8];
    undoQuantization(zigzag, quant);
    inverseDCT(quant, output);
}

// Reads all of AC coefficients of one MCU
void decodeCreateMatrix(string binary, int &i, double matrix[8][8], int &matrixDecoded, size_t length) {
    string curr = "";
    int numElemsFound = 0;
    rl_elem rl_encoded;

    while (numElemsFound < 64) {
        if (curr.length() >= 2) {
            int rs;
            if (matrixDecoded % 3 == 0) {
                auto it = ACluminance.find(curr);
                if (it != ACluminance.end()) {
                    rs = it->second;
                } else {
                    rs = -1;
                }
            }

            else {
                auto it = ACchrominance.find(curr);
                if (it != ACchrominance.end()) {
                    rs = it->second;
                } else {
                    rs = -1;
                }
            }

            if (rs != -1) {
                int run = rs >> 4;
                int size = rs & 0x0F;

                if (run == 0 && size == 0) {
                    break;
                }

                string actualStr = binary.substr(i, size);
                int actual = getIntegerFromString(size, actualStr);
                rl_encoded.push_back(make_tuple(run, actual));

                i += size;
                curr = "";
                numElemsFound += 1;
            }

            else {
                curr += binary[i];
                i++;
            }
        }

        else {
            curr += binary[i];
            i++;
        }
    }

    int array[64];
    array[0] = matrix[0][0];
    decodeRLE(rl_encoded, array);
    transformMatrix(array, matrix);
}

// Copies given matrix into YCbCr data (copies MCU into data)
void copyMatrixIntoYCbCr(struct yCbCr_data *data, double matrix[8][8], int matrixNo) {
    int row = ((matrixNo / 3) / (data->width / 8)) * 8;
    int col = ((matrixNo / 3) % (data->width / 8)) * 8;
    if (matrixNo % 3 == 0) {
        // Copy into Y_data
        for (int i=0; i < 8; i++) {
            for (int j=0; j < 8; j++) {
                // data->y_data[row + i][col + j] = matrix[i][j];
                data->y_data[col + j][row + i] = matrix[i][j];
            }
        }
    }
    else if (matrixNo % 3 == 1) {
        // Copy into Cb_data
        for (int i=0; i < 8; i++) {
            for (int j=0; j < 8; j++) {
                // data->cb_data[row + i][col + j] = matrix[i][j];
                data->cb_data[col + j][row + i] = matrix[i][j];
            }
        }
    }
    else {
        // Copy into Cr_data
        for (int i=0; i < 8; i++) {
            for (int j=0; j < 8; j++) {
                // data->cr_data[row + i][col + j] = matrix[i][j];
                data->cr_data[col + j][row + i] = matrix[i][j];
            }
        }
    }
}


// Decoded Huffman codes from given string. The algorithm is as follows:
// Case 1: Encoding for Y string
// a. If first element, which means that it is DC coefficient:
//      i. Get the category number: DC luminance map
//      ii. Get the number of bits to read
//      iii. Read the actual number of bytes
//      iv. Get the actual number (effectively undo the table)
//      v. Calculate the DC coefficient by undoing delta encoding (need prev dc coefficient)
// b. Else, it means that it is AC coefficient
//      i. Read the rs_bits
//      ii. Read the value based on size number
//      iii. Get the actual number (effectively undo the table)
//      iv. Get the run length encoding
//      v. Undo the run length encoding
// c. Reconstruct the matrix
// Case 2: Encoding for Cr or Cr string
// Same procedure as Y, use chrominance tables
void decodeHuffman(struct yCbCr_data *data, string binary) {

    size_t length = binary.length();

    int i = 0;

    string curr = "";

    bool isDCcoeff = true;

    int dcCoeffPrevY = 0;
    int dcCoeffPrevCb = 0;
    int dcCoeffPrevCr = 0;

    int matrixDecoded = 0;
    int matrixToDecode = (data->width / 8) * (data->height / 8) * 3;

    double currMatrix[8][8];
    while (i < length) {
        if (matrixDecoded == matrixToDecode) {
            break;
        }
        if (curr.length() >= 2) {
            int numBitsToRead;
            if (matrixDecoded % 3 == 0) {
                auto it = DCluminance.find(curr);
                if (it != DCluminance.end()) {
                    numBitsToRead = it->second;
                } else {
                    numBitsToRead = -1;
                }
            }
            else {
                auto it = DCchrominance.find(curr);
                if (it != DCchrominance.end()) {
                    numBitsToRead = it->second;
                } else {
                    numBitsToRead = -1;
                }
            }

            if (numBitsToRead != -1) {
                int dcCoeffDelta;
                if (numBitsToRead != 0) {
                    string valString = binary.substr(i, numBitsToRead);
                    dcCoeffDelta = getIntegerFromString(numBitsToRead, valString);
                }
                else {
                    dcCoeffDelta = 0;
                }

                int dcCoeff;
                if (matrixDecoded % 3 == 0) {
                    dcCoeff = dcCoeffPrevY + dcCoeffDelta;
                    dcCoeffPrevY = dcCoeff;
                }
                else if (matrixDecoded % 3 == 1) {
                    dcCoeff = dcCoeffPrevCb + dcCoeffDelta;
                    dcCoeffPrevCb = dcCoeff;
                }
                else {
                    dcCoeff = dcCoeffPrevCr + dcCoeffDelta;
                    dcCoeffPrevCr = dcCoeff;
                }

                currMatrix[0][0] = dcCoeff;
                i += numBitsToRead;
                curr = "";

                decodeCreateMatrix(binary, i, currMatrix, matrixDecoded, length);
                copyMatrixIntoYCbCr(data, currMatrix, matrixDecoded);

                matrixDecoded += 1;
            }
            else {
                curr += binary[i];
                i++;
            }
        }
        else {
            curr += binary[i];
            i++;
        }
    }
}

// Ensures that RGB value does not go over 255 or under 0
uint8_t clamp(int value) {
    return static_cast<uint8_t>(max(0, min(255, value)));
}

// Converts YCbCr image into RGB values
void yCbCrToRGB(struct yCbCr_data *data, int *matrix, int width, int height) {

    int r_data[63*63];
    int g_data[63*63];
    int b_data[63*63];
    int idxRGB = 0;

    int index = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            float y = static_cast<float>(data->y_data[i][j]);
            float cb = static_cast<float>(data->cb_data[i][j]);
            float cr = static_cast<float>(data->cr_data[i][j]);

            float r = (cr * (2 - 2 * 0.299) + y);
            float b = cb * (2 - 2 * 0.114) + y;
            float g = (y - 0.114 * b - 0.299 * r) / 0.587;

            if (idxRGB < 63 * 63) {
                r_data[idxRGB] = r;
                g_data[idxRGB] = g;
                b_data[idxRGB] = b;
                idxRGB += 1;
            }

            int R = static_cast<int>(r) + 128;
            int G = static_cast<int>(g) + 128;
            int B = static_cast<int>(b) + 128;

            matrix[index++] = clamp(R);
            matrix[index++] = clamp(G);
            matrix[index++] = clamp(B);
        }
    }
}

// Reads count number of bytes from filestream
vector<uint8_t> readBytes(ifstream& file, size_t count) {
    vector<uint8_t> buffer(count);
    file.read(reinterpret_cast<char*>(buffer.data()), count);
    return buffer;
}

// Reads two bytes from filestream and interprets it as uint16_t
// Used for reading lengths of sections
uint16_t readUint16(ifstream& file) {
    auto bytes = readBytes(file, 2);
    return (bytes[0] << 8) | bytes[1];
}

// Reads quantization table stored in JPEG image
void readQuantizationTables(ifstream& file) {
    uint16_t length = readUint16(file) - 3; 
    file.ignore(1);
    auto tables = readBytes(file, length);
    
    for (size_t i=0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            Tb[i][j] = tables[i * 8 + j];
        }
    }
}

// Reads start of frame of image, gets width and height
// of image
void readStartOfFrame(ifstream& file, int &h, int &w) {
    uint16_t length = readUint16(file) - 2;
    file.ignore(1);

    uint16_t height = readUint16(file);
    uint16_t width = readUint16(file);

    h = (int) height;
    w = (int) width;

    file.ignore(length - 5);
}

// Skips of the start of scan part
void readStartOfScan(ifstream& file) {
    uint16_t length = readUint16(file) - 2; 
    file.ignore(length);
}

// Reads data of image as a bitstring
string readImageData(ifstream& file) {
    stringstream buffer;
    buffer << file.rdbuf();
    string binaryData = buffer.str();

    // Convert the binary data to a string of bits
    string bitString;
    for (unsigned char c : binaryData) {
        bitString += bitset<8>(c).to_string();
    }
    return bitString;
}

// Read all of the markers of JPG
// Returns binary string for the given JPEG file
string decodeJPEG(const string& filename, int &h, int &w) {

    ifstream file(filename, ios::binary);
    
    if (!file) {
        cout << "Cannot open file" << endl;
        return "";
    }

    while (file) {

        uint8_t marker1 = file.get();
        if (marker1 != 0xFF) 
            continue;

        uint8_t marker2 = file.get();
        if (marker2 == 0xD9) break; // EOI marker
        if (marker2 == 0xDA) { // Start of Scan
            readStartOfScan(file);
            break;
        } 
        else if (marker2 == 0xDB) { // DQT marker
            readQuantizationTables(file);
        } 
        else if (marker2 == 0xC0) { // SOF0 marker
            readStartOfFrame(file, h, w);
        } 
        else if (marker2 == 0xD8) { // JPEG header
            continue;
        }
        else {
            uint16_t length = readUint16(file) - 2;
            file.ignore(length);
        }
    }

    string imageData = readImageData(file);
    return imageData;
}

// Decoding JPEG Image
// Check for start of Image
// Skip over the application header
// Extract the two quantization tables
// Skip over the start of frame - get length and height
// Go to start of scan
// Decode the entropy-encoded segment
// 1. Go through each of the 8x8 blocks - Y, Cb, Cr
// 2. Decode the Huffman decoding - first decode the DC coefficient, then AC coefficients
// 3. Create the matrix by undoing the run-length encoding
// 4. Reverse the zig-zag traversal
// 5. Undo the quantization: a_i,j = val * Q_i,j
// 6. Perform IDCT
// 7. Level Shift back into original
// 8. Convert Y-Cb-Cr into RGB
// Check for end of image
int main(int argc, char *argv[]) {
    // Create tables for DC, AC Huffman codes
    createDCACTables();

    string filename = argv[1];
    // Try compressing the file
    cout << "Compressing ppm to jpeg " << filename << endl;

    int height;
    int width;
    // Read all of the headers
    string binary = decodeJPEG(filename, height, width);

    cout << "Dimensions " << height << " " << width << endl;

    int newHeight = findNearestEight(height);
    int newWidth = findNearestEight(width);

    int numVerMCU = newHeight / 8;
    int numHorMCU = newWidth / 8;

    struct yCbCr_data* data = new struct yCbCr_data;

    double **y_data = new double*[newWidth];
    double **cb_data = new double*[newWidth];
    double **cr_data = new double*[newWidth];
    for (size_t i=0; i < newWidth; i++) {
        y_data[i] = new double[newHeight];
        cb_data[i] = new double[newHeight];
        cr_data[i] = new double[newHeight];
    }

    data->y_data = y_data;
    data->cb_data = cb_data;
    data->cr_data = cr_data;
    data->height = newHeight;
    data->width = newWidth;

    decodeHuffman(data, binary);

    int *RGBmatrix = new int[width * height * 3];
    
    yCbCrToRGB(data, RGBmatrix, width, height);

    filename.resize(filename.size() - 5);

    ofstream open(filename + "copy.ppm", ios::out | ios::trunc);

    open << "P6\n" << width << " " << height << "\n255\n";

    for (int i=0; i < width * height * 3; i++) {
        char character = static_cast<char>(RGBmatrix[i]);
        open.write(&character, sizeof(character));
    }

    for (size_t i=0; i < newWidth; i++) {
        delete[] y_data[i];
        delete[] cb_data[i];
        delete[] cr_data[i];
    }
    delete[] y_data;
    delete[] cb_data;
    delete[] cr_data;

    delete data;

    delete[] RGBmatrix;

    open.close();
    return 0;
}