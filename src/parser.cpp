#include "placer.hpp"

#include <fstream>
#include <assert.h>

void Placer::parseAll(std::string alpha, std::string blkFile, std::string netFile){
    cout << "> Parsing..." << endl;

    if(parseAlpha(alpha, false) == false){
        std::cerr << "Error: Fail to parse alpha." << std::endl;
        exit(EXIT_FAILURE);
    }

    if(parseBlkFile(blkFile, false) == false){
        std::cerr << "Error: Fail to parse: " << blkFile << std::endl;
        exit(EXIT_FAILURE);
    }

    if(parseNetFile(netFile, false) == false){
        std::cerr << "Error: Fail to parse: " << netFile << std::endl;
        exit(EXIT_FAILURE);
    }

    cout << "> Parsing...[Finished]" << endl;
    
    calTTLArea();
}

bool Placer::parseAlpha(std::string alpha, bool verbose){
    m_alpha = std::stod(alpha);
    if(m_alpha < 0 || m_alpha >= 1){
        std::cerr << "Error: Alpha must be in the range: 0 ~ 1" << std::endl;
        return false;
    }

    if(verbose){
        std::cout << "Verbose On: " << "Alpha: " << m_alpha << std::endl;
    }

    return true;
}

bool Placer::parseBlkFile(std::string blkFile, bool verbose){
    std::ifstream inFile(blkFile, std::ios::in);
    if(!inFile){
        std::cerr << "Error: Fail to open: " << blkFile << std::endl;
        return false;
    }

    std::string str;
    std::vector<std::string> vec_str;
    
    /* parse outline */
    getline(inFile, str);
    splitString(str, vec_str);
    m_outlineW = std::stod(vec_str[1]);
    m_outlineH = std::stod(vec_str[2]);
    
    /* parse # of blks */
    vec_str.clear();
    getline(inFile, str);
    splitString(str, vec_str);
    int numBlks = std::stoi(vec_str[1]);
    
    m_vBlockList.resize(numBlks);
    for(int i = 0; i < numBlks; ++i){
        m_vBlockList[i] = new Block;
    }
    m_blockHashSet.init(getHashSize(numBlks));

    /* parse # of pins */
    vec_str.clear();
    getline(inFile, str);
    splitString(str, vec_str);
    int numPins = std::stoi(vec_str[1]);
    
    m_vPinList.resize(numPins);
    for(int i = 0; i < numPins; ++i){
        m_vPinList[i] = new Pin;
    }
    m_PinHashSet.init(getHashSize(numPins));

    /* parse blks */
    getline(inFile, str);
    for(int i = 0; i < m_vBlockList.size(); ++i){
        vec_str.clear();
        getline(inFile, str);
        splitString(str, vec_str);
        /* Name */
        m_vBlockList[i]->setName(vec_str[0]);
        /* BlkW */
        m_vBlockList[i]->setW(std::stod(vec_str[1]));
        /* BlkH */
        m_vBlockList[i]->setH(std::stod(vec_str[2]));
       
        /* Insert to HashSet */
        if(m_blockHashSet.insert(BlockV(i, vec_str[0])) == false){
            std::cerr << "Error: Two Blocks have same name: " << vec_str[0] << std::endl;
            return false;
        }
    }
    
    /* parse pins */
    if(numPins != 0){
        getline(inFile, str);
        for(int i = 0; i < m_vPinList.size(); ++i){
            vec_str.clear();
            getline(inFile, str);
            splitString(str, vec_str);
            /* Name */
            m_vPinList[i]->setName(vec_str[0]);
            /* X */
            m_vPinList[i]->setX(std::stod(vec_str[2]));
            /* Y */
            m_vPinList[i]->setY(std::stod(vec_str[3]));
            /* Insert to HashSet */
            if(m_PinHashSet.insert(PinV(i, vec_str[0])) == false){
                std::cerr << "Error: Two Pins have same name: " << vec_str[0] << std::endl;
            return false;
            }
        }
    }
    
    if(verbose){
        cout << " -------------------Blocks Info------------------- " << endl;
        printBlockList();
        cout << " -------------------Pins Info------------------- " << endl;
        printPinList();
    }
    
    inFile.close();
    return true;
}

bool Placer::parseNetFile(std::string netFile, bool verbose){

    std::ifstream inFile(netFile, std::ios::in);
    if(!inFile){
        std::cerr << "Error: Fail to open: " << netFile << std::endl;
        return false;
    }

    std::string str;
    std::vector<std::string> vec_str;
    
    /* parse number of nets */
    getline(inFile, str);
    splitString(str, vec_str);
    int numNets = std::stoi(vec_str[1]);
    m_vNetList.resize(numNets);
    for(int i = 0; i < numNets; ++i){
        m_vNetList[i] = new Net;
    }

    cout << "debug0" << endl;
    /* parse nets */
    for(int i = 0; i < numNets; ++i){
        
        vec_str.clear();
        getline(inFile, str);
        splitString(str, vec_str);
        int netDegree = std::stoi(vec_str[1]);
        
        std::string tmpName;
        for(int j = 0; j < netDegree; ++j){
            
            vec_str.clear();
            getline(inFile, str);
            splitString(str, vec_str);
            tmpName = vec_str[0];

            BlockV tmpBlockV(-1, tmpName);
            PinV tmpPinV(-1, tmpName);

            // Check if in _blockList
            if(m_blockHashSet.query(tmpBlockV)){
                assert(tmpBlockV.getNo() >= 0);
                m_vNetList[i]->addBlk(m_vBlockList[tmpBlockV.getNo()]);
            }
            // Check if in _terminalList
            else if(m_PinHashSet.query(tmpPinV)){
                assert(tmpPinV.getNo() >= 0);
                m_vNetList[i]->addPin(m_vPinList[tmpPinV.getNo()]);
            }
            else{
                cerr << "Error: Neither a block nor a terminal is named \"" << tmpName << "\"." << endl;
                return false;
            }
    
        }  
    }

    if(verbose){
        cout << " -------------------Nets Info------------------- " << endl;
        printNetList();
    }

    inFile.close();
    return true;
}

