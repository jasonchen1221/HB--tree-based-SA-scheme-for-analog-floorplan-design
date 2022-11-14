#include <iostream>
#include "placer.hpp"

time_t startTime;

int main(int argc, char* argv[]){
    
    startTime = clock();
    
    Placer placer;
    // alpha, blkFile, netFile, grpFile
    placer.parseAll(argv[1], argv[2], argv[3], argv[4]);    

    return 0;
}