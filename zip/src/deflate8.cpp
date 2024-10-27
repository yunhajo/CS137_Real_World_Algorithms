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
#include <set>
#include "deflate8.h"
using namespace std;

int dArray[28];

map<string, vector<int>> hashTable;

string lz77(string fileContent) {
    string match = "";
    string buffer = "";
    string first = fileContent.substr(0,3);
    hashTable[first].push_back(0);
    string result = "";

    int i = 0;
    int length = fileContent.length();

    while (i < length) {
        bool twofivesize = false;
        if (i < length - 2) {
            string next = fileContent.substr(i+1, 3);

            if (hashTable[next].size() != 0) {
                string max_match = next;
                int lenMatch = 3;
                int distMatch = -1;
                int idxAtMatch = -1;

                set<int> s;
                unsigned size1 = hashTable[next].size();
                for( unsigned k = 0; k < size1; ++k ) s.insert( hashTable[next][k] );
                hashTable[next].assign( s.begin(), s.end() );
                int size = hashTable[next].size();
                vector<int> indices = hashTable[next];

                for (int j=size-1; j >= 0; j--) {
                    string curr_match = "";
                    int curr = i + 1;
                    int idx = indices[j];
                    if (curr - idx <= 32768) {
                        while (fileContent[idx] == fileContent[curr] && curr_match.length() < 258 && curr < length) {
                            if (curr + 3 < length) {
                                string tmp2 = fileContent.substr(curr, 3);
                                hashTable[tmp2].push_back(curr);
                            }
                            curr_match += fileContent[curr];
                            idx++;
                            curr++;
                        }
                        if (curr_match.length() >= max_match.length()) { 
                            bool b = (i+1) - indices[j] < distMatch;
                            if ((distMatch != -1 && ((i+1) - indices[j] > distMatch)) && (curr_match.length() == max_match.length())) {
                                continue;
                            }
                            max_match = curr_match;
                            lenMatch = curr_match.length();
                            distMatch = ((i+1) - indices[j]);
                            idxAtMatch = indices[j];
                        }
                    }
                    if (curr_match.length() >= 258) {
                        break;
                    }
                }
                if (distMatch != -1) {
                    if (i >= buffer.length()) {
                        result += fileContent[i];
                        buffer += fileContent[i];
                    }
                    buffer += max_match;
                    result += "<";
                    result += to_string(max_match.length());
                    result += ",";
                    result += to_string(distMatch);
                    result += ">";

                    i += lenMatch;
                    if (i < length - 2) {
                        string newNext = fileContent.substr(i+1, 3);
                        if (hashTable[newNext].size() == 0) { 
                            hashTable[newNext].push_back(i+1);
                            i++;
                        }
                        else {
                            int exists = -1;
                            int iSize = hashTable[newNext].size();
                            for (int l=0; l < iSize; l++) {
                                if (i - hashTable[newNext][l] <= 32768) {
                                    exists = 1;
                                    break;
                                }
                            }
                            if (exists == -1) {
                                hashTable[newNext].push_back(i+1);
                                i++;
                            }
                        }
                    }
                }
                else {
                    result += fileContent[i];
                    buffer += fileContent[i];
                    i++;
                    hashTable[next].push_back(i);
                }
            }
            else {
                result += fileContent[i];
                buffer += fileContent[i];
                i++;
                hashTable[next].push_back(i);
            }
        }
        else {
            if (i >= buffer.length()) {
                result += fileContent[i];
                buffer += fileContent[i];
            }
            i++;
        }
    }
    return result;
}


void initdArray() {
    int exp = 1;
    for (int i = 0; i < 27; i += 2) {
        dArray[i] = pow(2.0, exp);
        dArray[i+1] = pow(2.0, exp);
        exp += 1;
    }
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

int RoundUp(int &x)
{
    return (((x + 7) & (-8)) - x);
}

string getLengthString(int length) {
    if (length >= 3 && length <= 10) {
        string lenStr = decToBinary(length + 254 - 256, 7);
        return lenStr;
    }
    else if (length >= 11 && length <= 18) {
        int tmp = floor((length - 11) / 2);
        string lenStr = decToBinary(265 + tmp - 256, 7);
        string additional = decToBinary(length - (11 + 2 * tmp), 1);
        reverse(additional.begin(), additional.end());
        return lenStr + additional;
    }
    else if (length >= 19 && length <= 34) {
        int tmp = floor((length - 19) / 4);
        string lenStr = decToBinary(269 + tmp - 256, 7);
        string additional = decToBinary(length - (19 + 4 * tmp), 2);
        reverse(additional.begin(), additional.end());
        return lenStr + additional;
    }
    else if (length >= 35 && length <= 66) {
        int tmp = floor((length - 35) / 8);
        string lenStr = decToBinary(273 + tmp - 256, 7);
        string additional = decToBinary(length - (35 + 8 * tmp), 3);
        reverse(additional.begin(), additional.end());
        return lenStr + additional;
    }
    else if (length >= 67 && length <= 130) {
        int tmp = floor((length - 67) / 16);
        string lenStr;
        if (length >= 115 && length <= 130) {
            lenStr = decToBinary(280 - 88, 8);
        }
        else {
            lenStr = decToBinary(277 + tmp - 256, 7);
        }
        string additional = decToBinary(length - (67 + 16 * tmp), 4);
        reverse(additional.begin(), additional.end());
        return lenStr + additional;
    }
    else if (length >= 131 && length <= 257) {
        int tmp = ceil((length - 131) / 32);
        string lenStr = decToBinary(281 + tmp - 88, 8);
        string additional = decToBinary(length - (131 + 32 * tmp), 5);
        reverse(additional.begin(), additional.end());
        return lenStr + additional;
    }
    else if (length == 258) {
        string lenStr = decToBinary(285 - 88, 8);
        return lenStr;
    }
    else {
        throw std::invalid_argument( "length out of range" );
    }
}

string getDistanceString(int distance) {
    if (distance >= 1 && distance <= 4) {
        string result = decToBinary(distance - 1, 5);
        return result;
    }
    int tmp = distance - 5;

    size_t counter = 0;
    while (tmp >= 0) {
        tmp -= dArray[counter];
        counter += 1;
    }

    string result = decToBinary(counter + 3, 5);

    int val = 5;
    for (int i = 0; i < counter - 1; i++) {
        val += dArray[i];
    }

    int add = ((counter + 3) / 2) - 1;
    string additional = decToBinary(distance - val, add);
    reverse(additional.begin(), additional.end());
    return result + additional;
}

int deflate8(string zip_filename) {
    cout << "Calling deflate8" << endl;
    // string zip_filename = argv[1];
    ifstream file(zip_filename); 
    string fileString((istreambuf_iterator<char>(file)), 
                        istreambuf_iterator<char>());

    string result = "110";

    string fileContent = lz77(fileString);
    int len = fileContent.length();
    // cout<< fileContent << endl;
    int i = 0;
    initdArray();

    // cout << len << endl;

    while (i < len) {
        // // cout << "i is " << i << endl;
        if (fileContent[i] == '<') {
            if (i < len - 3 && isdigit(fileContent[i+1]) && fileContent[i+2] == ',') {
                int lenNum = fileContent[i+1] - '0';
                i += 3;

                string dStr = "";
                while (fileContent[i] != '>') {
                    dStr += fileContent[i];
                    i += 1;
                }
                int dist = stoi(dStr);

                string lenStr = getLengthString(lenNum);
                string distStr = getDistanceString(dist);

                // // cout << "length is " << lenNum << " lenStr is " << lenStr << endl; 
                // // cout << "distance is " << dist << " distStr is " << distStr << endl; 

                string lenDist = lenStr + distStr;
                result += lenDist;
                i += 1;
                continue;
            }
            else if (i < len - 4 && isdigit(fileContent[i+1]) && isdigit(fileContent[i+2]) && fileContent[i+3] == ',') {
                int lenNum = stoi(fileContent.substr(i+1, i+2));
                i += 4;

                string dStr = "";
                while (fileContent[i] != '>') {
                    dStr += fileContent[i];
                    i += 1;
                }
                int dist = stoi(dStr);
                string lenStr = getLengthString(lenNum);
                string distStr = getDistanceString(dist);

                // // cout << "length is " << lenNum << " lenStr is " << lenStr << endl; 
                // // cout << "distance is " << dist << " distStr is " << distStr << endl; 

                string lenDist = lenStr + distStr;
                result += lenDist;
                i += 1;
                continue;
            }
            else if (i < len - 4 && isdigit(fileContent[i+1]) && isdigit(fileContent[i+2]) 
            && isdigit(fileContent[i+3]) && fileContent[i+4] == ',') {
                int lenNum = stoi(fileContent.substr(i+1, i+3));
                i += 5;

                string dStr = "";
                while (fileContent[i] != '>') {
                    dStr += fileContent[i];
                    i += 1;
                }
                int dist = stoi(dStr);

                string lenStr = getLengthString(lenNum);
                string distStr = getDistanceString(dist);

                // // cout << "length is " << lenNum << " lenStr is " << lenStr << endl; 
                // // cout << "distance is " << dist << " distStr is " << distStr << endl; 

                string lenDist = lenStr + distStr;
                result += lenDist;
                i += 1;
                continue;
            }
        }

        uint8_t c = uint8_t(fileContent[i]);
        if (0 <= c && c <= 143) {
            string binary = decToBinary(c + 48, 8);
            result += binary;
            // // cout << "Char is " << (unsigned) c << " " << binary << endl;
            i += 1;
        }
        else if (144 <= c && c <= 255) {
            string binary = decToBinary(c + 256, 9);
            result += binary;
            // // cout << "Char is " << (unsigned) c << " " << binary << endl;
            i += 1;
        }
    }

    result += "0000000";
    int finalLen = result.length();
    int nearEight = RoundUp(finalLen);

    for (int j=0; j < nearEight; j++) {
        result += "0";
    } 

    // cout << result << endl;

    ofstream open(zip_filename + ".deflate", ios::out | ios::trunc);

    for (int j=0; j < result.length(); j+=8) {
        string byte = result.substr(j, 8);
        reverse(byte.begin(), byte.end());
        unsigned long long tmp = std::stoull(byte, 0, 2);
        open << hex << char(tmp);
    }

    return 0;
}