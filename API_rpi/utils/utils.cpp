
#include "utils.h" // comment if there are templates we include this .tpp file at the end of the .h file

int str2int(string s){
	try {return stoi(s);}
	catch (exception& e) {cout << "Exception: " << e.what() << '\n';}
	return BIG_INT;
}

float str2float(string s){
	try {return stof(s);}
	catch (exception& e) {cout << "Exception: " << e.what() << '\n';}
	return BIG_FLOAT;
}



/*
 * split_string -> working
 *
 * In:
 *      msg: string to split
 *      delimiters: contain characters that will split the 'msg'
 *
 * Out:
 *      words: vector with the splited input message
 * e.g: msg = "x=1,y=2"
 *      delimiters = "=,"
 *
 *      words (vector) = {"x", "1", "y", "2"}
 */
vector<string> split_str(string msg, string delimiters){
	// split the message
	char str[msg.length()], delim[delimiters.length()];;
	for(unsigned int i=0; i< msg.length(); i++){
		str[i] = msg[i];
	}
	//cout << "\ndelimiters:\n";
	for(unsigned int i=0; i< delimiters.length(); i++){
		delim[i] = delimiters[i];
		//cout << "  delim " << i << ": " << delim[i] << endl;
	}
	char * pch;
	vector<string> words;
	bool keep_reading=true, store=false;
	pch = strtok (str,delim); //" ,.-"
	//cout << "words: " << words.size() << endl;
	//cout << "pch:" << endl;
	while (pch != NULL && keep_reading){
		//cout << pch << endl;
		words.push_back(pch);
		pch = strtok (NULL, delim); //" ,.-"
	}
	//cout << "words: " << words.size() << endl;
	return words;
}


