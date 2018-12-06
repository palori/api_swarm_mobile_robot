
#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <unistd.h> // time sleep microseconds

//#include <Arduino.h>
using namespace std;



void test_strtok ()
{
  char str[] ="00000000000,@,s=500,x=0.22,y=0.53,th=0.08,$,0000"; //"- This, a sample string.";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);

  vector<string> words;

  bool keep_reading=true, store=false;
  
  pch = strtok (str," ,="); //" ,.-"
  while (pch != NULL && keep_reading)
  {
    //usleep(1000000); //microseconds
    
    //cout << c[0];
    if(*pch=='@' && !store){
        store=true;
        printf("store=true\n");
    }
    else if(*pch=='$' && store){
        keep_reading=false;
        store = false;
    }
    else if(*pch!='$' && store) {
        words.push_back(pch);
        printf ("%s\n",pch);
    }
    pch = strtok (NULL, " ,="); //" ,.-"
  }

}

void test_strings_and_floats(float x){
    string xs = to_string(x);
    float xf = stof(xs);
    cout << "x:  " << x << endl;
    cout << "xs: " << xs << endl;
    cout << "xf: " << xf << endl;
}

void test_substrings(){
    string s = "hola, com estas?";
    printf("%s\n", s.substr(0,1));
}

void test_str2char(){
	string str = "string";
	vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1); //str2vector_char

	cout << str << " == ";
	for (int i=0; i<cstr.size(); i++){
		cout << cstr[i];
	}
	cout << endl;
}

int main(){
	//test_substrings();					//working
    //test_strings_and_floats(0.28);		//working
    //test_str2char();						//working
    //test_strtok();						//working
}