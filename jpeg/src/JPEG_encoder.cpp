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

double pi = 3.14159265358979323846;
typedef vector <tuple<int, int>> rl_elem;

enum tableType {
    LUM, 
    CHRR,
    CHRB
};

int Tb[8][8] = {
    16,    11,    10,    16,    24,    40,    51,    61,
    12,    12,    14,    19,    26,    58,    60,    55,
    14,    13,    16,    24,    40,    57,    69,    56,
    14,    17,    22,    29,    51,    87,    80 ,   62,
    18,    22,    37,    56,    68,    109,   103,   77,
    24,    35,    55,    64,    81,    104,   113,   92,
    49,    64,    78,    87,   103,    121,   120,   101,
    72,    92,    95,    98,   112,    100,   103,   99
};

int zigzag[64] = {
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

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

int Q = 50;

struct ppm_data {
    int width;
    int height;
    int data_length;
    int *data;
};

struct yCbCr_data {
    int width;
    int height;
    double **y_data;
    double **cb_data;
    double ** cr_data;
};

string code_word[16];
map<int, string> DCluminance;
map<int, string> DCchrominance;
map<int, string> ACluminance;
map<int, string> ACchrominance;

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

// Define Huffman table for given type
int defineHuffmanTables(uint8_t byteStringLength[], uint8_t huffmancode[], int lengthHuff, map<int,string> &map) {

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
                map[huffmanCodeNum[index]] = huffmanCoded;
                index++;
            }
        }
    }
    return 0;
}

// Create Huffmantables for DC AC coefficients
void createDCACTables() {
    defineHuffmanTables(byteStringDClum, huffmancodelum, sizeof(huffmancodelum), DCluminance);
    defineHuffmanTables(byteStringDCchr, huffmancodelum, sizeof(huffmancodelum), DCchrominance);
    defineHuffmanTables(byteStringAClum, huffmancodeAClum, sizeof(huffmancodeAClum), ACluminance); 
    defineHuffmanTables(byteStringACchr, huffmancodeACchr, sizeof(huffmancodeACchr), ACchrominance);
}

// Calculate the Quantization Matrix, based on pre-defined value Q
int getQuantizationMatrix() {
    int S;
    if (Q < 50) {
        S = 5000/Q;
    }
    else {
        S = 200 - 2*Q;
    }

    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            Tb[i][j] = (S * Tb[i][j] + 50) / 100;
        }
    }
    return 0;
}

// Finds nearest multiple of 8 for an integer
int findNearestEight(int num) {
    int remainder = num % 8;
    if (remainder == 0) {
        return num;
    }
    return num + 8 - remainder;
}

float C(int u)
{
  if(u == 0)
    return (1.0 / sqrt(2.0));
  else
    return 1.0;
}

// Performs DctTransform on the new matrix
int dctTransformMatrix(double matrix[8][8], double dct[8][8]) {
    float sum;
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++) {
        sum = 0.0;
        for(int u = 0; u < 8; u++)
            for(int v = 0; v < 8; v++)
                sum += float(matrix[u][v]) * cos((2.0 * float(u) + 1.0) * float(i) * 3.14/16.0) *
                cos((2.0 * float(v) + 1.0) * float(j) * 3.14/16.0);
            dct[i][j] = int(0.25 * C(i) * C(j) * sum);
        }
    return 0;
}

// Quantizes the matrix with pre define quantization table
int quantizeMatrix(double matrix[8][8], int transform[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            transform[i][j] = round(matrix[i][j] / Tb[i][j]);
        }
    }
    return 0;
}

// Zig zag encodes the data
int convertZigZag(int matrix[8][8], int result[64]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int idx = zigzag[i * 8 + j];
            result[idx] = matrix[i][j];
        }
    }
    return 0;
}

// Transforms the matrix by applying DCT transform, quantization, and zig zag encoding
int transformMCU(double matrix[8][8], int newMatrix[64]) {
    auto dctTransform = new double[8][8];
    dctTransformMatrix(matrix, dctTransform);

    int transform[8][8];
    quantizeMatrix(dctTransform, transform);

    convertZigZag(transform, newMatrix);
    delete[] dctTransform;
    return 0;
}

// Run-length encodes the data
rl_elem runLengthEncode(int arr[64])
{
    rl_elem rl_encoded;
    int count = 0;
    for (int i = 1; i < 64; i++) {
        if (arr[i] == 0) {
            count++;
        }
        else {
            rl_encoded.push_back(make_tuple(count, arr[i]));
            count = 0;
        }
    }
    return rl_encoded;
}

int numberOfBits(int num) {
    return floor(log2(abs(num))) + 1;
}

// Returns category (number) and number of 
// bits needed to encode the data
tuple<int, string> getBitsandValue(int num) {
    if (num == 0) {
        return make_tuple(0, "");
    }
    int category = numberOfBits(num);
    string bits;
    if (num > 0) {
        bits = decToBinary(num, category);
    }
    else {
        int offset = num + (1 << category) - 1;
        bits = decToBinary(offset, category);
    }
    return make_tuple(category, bits);
}

// If encodeDC is true, delta encode the DC coefficient
string getBinaryStringMCU(int MCU[64], int dc_coeff_pred, tableType type) {
    string result = "";
    int dc_coeff_curr;
    if (dc_coeff_pred == -500) {
        dc_coeff_curr = MCU[0];
    }
    else {
        dc_coeff_curr = MCU[0] - dc_coeff_pred;
    }

    tuple<int, string> dc_tuple = getBitsandValue(dc_coeff_curr);
    if (type == LUM) {
        result += DCluminance[get<0>(dc_tuple)];
    }
    else {
        result += DCchrominance[get<0>(dc_tuple)];
    }
    result += get<1>(dc_tuple);

    rl_elem rle = runLengthEncode(MCU);
    for (auto it = rle.begin(); it != rle.end(); ++it) {
        int run, number;
        tie(run, number) = *it;

        tuple<int, string> ac_tuple = getBitsandValue(number);

        int rs = (run << 4) | get<0>(ac_tuple);

        string rs_bits;

        if (type == LUM) {
            rs_bits = ACluminance[rs];
        }
        else {
            rs_bits = ACchrominance[rs];
        }
        result += rs_bits;
        result += get<1>(ac_tuple);
    }
    if (type == LUM) {
        result += ACluminance[0];
    }
    else {
        result += ACchrominance[0];
    }
    return result;
}

string transformChannel(int width, int height, struct yCbCr_data *data, tableType type) {
    string result = "";
    int dc_coeff_prevY = -500;
    int dc_coeff_prevCb = -500;
    int dc_coeff_prevCr = -500;

    for (size_t i=0; i < height / 8; i++) {
        for (size_t j=0; j < width / 8; j++) {

            auto arrayY = new double[8][8];
            auto arrayCb = new double[8][8];
            auto arrayCr = new double[8][8];

            for (size_t k = 0; k < 8; k++) {
                for (size_t l = 0; l < 8; l++) {
                    arrayY[k][l] = data->y_data[j*8 + l][i*8 + k] - 128;
                    arrayCb[k][l] = data->cb_data[j*8 + l][i*8 + k] - 128;
                    arrayCr[k][l] = data->cr_data[j*8 + l][i*8 + k] - 128;
                }
            }

            int finalMCUY[64];
            int finalMCUCb[64];
            int finalMCUCr[64];
            
            transformMCU(arrayY, finalMCUY);
            transformMCU(arrayCb, finalMCUCb);
            transformMCU(arrayCr, finalMCUCr);

            string encodedY = getBinaryStringMCU(finalMCUY, dc_coeff_prevY, LUM);
            string encodedCb = getBinaryStringMCU(finalMCUCb, dc_coeff_prevCb, CHRB);
            string encodedCr = getBinaryStringMCU(finalMCUCr, dc_coeff_prevCr, CHRR);

            result += encodedY;
            result += encodedCb;
            result += encodedCr;

            dc_coeff_prevY = finalMCUY[0];
            dc_coeff_prevCb = finalMCUCb[0];
            dc_coeff_prevCr = finalMCUCr[0];

            delete[] arrayY;
            delete[] arrayCb;
            delete[] arrayCr;
        }
    }
    return result;
}

int transform(struct yCbCr_data *data, ofstream &output) {
    cout << data->width << endl;
    cout << data->height << endl;
    string result = transformChannel(data->width, data->height, data, LUM);

    int paddedLength = findNearestEight(result.length());

    for (int i=0; i < paddedLength - result.length(); i++) {
        result += "0";
    }

    for (int j=0; j < result.length(); j+=8) {
        string byte = result.substr(j, 8);
        unsigned long long tmp = std::stoull(byte, 0, 2);
        output << hex << char(tmp);
        if (j == result.length() - 8 && tmp == 255) {
            output.put(0x00);
        }
    }
    return 0;
}



// Converts rgb matrix into YCbCr matrix
// Expands the matrix if necessary (simply copies over
// last row or column)
struct yCbCr_data *rgbToYCbCr(struct ppm_data *ppm_data) {

    int new_width = findNearestEight(ppm_data->width);
    int new_height = findNearestEight(ppm_data->height);

    struct yCbCr_data* new_data = new struct yCbCr_data;

    double **y_data = new double*[new_width];
    double **cb_data = new double*[new_width];
    double **cr_data = new double*[new_width];

    for (size_t i=0; i < new_width; i++) {
        y_data[i] = new double[new_height];
        cb_data[i] = new double[new_height];
        cr_data[i] = new double[new_height];
    }

    int idxRGB = 0;

    for (int i = 0; i < ppm_data->data_length / 3; i ++) {
        int R = ppm_data->data[i * 3];
        int G = ppm_data->data[i * 3 + 1];
        int B = ppm_data->data[i * 3 + 2];

        double y_val = R * 0.299 + G * 0.587 + B * 0.114;
        double cb_val = 128 - (R * 0.168736) - (G * 0.331264) + (B * 0.5);
        double cr_val = 128 + (R * 0.5) - (G * 0.418688) -(B * 0.081312);

        int row = i / ppm_data->width;
        int col = i % ppm_data->width;

        y_data[row][col] = y_val;
        cb_data[row][col] = cb_val;
        cr_data[row][col] = cr_val;
    }

    for (size_t i = ppm_data->height; i < new_height; i++) {
        for (size_t j=0; j < ppm_data->width; j++) {
            y_data[i][j] = y_data[ppm_data->height - 1][j];
            cb_data[i][j] = cb_data[ppm_data->height - 1][j];
            cr_data[i][j] = cr_data[ppm_data->height - 1][j];
        }
    }

    for (size_t i = ppm_data->width; i < new_width; i++) {
        for (size_t j=0; j < new_height; j++) {
            y_data[j][i] = y_data[j][ppm_data->width - 1];
            cb_data[j][i] = cb_data[j][ppm_data->width - 1];
            cr_data[j][i] = cr_data[j][ppm_data->width - 1];
        }
    }

    new_data->y_data = y_data;
    new_data->cb_data = cb_data;
    new_data->cr_data = cr_data;
    new_data->width = new_width;
    new_data->height = new_height;

    return new_data;
}

// Comes up with App0Marker for JPEG image
vector<unsigned char> getApp0Marker(int width, int height) {

    unsigned short xDensity = 1;
    unsigned short yDensity = 1;

    vector<unsigned char> app0Marker;

    // Add the APP0 marker (FF E0)
    app0Marker.push_back(0xFF);
    app0Marker.push_back(0xE0);

    // Calculate the length of the header 
    unsigned short length = 16;
    app0Marker.push_back((length >> 8) & 0xFF); 
    app0Marker.push_back(length & 0xFF);        

    // Add the JFIF identifier
    app0Marker.push_back('J');
    app0Marker.push_back('F');
    app0Marker.push_back('I');
    app0Marker.push_back('F');
    app0Marker.push_back(0x00);

    // Version
    app0Marker.push_back(0x01); 
    app0Marker.push_back(0x01); 

    // Units
    app0Marker.push_back(0x00);

    // Add the X density (simply width)
    app0Marker.push_back((xDensity >> 8) & 0xFF); 
    app0Marker.push_back(xDensity & 0xFF);        

    // Add the Y density (simply height)
    app0Marker.push_back((yDensity >> 8) & 0xFF); 
    app0Marker.push_back(yDensity & 0xFF);       

    app0Marker.push_back(0x00);

    app0Marker.push_back(0x00);

    return app0Marker;
}

// Encodes the quantization table to be written out to
// file
vector<unsigned char> getQuantizationMarker(int destination) {
    vector<unsigned char> dqtSegment;

    // Add the DQT marker (FF DB)
    dqtSegment.push_back(0xFF);
    dqtSegment.push_back(0xDB);

    unsigned short length = 67;
    dqtSegment.push_back((length >> 8) & 0xFF); 
    dqtSegment.push_back(length & 0xFF);        

    dqtSegment.push_back(destination & 0xFF);

    for (size_t i=0; i < 8; i++) {
        for (size_t j=0; j < 8; j++) {
            dqtSegment.push_back(static_cast<unsigned char>(Tb[i][j]));
        }
    } 
    return dqtSegment;
}

// Writes start of frame to file
void writeSOFSegment(ofstream &outputFile, int width, int height, int numComponents) {
    // Start of Frame marker
    outputFile.put(0xFF);
    outputFile.put(0xC0);

    // Length of the segment 
    int length = 8 + 3 * numComponents;
    outputFile.put((length >> 8) & 0xFF);
    outputFile.put(length & 0xFF);

    // Precision
    outputFile.put(0x08);

    // Image height
    outputFile.put((height >> 8) & 0xFF); 
    outputFile.put(height & 0xFF);       

    // Image width
    outputFile.put((width >> 8) & 0xFF);  
    outputFile.put(width & 0xFF);        

    outputFile.put(numComponents);

    // Component specification
    for (int i = 1; i <= numComponents; ++i) {
        outputFile.put(i); 

        if (i == 1) {
            outputFile.put(0x11);
            outputFile.put(0x00); 
        } else {
            outputFile.put(0x11);
            outputFile.put(0x01); 
        }
    }
}

// Writes Start of Scan segment to file
void writeSOSSegment(ofstream &outputFile, int numComponents) {
    // Start of Scan marker
    outputFile.put(0xFF);
    outputFile.put(0xDA);

    // Length of the segment
    int length = 6 + 2 * numComponents;
    outputFile.put((length >> 8) & 0xFF); 
    outputFile.put(length & 0xFF);

    // Number of components
    outputFile.put(numComponents);

    // Component specification
    for (int i = 1; i <= numComponents; ++i) {
        outputFile.put(i); 

        if (i == 1) {
            outputFile.put(0x00); 
        } else {
            outputFile.put(0x11);
        }
    }

    outputFile.put(0x00); 
    outputFile.put(0x3F); 
    outputFile.put(0x00);
}

// Writes Huffman table to JPEG
void writeHuffmanTables(ofstream &outputFile) {
    outputFile.put(0xFF);
    outputFile.put(0xC4);

    int DClum = sizeof(byteStringDClum) + sizeof(huffmancodelum) + 3;
    outputFile.put((DClum >> 8) & 0xFF); 
    outputFile.put(DClum & 0xFF);

    outputFile.put(0x00);

    for (uint8_t val : byteStringDClum) {
        outputFile.put(val);
    }

    for (uint8_t val : huffmancodelum) {
        outputFile.put(val);
    }

    outputFile.put(0xFF);
    outputFile.put(0xC4);

    int DCchr = sizeof(byteStringDCchr) + sizeof(huffmancodelum) + 3;
    outputFile.put((DCchr >> 8) & 0xFF); 
    outputFile.put(DCchr & 0xFF); 
    outputFile.put(0x01);

    for (uint8_t val : byteStringDCchr) {
        outputFile.put(val);
    }

    for (uint8_t val : huffmancodelum) {
        outputFile.put(val);
    }

    outputFile.put(0xFF);
    outputFile.put(0xC4);

    int AClum = sizeof(byteStringAClum) + sizeof(huffmancodeAClum) + 3;
    outputFile.put((AClum >> 8) & 0xFF); 
    outputFile.put(AClum & 0xFF); 

    outputFile.put(0x10);

    for (uint8_t val : byteStringAClum) {
        outputFile.put(val);
    }

    for (uint8_t val : huffmancodeAClum) {
        outputFile.put(val);
    }

    outputFile.put(0xFF);
    outputFile.put(0xC4);

    int ACchr = sizeof(byteStringACchr) + sizeof(huffmancodeACchr) + 3;
    outputFile.put((ACchr >> 8) & 0xFF); 
    outputFile.put(ACchr & 0xFF);

    outputFile.put(0x11);

    for (uint8_t val : byteStringACchr) {
        outputFile.put(val);
    }

    for (uint8_t val : huffmancodeACchr) {
        outputFile.put(val);
    }
}

int main(int argc, char *argv[]) {
    createDCACTables();
    getQuantizationMatrix();
    string filename = argv[1];
    // Try compressing the file
    cout << "Compressing ppm to jpeg" << endl;

    const auto fsize = fs::file_size(filename);

    cout << fsize << endl;

    ifstream file(filename); 
    string fileString((istreambuf_iterator<char>(file)), 
                        istreambuf_iterator<char>());

    cout << fileString.size() << endl;

    int len = 0;
    if (fileString[0] != 'P' || fileString[1] != '6') {
        cout << "Unsupported format" << endl;
        return -1;
    }
    
    int width = 3;
    while (fileString[width] != ' ') {
        width ++;
    }
    string widthStr = fileString.substr(3, width - 3);

    int height = width + 1;
    while (fileString[height] != '\n') {
        height ++;
    }
    string heightStr = fileString.substr(width + 1, height - width - 1);

    int pic_width = stoi(widthStr);
    int pic_height = stoi(heightStr);

    int pixel_length = height + 1;
    while (fileString[pixel_length] != '\n') {
        pixel_length++;
    }

    string pixel_max = fileString.substr(height + 1, pixel_length - height - 1);
    if (pixel_max != "255") {
        cout << "Wrong maximum pixel value " << pixel_max << endl;
    }

    int data_length = fsize - pixel_length - 1;
    int data[data_length];
    for (int i = pixel_length + 1; i < data_length + pixel_length + 1; i++) {
        char c = fileString[i];
        data[i - pixel_length - 1] = static_cast<unsigned char>(c);
    }

    struct ppm_data ppm_data;
    ppm_data.width = pic_width;
    ppm_data.height = pic_height;
    ppm_data.data_length = data_length;
    ppm_data.data = data;

    yCbCr_data *ycbcr = rgbToYCbCr(&ppm_data);

    filename.resize(filename.size() - 4);

    ofstream open(filename + ".jpeg", ios::out | ios::trunc);

    open.put(0xFF);
    open.put(0xD8);

    int expandedWidth = findNearestEight(pic_width);
    int expandedHeight = findNearestEight(pic_height);

    vector <unsigned char>app0Marker = getApp0Marker(expandedWidth, expandedHeight);

    open.write(reinterpret_cast<char*>(&app0Marker[0]), app0Marker.size());

    vector<unsigned char>quantLum = getQuantizationMarker(0);
    vector<unsigned char>quantChr = getQuantizationMarker(1);

    open.write(reinterpret_cast<char*>(&quantLum[0]), quantLum.size());
    open.write(reinterpret_cast<char*>(&quantChr[0]), quantChr.size());

    writeSOFSegment(open, expandedWidth, expandedHeight, 3);
    writeHuffmanTables(open);
    writeSOSSegment(open, 3);

    transform(ycbcr, open);

    for (size_t i = 0; i < ycbcr->width; i++) {
        delete[] ycbcr->y_data[i];
        delete[] ycbcr->cb_data[i];
        delete[] ycbcr->cr_data[i];
    }
    delete[] ycbcr->y_data;
    delete[] ycbcr->cb_data;
    delete[] ycbcr->cr_data;

    // Free the struct
    delete ycbcr;

    open.put(0xFF);
    open.put(0xD9);
    open.close();
    return 0;
}