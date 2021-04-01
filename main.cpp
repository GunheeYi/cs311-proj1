#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <bitset>
#include <cmath>

using namespace std;

vector<pair<string, unsigned int> > syms;

long stringToInt(string s) {
    if(s.length() >=3 && s.substr(0,2)=="0x") return stoi(s.substr(2,s.length()-2), 0, 16);
    return stoi(s);
}

string toBinString(long i, size_t length) {
	if (i >= 0) return bitset<32>(i).to_string().substr(32-length, length);
	else {
		i += pow(2,length-1);
		return "1" + toBinString(i,length-1);
	}
}

long symToInt(string s) {
	for(auto i = syms.begin(); i < syms.end(); i++) {
		if (i->first==s) return i->second;
	}
	return stringToInt(s);
}

pair<long, long> toIntAndOffsetInt(string s) {
	long offset = 0;
	int opening = s.find("(");
	int closing = s.find(")");
	if (opening < s.length()) {
		offset = stringToInt(s.substr(0,opening));
		s = s.substr(opening+1, closing-opening-1);
	}
	return make_pair(symToInt(s), offset);
}

class Mold {
	public:
		std::string name, type, opcode, funct;
		
		Mold(std::string name, std::string type, unsigned int opcode, unsigned int funct) {
			this->name = name;
			this->type = type;
			this->opcode = toBinString(opcode, 6);
			if(this->type=="R") this->funct = toBinString(funct, 6);
		}
};

class MoldSet {
	public:
		std::vector<Mold> set;
		
		MoldSet() {
			set.push_back(Mold("addiu", "I", 0x9, 0x0));
			set.push_back(Mold("addu", "R", 0x0, 0x21));
			set.push_back(Mold("and", "R", 0x0, 0x24));
			set.push_back(Mold("andi", "I", 0xc, 0x0));
			set.push_back(Mold("beq", "I", 0x4, 0x0));
			set.push_back(Mold("bne", "I", 0x5, 0x0));
			set.push_back(Mold("j", "J", 0x2, 0x0));
			set.push_back(Mold("jal", "J", 0x3, 0x0));
			set.push_back(Mold("jr", "R", 0x0, 0x08));
			set.push_back(Mold("lui", "I", 0xf, 0x0));
			set.push_back(Mold("lw", "I", 0x23, 0x0));
			set.push_back(Mold("nor", "R", 0x0, 0x27));
			set.push_back(Mold("or", "R", 0x0, 0x25));
			set.push_back(Mold("ori", "I", 0xd, 0x0));
			set.push_back(Mold("sltiu", "I", 0xb, 0x0));
			set.push_back(Mold("sltu", "R", 0x0, 0x2b));
			set.push_back(Mold("sll", "R", 0x0, 0x00));
			set.push_back(Mold("srl", "R", 0x0, 0x02));
			set.push_back(Mold("sw", "I", 0x2b, 0x0));
			set.push_back(Mold("subu", "R", 0x0, 0x23));
		}
	
		string make(vector<string> inst, unsigned int textCounter) {
			
			Mold m("null", "null", 0, 0);
			for (auto i = set.begin(); i < set.end(); i++) {
				if(inst[0]==i->name) m = *i;
			}

			string rs="00000", rt="00000", rd="00000", shamt="00000", imm="0000000000000000", add="00000000000000000000000000";

			if(m.name=="addiu") {
				rt = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				imm = toBinString(symToInt(inst[3]), 16);
			} else if (m.name=="addu") {
				rd = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				rt = toBinString(symToInt(inst[3]), 5);
			} else if (m.name=="and") {
				rd = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				rt = toBinString(symToInt(inst[3]), 5);
			} else if (m.name=="andi") {
				rt = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				imm = toBinString(symToInt(inst[3]), 16);
			} else if (m.name=="beq") {
				rs = toBinString(symToInt(inst[1]), 5);
				rt = toBinString(symToInt(inst[2]), 5);
				imm = toBinString((symToInt(inst[3])-textCounter)/4-1, 16);
			} else if (m.name=="bne") {
				rs = toBinString(symToInt(inst[1]), 5);
				rt = toBinString(symToInt(inst[2]), 5);
				imm = toBinString((symToInt(inst[3])-textCounter)/4-1, 16);
			} else if (m.name=="j") {
				add = toBinString(symToInt(inst[1])/4, 26);
			} else if (m.name=="jal") {
				add = toBinString(symToInt(inst[1])/4, 26);
			} else if (m.name=="jr") {
				rs = toBinString(symToInt(inst[1]), 5);
			} else if (m.name=="lui") {
				rt = toBinString(symToInt(inst[1]), 5);
				imm = toBinString(symToInt(inst[2]), 16); 
			} else if (m.name=="lw") {
				rt = toBinString(symToInt(inst[1]), 5);
				pair<long, long> p = toIntAndOffsetInt(inst[2]);
				rs = toBinString(p.first, 5);
				imm = toBinString(p.second, 16);
			} else if (m.name=="nor") {
				rd = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				rt = toBinString(symToInt(inst[3]), 5);
			} else if (m.name=="or") {
				rd = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				rt = toBinString(symToInt(inst[3]), 5);
			} else if (m.name=="ori") {
				rt = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				imm = toBinString(symToInt(inst[3]), 16);
			} else if (m.name=="sltiu") {
				rt = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				imm = toBinString(symToInt(inst[3]), 16);
			} else if (m.name=="sltu") {
				rd = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				rt = toBinString(symToInt(inst[3]), 5);
			} else if (m.name=="sll") {
				rd = toBinString(symToInt(inst[1]), 5);
				rt = toBinString(symToInt(inst[2]), 5);
				shamt = toBinString(symToInt(inst[3]), 5);
			} else if (m.name=="srl") {
				rd = toBinString(symToInt(inst[1]), 5);
				rt = toBinString(symToInt(inst[2]), 5);
				shamt = toBinString(symToInt(inst[3]), 5);
			} else if (m.name=="sw") {
				rt = toBinString(symToInt(inst[1]), 5);
				pair<long, long> p = toIntAndOffsetInt(inst[2]);
				rs = toBinString(p.first, 5);
				imm = toBinString(p.second, 16);
			} else if (m.name=="subu") {
				rd = toBinString(symToInt(inst[1]), 5);
				rs = toBinString(symToInt(inst[2]), 5);
				rt = toBinString(symToInt(inst[3]), 5);
			}

			if (m.type=="R") {
				return m.opcode + rs + rt + rd + shamt + m.funct;
			} else if (m.type=="I") {
				return m.opcode + rs + rt + imm;
			} else if (m.type=="J") {
				return m.opcode + add;
			}
			return "";
		}
};

char * getsNew(char *s, size_t size) {
  char *r;
  int len;

  r = fgets(s, size, stdin);
  if (!r)
    return r;

  len = strlen(s);
  if (s[len - 1] == '\n') s[len - 1] = '\0'; //replace new line with null

  return s;
}

int main(int argc, char* argv[]){

	if(argc != 2){
		printf("Usage: ./runfile <assembly file>\n"); //Example) ./runfile /sample_input/example1.s
		printf("Example) ./runfile ./sample_input/example1.s\n");
		exit(0);
	}
	else
	{
		
		// To help you handle the file IO, the deafult code is provided.
		// If we use freopen, we don't need to use fscanf, fprint,..etc. 
		// You can just use scanf or printf function 
		// ** You don't need to modify this part **
		// If you are not famailiar with freopen,  you can see the following reference
		// http://www.cplusplus.com/reference/cstdio/freopen/

		//For input file read (sample_input/example*.s)

		char *file=(char *)malloc(strlen(argv[1])+3);
		strncpy(file,argv[1],strlen(argv[1]));

		if(freopen(file, "r",stdin)==0){
			printf("File open Error!\n");
			exit(1);
		}

		//From now on, if you want to read string from input file, you can just use scanf function.
		// for (std::vector<pair<string, int>>::const_iterator i = syms.begin(); i != syms.end(); ++i) std::cout << i->first << " " << i->second << std::endl;

		vector<vector<string> > vv;

		int i = 0;
		char *a = (char*) malloc(1000);
		while(getsNew(a, 1000)) {
			vector<string> v;
			string s = "";
			for (int j = 0; j < strlen(a); j++) {
				if(*(a+j)=='	' || *(a+j)==' ' || *(a+j)==',') {
					if(!s.empty()) { 
						v.push_back(s);
						s = "";
					}
					if(*(a+j)==0) break;
				} else {
					s += *(a+j);
				}
			}
			if(!s.empty()) v.push_back(s);
			vv.push_back(v);
			i++;
		}

		string data = "";
		string text = "";

		for(unsigned int i = 0; i < 32; i++) {
			syms.push_back(make_pair("$"+to_string(i), i));
		}

		vector<vector<string> > insts;
		long dataCounter = 0x10000000;
		long textCounter = 0x400000;
		string reading = "";
		for(auto ii = vv.begin(); ii < vv.end(); ii++) {
			vector<string> line = *ii;
			if(line[0]==".data") {
				reading = "data";
				continue;
			} else if(line[0]==".text") {
				reading = "text";
				continue;
			}
			if(reading=="data") {
				if(line.size()==3) syms.push_back(make_pair(line[0].substr(0, line[0].length()-1), dataCounter));
				data += toBinString(stringToInt(line[line.size()-1]), 32);
				dataCounter += 4;
				continue;
			} else if(reading=="text") {
				if(line.size()==1) {
					syms.push_back(make_pair(line[0].substr(0, line[0].length()-1), textCounter));
					continue;
				}
				if(!line.empty()) {
					if (line[0]=="la") {
						long memInt = symToInt(line[2]);
						// string wholeAddStr = toBinString(memInt, 32);
						// string lowerAddStr = wholeAddStr.substr(16,16);
						vector<string> upper{"lui", line[1], to_string(memInt/0x10000)};
						insts.push_back(upper);
						if(memInt%0x10000!=0) {
							textCounter += 4;
							vector<string> lower{"ori", line[1], line[1], to_string(memInt%0x10000)};
							insts.push_back(lower);
						}
					} else {
						insts.push_back(line);
					}
					textCounter += 4;
				}
			}
		}
		
		MoldSet ms;
		textCounter = 0x400000;
		for(auto i = insts.begin(); i < insts.end(); i++) {
			string lineTranslated =  ms.make(*i, textCounter);
			text += lineTranslated;
			textCounter += 4;
		}

		long textSize = text.length() / 8;
		long dataSize = data.length() / 8;

		string full = toBinString(textSize, 32) + toBinString(dataSize,32) + text + data;
		
		file[strlen(file)-1] ='o';
		freopen(file,"w",stdout);

		//If you use printf from now on, the result will be written to the output file.

		printf("%s", full.c_str()); 
		

	}
	return 0;
}

