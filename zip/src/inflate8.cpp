#include "inflate8.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include <bits/stdc++.h>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <cmath>
#include <bitset>
#include <map>
using namespace std;

int alphOrder[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

vector<vector<string>> posLenCodes(8);
map<string, int> codeLenCode;

vector<vector<string>> posLitLenCodes(19);
map<string, int> litLenCode;

vector<vector<string>> posDistCodes(19);
map<string, int> distCode;

int dArray[28];

void initdArray() {
    int exp = 1;
    for (int i = 0; i < 27; i += 2) {
        dArray[i] = pow(2.0, exp);
        dArray[i+1] = pow(2.0, exp);
        exp += 1;
    }
}

string getBinaryString(string filename) {
    ifstream f(filename, ios::binary | ios::in);
    string result = "";
    char c;
    while (f.get(c))
    {
        for (int i = 0; i < 8; i++)  // (int i = 7; i >= 0; i--) or if you want reverse bit order in bytes
            result += to_string(((c >> i) & 1));
    }
    return result;
}

int RoundUp(int &x)
{
    return (((x + 7) & (-8)) - x);
}

tuple<int, int> checkLengthSeven(string lenString) {
    unsigned long long value = std::stoull(lenString, 0, 2);
    int length = value + 256;
    if (length >= 256 && length <= 279) {
        if (length >= 257 && length <= 264) {
            return make_tuple(value + 2, 0);
        }
        else if (length >= 265 && length <= 268) {
            return make_tuple(11 + 2 * (length - 265) , 1);
        }
        else if (length >= 269 && length <= 272) {
            return make_tuple(19 + 4 * (length - 269), 2);
        }
        else if (length >= 273 && length <= 276) {
            return make_tuple(35 + 8 * (length - 273), 3);
        }
        else if (length >= 277 && length <= 279) {
            return make_tuple(67 + 16 * (length - 277), 4);
        }
    }
    return make_tuple(0, 0);
}

int getLength(int binary, string additional) {
    unsigned long long value = std::stoull(additional, 0, 2);
    return (binary + value);
}

tuple<int, int> checkLengthEight(string lenString) {
    unsigned long long value = std::stoull(lenString, 0, 2);
    int length = value + 88;
    if (length >= 280 && length <= 287) {
        if (length == 280) {
            return make_tuple(115, 4);
        }
        else if (length >= 281 && length <= 284) {
            return make_tuple(131 + 32 * (length - 281), 5);
        }
        else if (length == 285) {
            return make_tuple(258, 0);
        }
    }
    return make_tuple(0, 0);
}

int matchASCII(string binary) {
    unsigned long long value = std::stoull(binary, 0, 2);
    if (binary.length() == 8) {
        int num = value - 48;
        if (num >= 0 && num <= 143) {
            return num;
        }
    }
    else if (binary.length() == 9) {
        int num = value - 256;
        if (num >= 144 && num <= 255) {
            return num;
        }
    }
    return -1;
}

tuple<int, int> checkLength(int length) {
    if (length >= 256 || length <= 287) {
        if (length >= 257 && length <= 264) {
            return make_tuple(length - 254, 0);
        }
        else if (length >= 265 && length <= 268) {
            return make_tuple(11 + 2 * (length - 265) , 1);
        }
        else if (length >= 269 && length <= 272) {
            return make_tuple(19 + 4 * (length - 269), 2);
        }
        else if (length >= 273 && length <= 276) {
            return make_tuple(35 + 8 * (length - 273), 3);
        }
        else if (length >= 277 && length <= 279) {
            return make_tuple(67 + 16 * (length - 277), 4);
        }
        else if (length == 280) {
            return make_tuple(115, 4);
        }
        else if (length >= 281 && length <= 284) {
            return make_tuple(131 + 32 * (length - 281), 5);
        }
        else if (length == 285) {
            return make_tuple(258, 0);
        }
    }
    return make_tuple(0, 0);
}

tuple<int, int> checkDistance(string binary) {
    unsigned long long value = std::stoull(binary, 0, 2);
    if (value >=0 && value <= 29) {
        if (value >= 0 && value <= 3) {
            return make_tuple(value + 1, 0);
        }
        else {
            int q = value - 4;
            int val = 5;
            for (int i = 0; i < q; i++) {
                val += dArray[i];
            }
            return make_tuple(val, (value / 2 - 1));
        }
    }
    return make_tuple(0, 0);
}

tuple<int, int> checkDistanceDynamic(int value) {
    if (value >=0 || value <= 29) {
        if (value >= 0 && value <= 3) {
            return make_tuple(value + 1, 0);
        }
        else {
            int q = value - 4;
            int val = 5;
            for (int i = 0; i < q; i ++) {
                val += dArray[i];
            }
            return make_tuple(val, (value / 2 - 1));
        }
    }
    return make_tuple(0, 0);
}

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

void createDynamicCodeAlpabet(string litBinary) {
    int len = litBinary.length();
    int code_length[19];
    int max_length = 0;

    int idx = 0;
    for (int i=0; i < 19; i += 1) {
        if (i < len / 3) {
            string three = litBinary.substr(idx, 3);
            reverse(three.begin(), three.end());
            unsigned long long tmp = std::stoull(three, 0, 2);
            if (tmp > max_length) {
                max_length = tmp;
            }
            int order = alphOrder[i];
            code_length[order] = tmp;
            idx += 3;
        }
        else {
            int order = alphOrder[i];
            code_length[order] = 0;
        }
    }

    int bl_count[max_length + 1];
    fill_n(bl_count, max_length + 1, 0);

    for (int j=0; j < 19; j+=1) {
        int tmp = code_length[j];
        bl_count[tmp] += 1;
    }

    bl_count[0] = 0;
    
    int code = 0;
    int next_code[max_length + 1];
    
    for (int k=1; k <= max_length+1; k += 1) {
        code = (code + bl_count[k-1]) << 1;
        next_code[k] = code;
    }

    for (int i=0; i < 19; i += 1) {
        if (code_length[i] != 0) {
            int const len2 = code_length[i]; 
            string codes = decToBinary(next_code[len2], len2);
            posLenCodes[len2].push_back(codes);
            codeLenCode[codes] = i;
            next_code[len2] += 1;
        }
    }
}

void createLitLenDistAlphabet(string &binary, int &i, int hlit, int hdist) {
    
    int code_length[hlit + hdist];
    int counter = 0;

    int max_length = 0;

    string curr = "";

    while (i < binary.length()) {
        if (counter == hlit + hdist) {
            break;
        }
        int len = curr.length();
        if (posLenCodes[len].size() != 0) {
            int code_len = -1;
            if (find(posLenCodes[len].begin(), posLenCodes[len].end(), curr) != posLenCodes[len].end()) {
                code_len = codeLenCode[curr];
            }
            if (code_len != -1) {
                if (code_len <= 15) {
                    code_length[counter] = code_len;
                    counter++;
                    curr = "";
                    if (code_len > max_length) {
                        max_length = code_len;
                    }
                }
                else if (code_len == 16) {
                    string tmpString = binary.substr(i, 2);
                    reverse(tmpString.begin(), tmpString.end());
                    unsigned long long numCopy = std::stoull(tmpString, 0, 2);
                    i += 2;
                    numCopy += 3;
                    while (numCopy > 0) {
                        code_length[counter] = code_length[counter - 1];
                        counter++;
                        numCopy--;
                    }
                    curr = "";
                }
                else if (code_len == 17) {
                    string tmpString = binary.substr(i, 3);
                    reverse(tmpString.begin(), tmpString.end());
                    unsigned long long numZero = std::stoull(tmpString, 0, 2);
                    i += 3;
                    numZero += 3;
                    while (numZero > 0) {
                        code_length[counter] = 0;
                        counter++;
                        numZero--;
                    }
                    curr = "";
                }
                else if (code_len == 18) {
                    string tmpString = binary.substr(i, 7);
                    reverse(tmpString.begin(), tmpString.end());
                    unsigned long long numZero = std::stoull(tmpString, 0, 2);
                    i += 7;
                    numZero += 11;
                    while (numZero > 0) {
                        code_length[counter] = 0;
                        counter++;
                        numZero--;
                    }
                    curr = "";
                }
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

    int bl_count[max_length + 1];
    fill_n(bl_count, max_length + 1, 0);

    for (int j=0; j < hlit; j+=1) {
        int tmp = code_length[j];
        bl_count[tmp] += 1;
    }
    bl_count[0] = 0;
    
    int code = 0;
    int next_code[max_length + 1];
    fill_n(next_code, max_length + 1, 0);
    
    for (int k=1; k <= max_length+1; k += 1) {
        code = (code + bl_count[k-1]) << 1;
        next_code[k] = code;
    }

    for (int k=0; k < hlit; k += 1) {
        if (code_length[k] != 0) {
            int const len2 = code_length[k]; 
            string codes = decToBinary(next_code[len2], len2);
            posLitLenCodes[len2].push_back(codes);
            litLenCode[codes] = k;
            next_code[len2] += 1;
        }
    }

    int bl_countd[max_length + 1];
    fill_n(bl_countd, max_length + 1, 0);

    for (int j=hlit; j < hlit + hdist; j+=1) {
        int tmp = code_length[j];
        bl_countd[tmp] += 1;
    }
    bl_countd[0] = 0;
    
    int coded = 0;
    int next_coded[max_length + 1];
    fill_n(next_coded, max_length + 1, 0);
    
    for (int k=1; k <= max_length+1; k += 1) {
        coded = (coded + bl_countd[k-1]) << 1;
        next_coded[k] = coded;
    }

    for (int k=0; k < hdist; k += 1) {
        if (code_length[k + hlit] != 0) {
            int const len3 = code_length[k + hlit]; 
            string codes = decToBinary(next_coded[len3], len3);
            posDistCodes[len3].push_back(codes);
            distCode[codes] = k;
            next_coded[len3] += 1;
        }
    }
}

bool decodeFixedHuffman(string &binary, int &i, fstream &ofile, bool final) {
    int length = binary.length();

    bool dist = false;
    int reLength = 0;
    string curr = "";

    while (i < length) {
        if (!dist && curr.length() == 7){
            if (curr == "0000000" && final) {
                return true;
            }
            else if (curr == "0000000" && !final) {
                int nextEight = RoundUp(i);
                i += nextEight;
                curr = "";
                return false;
            }
            tuple<int, int> tmp = checkLengthSeven(curr);
            if (get<0>(tmp) != 0) {
                int length;
                if (get<1>(tmp) != 0) {
                    string additional = binary.substr(i, get<1>(tmp));
                    reverse(additional.begin(), additional.end());
                    length = getLength(get<0>(tmp), additional);
                }
                else {
                    length = get<0>(tmp);
                }
                dist = true;
                i += get<1>(tmp);
                curr = "";
                reLength = length;
            }
            else {
                curr += binary[i];
                i += 1;
            }
        }
        else if (!dist && curr.length() == 8) {
            tuple<int, int> tmp = checkLengthEight(curr);
            if (get<0>(tmp) != 0) {
                int length;
                if (get<1>(tmp) != 0) {
                    string additional = binary.substr(i, get<1>(tmp));
                    reverse(additional.begin(), additional.end());
                    length = getLength(get<0>(tmp), additional);
                }
                else {
                    length = get<0>(tmp);
                }
                dist = true;
                i += get<1>(tmp);
                curr = "";

                reLength = length;
            }
            else {
                int ch = matchASCII(curr);
                if (ch != -1) {
                    curr = "";
                    ofile << hex << char(ch);
                }
                else {
                    curr += binary[i];
                    i += 1;
                }
            }
            }
        else if (!dist && curr.length() == 9) {
            int ch = matchASCII(curr);
            if (ch != -1) {
                curr = "";
                ofile << hex << char(ch);
            }
            else {
                curr += binary[i];
                i += 1;
            }
        }
        else if (dist) {
            string distStr = binary.substr(i, 5);
            tuple<int, int> tmp2 = checkDistance(distStr);
            i += 5;
            int length;
            if (get<1>(tmp2) != 0) {
                string additional = binary.substr(i, get<1>(tmp2));
                reverse(additional.begin(), additional.end());
                length = getLength(get<0>(tmp2), additional);
            }
            else {
                length = get<0>(tmp2);
            }
            dist = false;
            i += get<1>(tmp2);
            curr = "";

            length = -length;
            ofile.seekg(0, std::ios_base::end);
            ofile.seekg(length, std::ios_base::end);
            
            while (reLength > 0) {
                ofile.seekg(length, std::ios_base::end);
                char input = ofile.get();
                ofile.seekp(0, std::ios_base::end);
                ofile << hex << input;
                reLength--;
            } 
        }
        else {
            curr += binary[i];
            i += 1;
        }
    }
    return false;
}

bool decodeDynamicHuffman(string &binary, int &i, fstream &ofile, bool final) {
    int length = binary.length();

    bool dist = false;
    int reLength = 0;
    string curr = "";

    while (i < length) {
        if (curr != "") {
            if (dist) {
                int curr_num = -1;
                if (find(posDistCodes[curr.length()].begin(), posDistCodes[curr.length()].end(), curr) != posDistCodes[curr.length()].end()) {
                    curr_num = distCode[curr];
                }
                if (curr_num != -1) {
                    tuple<int, int> tmp = checkDistanceDynamic(curr_num);
                    if (get<0>(tmp) != 0) {
                        int length;
                        if (get<1>(tmp) != 0) {
                            string additional = binary.substr(i, get<1>(tmp));
                            reverse(additional.begin(), additional.end());
                            length = getLength(get<0>(tmp), additional);
                        }
                        else {
                            length = get<0>(tmp);
                        }
                        dist = false;
                        i += get<1>(tmp);
                        curr = "";
                        length = -length;
                        
                        ofile.seekg(0, std::ios_base::end);
                        ofile.seekg(length, std::ios_base::end);
        
                        while (reLength > 0) {
                            ofile.seekg(length, std::ios_base::end);
                            char input = ofile.get();
                            ofile.seekp(0, std::ios_base::end);
                            ofile << hex << input;
                            reLength--;
                        } 
                    }
                    else {
                        curr += binary[i];
                        i += 1;
                    }
                }
                else {
                    curr += binary[i];
                    i += 1;
                }
            }
            else {
                int curr_num = -1;
                if (find(posLitLenCodes[curr.length()].begin(), posLitLenCodes[curr.length()].end(), curr) != posLitLenCodes[curr.length()].end()) {
                    curr_num = litLenCode[curr];
                }
                if (curr_num != -1) {
                    if (curr_num == 256) {
                        if (final) {
                            return true;
                        }
                        else {
                            curr = "";
                            return false;
                        }
                    }
                    else if (0 <= curr_num && curr_num <= 255) {
                        ofile.seekp(0, std::ios_base::end);
                        ofile << hex << char(curr_num);
                        curr = "";
                    }
                    else if (257 <= curr_num && curr_num <= 285) {
                        tuple<int, int> tmp = checkLength(curr_num);
                        if (get<0>(tmp) != 0) {
                            int length;
                            if (get<1>(tmp) != 0) {
                                string additional = binary.substr(i, get<1>(tmp));
                                reverse(additional.begin(), additional.end());
                                length = getLength(get<0>(tmp), additional);
                            }
                            else {
                                length = get<0>(tmp);
                            }
                            dist = true;
                            i += get<1>(tmp);
                            curr = "";
                            reLength = length;
                        }
                        else {
                            curr += binary[i];
                            i += 1;
                        }
                    }
                    else {
                        curr += binary[i];
                        i += 1;
                    }
                }
                else {
                    curr += binary[i];
                    i += 1;
                }
            }
        }
        else {
            curr += binary[i];
            i += 1;
        }
    }
    return false;
}

int decodeHuffman(string binary, string outputfile) {
    bool start = true;
    bool final = false;
    bool dist = false;
    int reLength = 0;
    string curr = "";

    size_t len = binary.length();
    string result = "";

    stringstream ss;

    fstream ofile;
    ofile.open(outputfile, ios::in | ios::out | ios::binary | ios::trunc);

    int i = 0;

    while (i < len) {
        if (start) {
            if (binary[i] == '1') {
                final = true;
            }
            i += 1;
            start = false;
            if (binary.substr(i, 2) == "10") {
                i += 2;
                bool f = decodeFixedHuffman(binary, i, ofile, final);
                if (f) {
                    break;
                }
                else {
                    start = true;
                    continue;
                }
            }
            else if (binary.substr(i, 2) == "01") {
                i += 2;
                string HLITstr = binary.substr(i, 5);
                reverse(HLITstr.begin(), HLITstr.end());
                unsigned long long HLIT = std::stoull(HLITstr, 0, 2) + 257;
                i += 5;

                string HDISTstr = binary.substr(i, 5);
                reverse(HDISTstr.begin(), HDISTstr.end());
                unsigned long long HDIST = std::stoull(HDISTstr, 0, 2) + 1;
                i += 5;
                
                string HCLENstring = binary.substr(i, 4);
                reverse(HCLENstring.begin(), HCLENstring.end());
                unsigned long long HCLEN = std::stoull(HCLENstring, 0, 2) + 4;
                i += 4;

                codeLenCode.clear();
                litLenCode.clear();
                distCode.clear();

                for (int k=0; k < 8; k++) {
                    posLenCodes[k].clear();
                }
                for (int k=0; k < 19; k++) {
                    posLitLenCodes[k].clear();
                }
                for (int k=0; k < 19; k++) {
                    posDistCodes[k].clear();
                }

                string clalph = binary.substr(i, HCLEN * 3);
                createDynamicCodeAlpabet(clalph);
                i += HCLEN * 3;
                createLitLenDistAlphabet(binary, i, HLIT, HDIST);
                bool f = decodeDynamicHuffman(binary, i, ofile, final);
                if (f) {
                    break;
                }
                else {
                    start = true;
                    continue;
                }
            }
            // continue;
        }

    }
    return 0;
}


int inflate8(string filename) {
    initdArray();
    // string filename = argv[1];
    string binaryString = getBinaryString(filename);
    string outputfile = filename.substr(0, filename.find(".deflate"));
    // string outputfile = "output.txt";
    // cout << "calling output.txt" << endl;
    decodeHuffman(binaryString, outputfile);
    return 0;
}