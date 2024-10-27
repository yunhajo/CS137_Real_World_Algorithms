#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include <bits/stdc++.h>
#include <sstream>
#include <iomanip>
#include "deflate8.h"
using namespace std;
namespace fs = std::filesystem;

string writeIntBytes(int n, int num_bytes) {
    unsigned char bytes[num_bytes];
    
    int b = 8 * (n - 1);
    for (int i = 0; i < num_bytes; i++) {
        bytes[i] = (n >> b) & 0xFF;
        b -= 8;
    }
    string str = "";
    for (int i = 0; i < num_bytes; i++) {
        unsigned char byte = bytes[i];
        for (int j = 7; j >= 0; j--) {
            int b = byte >> j;
            if (b & 1)
                str += "1";
            else
                str += "0";
        }
    }
    return str;
}

string convertToASCII(string filename) {
    string name = "";
    for (int i = 0; i < filename.length(); i++) {
        char x = filename.at(i);
        name += to_string(int(x));
    }
    return name;
}

string intToHex(int num, int len) {
    ostringstream ss;
    ss << setfill('0') << setw(len * 2) << hex << num;
    string result = ss.str();
    return result;
}

string hexToASCII(string hex) {
    string ascii = "";
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byte = hex.substr(i, 2);
        char ch = stoul(byte, nullptr, 16);
        ascii += ch;
    }
    reverse(ascii.begin(), ascii.end());
    return ascii;
}

int main(int argc, char *argv[]) {
    string outputZip = argv[1];
    string filename = argv[2];
    // Try compressing the file
    cout << "Deflating file" << endl;
    deflate8(filename);
    const auto fsizeComp = fs::file_size(filename + ".deflate");
    const auto fsize = fs::file_size(filename);

    cout << fsizeComp << endl;
    cout << fsize << endl;

    bool comp = false;
    if (fsizeComp < fsize) {
        comp = true;
    }
    fstream myfile;
    myfile.open(outputZip, ios::out);
    myfile << hexToASCII("04034b50");
    myfile << hexToASCII(intToHex(20, 2));
    string zero_s = hexToASCII(intToHex(0, 2));
    myfile << zero_s;
    if (comp) {
        myfile << hexToASCII(intToHex(8, 2));
    }
    else {
        myfile << zero_s;
    }
    myfile << zero_s;
    myfile << zero_s;
    myfile << hexToASCII("deadbeef");
    // Compressed file size
    // Uncompressed file size

    if (comp) {
        myfile << hexToASCII(intToHex(fsizeComp, 4));
    }
    else {
        myfile << hexToASCII(intToHex(fsize, 4));
    }
    myfile << hexToASCII(intToHex(fsize, 4));
    // file name length
    myfile << hexToASCII(intToHex(filename.length(), 2));
    myfile << zero_s;
    myfile << filename;
    // file data
    if (comp) {
        stringstream buffer;
        buffer << ifstream (filename + ".deflate").rdbuf();
        myfile << buffer.str();
    }
    else {
        stringstream buffer;
        buffer << ifstream (filename).rdbuf();
        myfile << buffer.str();
    }

    // Central directory
    myfile <<  hexToASCII("02014b50");
    myfile << hexToASCII(intToHex(30, 1));
    myfile << hexToASCII(intToHex(65, 1));
    myfile << hexToASCII(intToHex(20, 2));
    myfile << zero_s;
    if (comp) {
        myfile << hexToASCII(intToHex(8, 2));
    }
    else {
        myfile << zero_s;
    }
    myfile << zero_s;
    myfile << zero_s;
    myfile << hexToASCII("deadbeef");
    // compressed file size
    if (comp) {
        myfile << hexToASCII(intToHex(fsizeComp, 4));
    }
    else {
        myfile << hexToASCII(intToHex(fsize, 4));
    }
    // uncompressed file size
    myfile << hexToASCII(intToHex(fsize, 4));
    // file name length
    myfile << hexToASCII(intToHex(filename.length(), 2));
    myfile << zero_s;
    myfile << zero_s;
    myfile << zero_s;
    myfile << hexToASCII(intToHex(1, 2));
    myfile << hexToASCII("00000001");
    myfile << hexToASCII("00000000");
    // file name as ASCII string
    myfile << filename;

    myfile << hexToASCII("06054b50");
    myfile << zero_s;
    myfile << zero_s;
    myfile << hexToASCII(intToHex(1, 2));
    myfile << hexToASCII(intToHex(1, 2));
    // size of central directory
    int size_central = (46 + filename.length());
    myfile << hexToASCII(intToHex(size_central, 4));
    if (comp) {
        int offset_central = (30 + filename.length() + fsizeComp);
        myfile << hexToASCII(intToHex(offset_central, 4));
    }
    else {
        int offset_central = (30 + filename.length() + fsize);
        myfile << hexToASCII(intToHex(offset_central, 4));
    }
    // offset of central directory
    myfile << zero_s;
    
    myfile.close();
    return 0;
}