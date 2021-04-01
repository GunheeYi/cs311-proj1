// Your First C++ Program

#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <bitset>

using namespace std;

int stringToInt(string s) {
    if(s.length() >=3 && s.substr(0,2)=="0x") return stoi(s.substr(2,s.length()-2), 0, 16);
    return stoi(s);
}

long getAdd(vector<pair<string, unsigned int> > syms, string s) {
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

    vector<pair<string, unsigned int> > syms;
    syms.push_back(make_pair("asdf", 0xf));
    std::cout << getAdd(syms, "-4(-24)");
    return 0;
}