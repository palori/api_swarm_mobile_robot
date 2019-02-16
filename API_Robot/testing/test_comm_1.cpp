#include "../comm_1.h"


void test_comm1(){

    // 1. Need to make all methods 'public' to be able to call them from outside
    COMM comm;
    comm.debug_params();
    comm.msg2params("@,a=2,b=0");
    comm.debug_params();
}


int main(){

    test_comm1();
    return 0;
}