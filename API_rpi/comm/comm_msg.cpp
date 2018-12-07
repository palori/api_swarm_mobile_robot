
#include "comm_msg.h"


// this should go in utils
float str2float(string s){
    try{
        return stof(s); 
    }
    catch (exception& e)
	{
	    cout << "Exception: " << e.what() << '\n';
	}
	return NULL;
}

void msg2target(target & new_pose, string msg, bool print_msg){

    // split the message
    char str[msg.length()];
    for(unsigned int i=0; i< msg.length(); i++){
    	str[i] = msg[i];
    }
    char * pch;
    vector<string> words;
    bool keep_reading=true, store=false;
    pch = strtok (str," ,="); //" ,.-"
    while (pch != NULL && keep_reading)
    {
        //usleep(1000000); //microseconds
        
        //cout << c[0];
        if(*pch=='@' && !store){
            store=true;
        }
        else if(*pch=='$' && store){
            keep_reading=false;
            store = false;
        }
        else if(*pch!='$' && store) {
            words.push_back(pch);
            if (print_msg){
            	printf ("%s\n",pch);
            }
        }
        pch = strtok (NULL, " ,="); //" ,.-"
    }

    // save it as new target pose
    for (int i=0; i<words.size(); i++){
        if(words.at(i) == "s"){
            float val = str2float(words.at(i+1));
            if (val != NULL) {
                new_pose.servo = val;
                i++;
            }
        }
        else if(words.at(i) == "x"){
            float val = str2float(words.at(i+1));
            if (val != NULL) {
                new_pose.x = val;
                i++;
            }
        }
        else if(words.at(i) == "y"){
            float val = str2float(words.at(i+1));
            if (val != NULL) {
                new_pose.y = val;
                i++;
            }
        }
        else if(words.at(i) == "th"){
            float val = str2float(words.at(i+1));
            if (val != NULL) {
                new_pose.th = val;
                i++;
            }
        }
    }

    if (print_msg){ // show that results are saved
    	print_target(new_pose);
    }
}





