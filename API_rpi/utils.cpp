
#include "utils.h"

using namespace std;

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