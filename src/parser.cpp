#include "placer.hpp"

#include <fstream>

void Placer::parseAll(std::string alpha, std::string blkFile, std::string netFile, std::string grpFile){
    
    if(parseAlpha(alpha) == false){
        std::cerr << "Error: Fail to parse alpha." << std::endl;
        exit(EXIT_FAILURE);
    }

    if(parseBlkFile(blkFile) == false){
        std::cerr << "Error: Fail to parse: " << blkFile << std::endl;
        exit(EXIT_FAILURE);
    }

    if(parseNetFile(netFile) == false){
        std::cerr << "Error: Fail to parse: " << netFile << std::endl;
        exit(EXIT_FAILURE);
    }

    if(parseGrpFile(grpFile) == false){
        std::cerr << "Error: Fail to parse: " << grpFile << std::endl;
        exit(EXIT_FAILURE);
    }

}

bool Placer::parseAlpha(std::string alpha){
    m_alpha = std::stod(alpha);
    if(m_alpha < 0 || m_alpha >= 1){
        std::cerr << "Error: Alpha must be in the range: 0 ~ 1" << std::endl;
        return false;
    }
    return true;
}

bool Placer::parseBlkFile(std::string blkFile){
    std::ifstream inFile(blkFile, std::ios::in);
    if(!inFile){
        std::cerr << "Error: Fail to open: " << blkFile << std::endl;
        return false;
    }

    /* parse outline */

    /* parse # of blks */
    /* parse # of pins */
    /* parse blks */
    /* parse pins */

    inFile.close();
    return true;
}

bool Placer::parseNetFile(std::string netFile){
    
}

bool Placer::parseGrpFile(std::string grpFile){

}

void Placer::splitString(std::string str, std::vector<std::string>& vec){
    std::vector<std::string> words;
    int tmp = 0, i = 0, width = 0;
    std::string word;

    while( i < str.length() ){
        while( (str[i] == ' ') || (str[i] == '\t') || (str[i] == '\r') ){
            i++;
        }
        tmp = i;
        width = 0;
        while( (str[i] != ' ') && (str[i] != '\t') && (str[i] != '\r') && (i < str.length()) ){
            i++;
            width++;
        }
        word = str.substr(tmp,width);
        words.push_back(word);
    }
    vec = words;
}