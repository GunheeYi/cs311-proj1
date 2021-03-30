#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <vector>

using namespace std;

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

		vector<pair<string, int>> syms;
		for(int i = 0; i < 32; i++){
			if (i==0) {
				syms.push_back(make_pair("$zero", i));
			} else if (i==1) {
				syms.push_back(make_pair("$at", i));
			} else if (i==28) {
				syms.push_back(make_pair("$gp", i));
			} else if (i==29) {
				syms.push_back(make_pair("$sp", i));
			} else if (i==30) {
				syms.push_back(make_pair("$fp", i));
			} else if (i==31) {
				syms.push_back(make_pair("$ra", i));
			} else if (i >= 2 && i <= 3) {
				syms.push_back(make_pair("$v"+to_string(i-2), i));
			} else if (i >= 4 && i <= 7) {
				syms.push_back(make_pair("$a"+to_string(i-4), i));
			} else if (i >= 8 && i <= 15) {
				syms.push_back(make_pair("$t"+to_string(i-8), i));
			} else if (i >= 16 && i <= 23) {
				syms.push_back(make_pair("$s"+to_string(i-16), i));
			} else if (i >= 24 && i <= 25) {
				syms.push_back(make_pair("$t"+to_string(i-16), i));
			} else if (i >= 26 && i <= 27) {
				syms.push_back(make_pair("$k"+to_string(i-26), i));
			}
		}

		// for (std::vector<pair<string, int>>::const_iterator i = syms.begin(); i != syms.end(); ++i) std::cout << i->first << " " << i->second << std::endl;

		vector<vector<string>> vv;

		char *a = (char*) malloc(1000);
		gets(a);
		gets(a);
		int j = 0;
		while(*(a+j)) {
			if (*(a+j)=='	') {
				printf("%s", "asdf");
			} else {
				printf("%c", *(a+j));
			}
			j++;
		}
		
		std::string s = "0xfffefffe";
		unsigned int x = std::stoul(s, nullptr, 16);
		
		


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

