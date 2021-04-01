#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <bitset>

using namespace std;

string toBinString(unsigned int i, size_t length) {
	return bitset<32>(i).to_string().substr(32-length, length);
}

class InstWithAdds {
	public:
		string name;
		vector<unsigned int> adds;

		InstWithAdds(string name, vector<unsigned int> adds) {
			this->name = name;
			this->adds = adds;
		}
};

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
	
		string make(InstWithAdds inst) {
			if(inst.name=="la") {
				unsigned int regAdd = inst.adds[0];
				unsigned int memAdd = inst.adds[1];
				string wholeAddStr = toBinString(memAdd, 8);
				string lowerAddStr = wholeAddStr.substr(4,4);
				if(lowerAddStr=="0000") {
					return make(InstWithAdds("lui", vector<unsigned int>{regAdd, memAdd >> 16}));
				} else {
					return make(InstWithAdds("lui", vector<unsigned int>{regAdd, memAdd >> 16})) + make(InstWithAdds("ori", vector<unsigned int>{regAdd, memAdd << 16 >> 16}));
				}
			}

			Mold m("null", "null", 0, 0);
			for (auto i = set.begin(); i < set.end(); i++) {
				if(inst.name==i->name) m = *i;
			}

			if (m.type=="R") {
				return m.opcode + toBinString(inst.adds[0], 5);
			}
			return m.name + m.type;
		}
};

string symToBin(std::string s) {
	if (s[0]=='$') cout << "Register" << endl;
	else cout << "Others" << endl;

	return "";
}

unsigned int getAdd(vector<pair<string, unsigned int> > syms, string s) {
	unsigned int offset = 0;
	unsigned int opening = s.find("(");
	unsigned int closing = s.find(")");
	if (opening < s.length()) {
		offset = stoi(s.substr(0,opening));
		s = s.substr(opening+1, closing-opening-1);
	}
	for(auto i = syms.begin(); i < syms.end(); i++) {
		if (i->first==s) return i->second + offset;
	}
	return stoi(s)+offset;
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
		while(gets(a)) {
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
			//a = (char*) malloc(1000);
		}
		
		// // vector print test
		// for (auto ii = vv.begin(); ii < vv.end(); ii++) {
		// 	for (auto i = ii->begin(); i < ii->end(); i++)
		// 	{
		// 		cout << *i << " ";
		// 	}
		// 	cout << endl;
		// }

		string data = "";
		string text = "";

		vector<pair<string, unsigned int> > syms;
		for(unsigned int i = 0; i < 32; i++) {
			syms.push_back(make_pair("$"+to_string(i), i));
		}

		vector<vector<string> > instsWithSyms;
		unsigned int dataCounter = 0x10000000;
		unsigned int textCounter = 0x400000;
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
				if(line.size()==3) syms.push_back(make_pair(line[0], dataCounter));
				dataCounter += 4;
				continue;
			} else if(reading=="text") {
				if(line.size()==1) {
					syms.push_back(make_pair(line[0], textCounter));
					continue;
				}

				instsWithSyms.push_back(line);
				textCounter += 4;
			}
		}

		vector<InstWithAdds> instsWithAdds;
		for(auto i = instsWithSyms.begin(); i < instsWithSyms.end(); i++) {
			vector<string> line = *i;
			InstWithAdds inst("", vector<unsigned int>{});
			inst.name = line[0];
			for(auto j = line.begin()+1; j < line.end(); j++) {
				inst.adds.push_back(getAdd(syms, *j));
			}
		}

		// for (auto i = syms.begin(); i < syms.end(); i++)
		// {
		// 	cout << i->first << "	" << toBinString(i->second, 32) << endl;
		// }

		MoldSet ms;
		for (auto i = instructions.begin(); i < instructions.end(); i++)
		{
			cout << ms.make(*i) << endl;
		}

		
		
		// std::string s = "0xfffefffe";
		// unsigned int x = std::stoul(s, nullptr, 16);

		// symToBin("$21");
		// symToBin("asdf");

		// For output file write 
		// You can see your code's output in the sample_input/example#.o 
		// So you can check what is the difference between your output and the answer directly if you see that file
		// make test command will compare your output with the answer
		file[strlen(file)-1] ='o';
		freopen(file,"w",stdout);

		//If you use printf from now on, the result will be written to the output file.

		printf("Hello World!\n"); 
		

	}
	return 0;
}

