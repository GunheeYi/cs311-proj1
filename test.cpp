// Your First C++ Program

#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <bitset>
#include <cmath>

using namespace std;

string toBinString(long i, size_t length) {
	return bitset<32>(i).to_string().substr(32-length, length);
}

string makeImmediate(short i) {
    bool negative = false;
    if(i<0) {
        negative = true;
        i += (short) pow(2,15);
    }
    string s = toBinString(i, 15);
    return (negative ? "1" : "0") + s;
}

int stringToInt(string s) {
    if(s.length() >=3 && s.substr(0,2)=="0x") return stoi(s.substr(2,s.length()-2), 0, 16);
    return stoi(s);
}

int makeOffsetAndAdd(vector<pair<string, unsigned int> > syms, string s) {
	int offset = 0;
	int opening = s.find("(");
	int closing = s.find(")");
	if (opening < s.length()) {
		offset = stringToInt(s.substr(0,opening));
		s = s.substr(opening+1, closing-opening-1);
	}
	for(auto i = syms.begin(); i < syms.end(); i++) {
		if (i->first==s) return i->second + offset;
	}
	return stringToInt(s)+offset;
}

int main() {

    // vector<pair<string, unsigned int> > syms;
    // syms.push_back(make_pair("asdf", 0xf));
    std::cout << makeImmediate(-3);
    return 0;
}