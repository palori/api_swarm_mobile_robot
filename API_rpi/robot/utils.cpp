
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
	for(unsigned int i=0; i< delimiters.length(); i++){
		delim[i] = delimiters[i];
	}
	char * pch;
	vector<string> words;
	bool keep_reading=true, store=false;
	pch = strtok (str,delim); //" ,.-"
	//cout << "words: " << words.size() << endl;
	//cout << "pch:" << endl;
	while (pch != NULL && keep_reading){
		cout << pch << endl;
		words.push_back(pch);
		pch = strtok (NULL, delim); //" ,.-"
	}
	//cout << "words: " << words.size() << endl;
	return words;
}


/*
 * add2vector
 *
 *   accepted types:
 *		- string
 *		- int
 *		- float
 *		- bool
 */
/*
vector<string> add2vector(vector<string> v, string new_data, int max_len){
	if (v.size() >= max_len){
		v.erase (v.begin());    // only when it's full, delete the first position
	}
	v.push_back(new_data);
	return v;
}	// consider doing the same with int, float, bool...


vector<int> add2vector(vector<int> v, int new_data, int max_len){
	if (v.size() >= max_len){
		v.erase (v.begin());    // only when it's full, delete the first position
	}
	v.push_back(new_data);
	return v;
}


vector<float> add2vector(vector<float> v, float new_data, int max_len){
	if (v.size() >= max_len){
		v.erase (v.begin());    // only when it's full, delete the first position
	}
	v.push_back(new_data);
	return v;
}


vector<bool> add2vector(vector<bool> v, bool new_data, int max_len){
	if (v.size() >= max_len){
		v.erase (v.begin());    // only when it's full, delete the first position
	}
	v.push_back(new_data);
	return v;
}
*/

/* moved to Items class
template <class T>
vector<T> add2vector(vector<T> v, T new_data, int max_len){
	if (v.size() >= max_len){
		v.erase (v.begin());    // only when it's full, delete the first position
	}
	v.push_back(new_data);
	return v;
}
*/



/*
 * print_vector
 *
 *   accepted types:
 *		- string
 *		- int
 *		- float
 *		- bool
 */
/*
void print_vector(vector<string> v){
	cout << "\nVector:\n";
	for (int i = 0; i < v.size(); i++){
		cout << v.at(i) << endl;
	}
	cout << endl;
}

void print_vector(vector<int> v){
	cout << "\nVector:\n";
	for (int i = 0; i < v.size(); i++){
		cout << to_string(v.at(i)) << endl;
	}
	cout << endl;
}

void print_vector(vector<float> v){
	cout << "\nVector:\n";
	for (int i = 0; i < v.size(); i++){
		cout << to_string(v.at(i)) << endl;
	}
	cout << endl;
}

void print_vector(vector<bool> v){
	cout << "\nVector:\n";
	for (int i = 0; i < v.size(); i++){
		cout << to_string(v.at(i)) << endl;
	}
	cout << endl;
}
*/

/* moved to Items class
template <class T>
void print_vector(vector<T> v){
	cout << "\nVector:\n";
	for (int i = 0; i < v.size(); i++){
		cout << to_string(v.at(i)) << endl;
	}
	cout << endl;
}
*/

/* cant be here for dependencies!!!
template <class T>
void print_vector(vector<Item<T>> v){
	cout << "\nVector:\n";
	for (int i = 0; i < v.size(); i++){
		cout << to_string(v.at(i).get_noMutex()) << endl;
	}
	cout << endl;
}*/