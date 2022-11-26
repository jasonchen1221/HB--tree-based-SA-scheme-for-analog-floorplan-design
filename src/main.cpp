#include <iostream>
#include "placer.hpp"

time_t startTime;

int main(int argc, char* argv[]){
    
    startTime = clock();
    
    Placer placer;
    // alpha, blkFile, netFile, grpFile
    //placer.parseAll(argv[1], argv[2], argv[3], argv[4]);    
        /* testing parser */
        placer.parseAlpha(argv[1], false);
        placer.parseBlkFile(argv[2], false);
        placer.parseNetFile(argv[3], false);
        placer.parseGrpFile(argv[4], false);
    
    placer.genDevice();

    return 0;
}