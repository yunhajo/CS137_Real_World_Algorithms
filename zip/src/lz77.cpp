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
using namespace std;

map<string, vector<int>> hashTable;

string lz77(string fileContent) {
    string match = "";
    string buffer = "";
    string first = fileContent.substr(0,3);
    hashTable[first].push_back(0);
    string result = "";

    int i = 0;
    int length = fileContent.length();
    cout << length << endl;

    while (i < length) {
        cout<< "i is " << i << " " << fileContent[i] << endl;
        if (i < length - 2) {
            string next = fileContent.substr(i+1, 3);
            // cout << i << " Next " << next << " end" << endl;

            if (hashTable[next].size() != 0) {
                string max_match = next;
                int lenMatch = 3;
                int distMatch = -1;
                int idxAtMatch = -1;
                // if (next == "   ") {
                //         cout << " Max match initial " << max_match.length() << endl;
                // }
                //sort( hashTable[next].begin(), hashTable[next].end() );
                //hashTable[next].erase( unique( hashTable[next].begin(), hashTable[next].end() ), hashTable[next].end() );
                set<int> s;
                unsigned size1 = hashTable[next].size();
                for( unsigned i = 0; i < size1; ++i ) s.insert( hashTable[next][i] );
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
                                string tmp = fileContent.substr(curr, 3);
                                hashTable[tmp].push_back(curr);
                            }
                            curr_match += fileContent[curr];
                            idx++;
                            curr++;
                        }
                        if (next == "   ") {
                            cout << idx << " " << curr_match.length() << " " << max_match.length() << " " << idxAtMatch << endl;
                        }
                        // && idx > idxAtMatch
                        if (curr_match.length() >= max_match.length()) { 
                            bool b = (i+1) - indices[j] < distMatch;
                            if ((distMatch != -1 && ((i+1) - indices[j] > distMatch)) && (curr_match.length() == max_match.length())) {
                                continue;
                            }
                            max_match = curr_match;
                            // cout << max_match << endl;
                            lenMatch = curr_match.length();
                            distMatch = ((i+1) - indices[j]);
                            idxAtMatch = indices[j];
                            // }
                        }
                    }
                    if (curr_match.length() >= 258) {
                        break;
                    }
                }
                if (distMatch != -1) {
                    // cout<< "distMatch " << endl;
                    if (i >= buffer.length()) {
                        result += fileContent[i];
                        buffer += fileContent[i];
                    }
                    cout << lenMatch << " , " << distMatch << endl;
                    // cout << "Max match " << max_match << " " << max_match.length() << endl;
                    buffer += max_match;
                    result += "<";
                    result += to_string(lenMatch);
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
                    }
                }
                else {
                    // cout<< "Here 2 " << endl;
                    result += fileContent[i];
                    buffer += fileContent[i];
                    i++;
                }
                hashTable[next].push_back(i);
            }
            else {
                // cout<< "Here 3 " << endl;
                result += fileContent[i];
                buffer += fileContent[i];
                i++;
            }
            hashTable[next].push_back(i);
        }
        else {
            // cout<< "Here 4 " << endl;
            if (i >= buffer.length()) {
                result += fileContent[i];
                buffer += fileContent[i];
            }
            i++;
        }
    }
    cout << result << endl;
    return result;
}

int main(int argc, char *argv[]) {
    string zip_filename = argv[1];
    ifstream file(zip_filename); 
    string fileContent((istreambuf_iterator<char>(file)), 
                        istreambuf_iterator<char>()); 
    
    string result = lz77(fileContent);
    ofstream outFile(zip_filename + ".lz77");
    outFile << result;
    outFile.close();
    return 0;
}