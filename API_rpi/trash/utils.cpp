
#include "utils.h"

using namespace std;

/*
int str2int(string s){
	try{
        return stoi(s); 
    }
    catch (exception& e)
    {
        cout << "Exception: " << e.what() << '\n';
    }
    return BIG_INT;
}
*/

float str2float(string s){
    try{
        return stof(s); 
    }
    catch (exception& e)
    {
        cout << "Exception: " << e.what() << '\n';
    }
    return BIG_FLOAT;
}

