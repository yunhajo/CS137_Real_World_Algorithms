#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include <bits/stdc++.h>
#include <sstream>
#include <iomanip>
#include <unistd.h>
using namespace std;
namespace fs = std::filesystem;

unsigned convertASCIItoHex(char *string) {
    
    stringstream ss;

    const char* it = string;
    const char* end = string + strlen(string);

    for (; it != end; ++it)
        ss << hex << unsigned(*it);

    unsigned result;
    ss >> result;
    return result;
}

int main(int argc, char *argv[]) {
    string zip_filename = argv[1];

    ifstream stream;
    stream.open(zip_filename, ios::binary);

    char file_signature[4];
    stream.read(file_signature, 4);
    if (string(file_signature, 4) != "\x50\x4b\x03\x04") {
        cout << "wrong file signature" << endl;
    }
    stream.ignore(4);
    
    int16_t compression;
    stream.read(reinterpret_cast<char *>(&compression), 2);
    stream.ignore(2);
    stream.ignore(2);
    stream.ignore(4);
    
    int32_t filesize;
    stream.read(reinterpret_cast<char *>(&filesize), 4);

    stream.ignore(4);

    short file_name_len;
    stream.read(reinterpret_cast<char *>(&file_name_len), 2);

    short extra_field_length;
    stream.read(reinterpret_cast<char *>(&extra_field_length), 2);

    string new_filename(file_name_len, ' ');
    stream.read(&new_filename[0], file_name_len);

    string dirS = "";
    size_t pos = new_filename.find_last_of("/");
    // make sure the poisition is valid

    if (pos != string::npos) {
        dirS = new_filename.substr(0, pos);

        struct stat sb;
        char *c = dirS.data();

        if (stat(c, &sb) != 0) {
            std::filesystem::create_directories(c);
        }
    }

    stream.ignore(extra_field_length);
    
    string filedata(filesize, ' ');
    stream.read(&filedata[0], filesize);
    
    if (compression == 0) {
        ofstream outfile (new_filename);
        outfile << filedata;
        outfile.close();
    }
    else if (compression == 8) {
        ofstream outfile (new_filename + ".deflate");
        outfile << filedata;
        outfile.close();
    }

    return 0;
}

// mkdir directories that doesn't exist