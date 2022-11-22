#include "placer.hpp"

#include <fstream>
//#include <assert.h>

void Placer::parseAll(std::string alpha, std::string blkFile, std::string netFile, std::string grpFile){
    
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

    if(parseGrpFile(grpFile, false) == false){
        std::cerr << "Error: Fail to parse: " << grpFile << std::endl;
        exit(EXIT_FAILURE);
    }

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
    //m_BlockHash.init(getHashSize(numBlks));
    
    /* parse # of pins */
    vec_str.clear();
    getline(inFile, str);
    splitString(str, vec_str);
    int numPins = std::stoi(vec_str[1]);
    m_vPinList.resize(numPins);
    //m_PinHash.init(getHashSize(numPins));
    
    /* parse blks */
    getline(inFile, str);
    for(int i = 0; i < m_vBlockList.size(); ++i){
        vec_str.clear();
        getline(inFile, str);
        splitString(str, vec_str);
        /* Name */
        m_vBlockList[i].setName(vec_str[0]);
        if(vec_str[0] == "NM8"  || vec_str[0] == "M4."  || vec_str[0] == "M6" ||
           vec_str[0] == "NM57" || vec_str[0] == "NM61" || vec_str[0] == "NM4."){
            m_vBlockList[i].m_lde.CHECK = "check1";
        }
        if(vec_str[0] == "NM51" || vec_str[0] == "NM1."){
            m_vBlockList[i].m_lde.CHECK = "check2";
        }
        /* BlkW */
        m_vBlockList[i].setWDrawn(std::stod(vec_str[1]));
        /* BlkL */
        m_vBlockList[i].setLDrawn(std::stod(vec_str[2]));
        /* NumFin */
        m_vBlockList[i].setNFin(std::stoi(vec_str[3]));
        /* Device Type */
        if     (vec_str[4] == "P") m_vBlockList[i].m_lde.eDeviceType = PMOS;
        else if(vec_str[4] == "N") m_vBlockList[i].m_lde.eDeviceType = NMOS;
        else if(vec_str[4] == "R") m_vBlockList[i].m_lde.eDeviceType = RESISTOR;
        else if(vec_str[4] == "C") m_vBlockList[i].m_lde.eDeviceType = CAPACITOR;
        else                       m_vBlockList[i].m_lde.eDeviceType = UNKNOWN_TYPE;
        /* Body Detached */
        if     (vec_str[5] == "n") m_vBlockList[i].m_lde.isBodyDetached = false;
        else if(vec_str[5] == "y") m_vBlockList[i].m_lde.isBodyDetached = true;
        else                       m_vBlockList[i].m_lde.isBodyDetached = false;
        /* BL */
        if     (vec_str[6] == "0") m_vBlockList[i].m_lde.BL = false;
        else if(vec_str[6] == "1") m_vBlockList[i].m_lde.BL = true;
        else                       m_vBlockList[i].m_lde.BL = false;
        /* BR */
        if     (vec_str[7] == "0") m_vBlockList[i].m_lde.BR = false;
        else if(vec_str[7] == "1") m_vBlockList[i].m_lde.BR = true;
        else                       m_vBlockList[i].m_lde.BR = false;
        /* BT */
        if     (vec_str[8] == "0") m_vBlockList[i].m_lde.BT = false;
        else if(vec_str[8] == "1") m_vBlockList[i].m_lde.BT = true;
        else                       m_vBlockList[i].m_lde.BT = false;
        /* BB */
        if     (vec_str[9] == "0") m_vBlockList[i].m_lde.BB = false;
        else if(vec_str[9] == "1") m_vBlockList[i].m_lde.BB = true;
        else                       m_vBlockList[i].m_lde.BB = false;
        /* Body Potential */
        m_vBlockList[i].m_bodyPotential = vec_str[10];
        /* LDE Mead */
        m_vBlockList[i].m_lde.MEAN = std::stod(vec_str[11]);
        /* LDE Deviation */
        m_vBlockList[i].m_lde.DEVIATION = std::stod(vec_str[12]);
        /* Insert to HashSet */
        /*if(m_BlockHash.insert(BlockV(i, m_vBlockList[i].getName())) == false){
            std::cerr << "Error: Two Blocks have same name: " << m_vBlockList[i].getName() << std::endl;
            return false;
        }*/
        m_BlockHash.insert({m_vBlockList[i].getName(), m_vBlockList[i]});
    }
    
    /* parse pins */
    getline(inFile, str);
    for(int i = 0; i < m_vPinList.size(); ++i){
        vec_str.clear();
        getline(inFile, str);
        splitString(str, vec_str);
        /* Name */
        m_vPinList[i].setName(vec_str[0]);
        /* X */
        m_vPinList[i].setX(std::stod(vec_str[2]));
        /* Y */
        m_vPinList[i].setY(std::stod(vec_str[3]));
        /* Insert to HashSet */
        /*if(m_PinHash.insert(PinV(i, m_vPinList[i].getName())) == false){
            std::cerr << "Error: Two Pins have same name: " << m_vPinList[i].getName() << std::endl;
            return false;
        }*/
        m_PinHash.insert({m_vPinList[i].getName(), m_vPinList[i]});
    }

    if(verbose){
        std::cout << "Outline: " << m_outlineW << " " << m_outlineH << std::endl;
        std::cout << "Num of Blks: " << m_vBlockList.size() << std::endl;
        std::cout << "=====================VECTOR========================" << std::endl;
        for(int i = 0; i < m_vBlockList.size(); ++i){
            std::cout << "----- Block [" << i << "] -----" << std::endl;
            std::cout << "Name: " << m_vBlockList[i].getName() << std::endl;
            std::cout << "W: " << m_vBlockList[i].getWDrawn() << std::endl;
            std::cout << "L: " << m_vBlockList[i].getLDrawn() << std::endl;
            std::cout << "NumFin: " << m_vBlockList[i].getNFin() << std::endl;
            std::cout << "Type: " << m_vBlockList[i].m_lde.eDeviceType << std::endl;
            std::cout << "BD-Detached: " << m_vBlockList[i].m_lde.isBodyDetached << std::endl;
            std::cout << "BL: " << m_vBlockList[i].m_lde.BL << std::endl;
            std::cout << "BR: " << m_vBlockList[i].m_lde.BR << std::endl;
            std::cout << "BT: " << m_vBlockList[i].m_lde.BT << std::endl;
            std::cout << "BB: " << m_vBlockList[i].m_lde.BB << std::endl;
            std::cout << "BD-Potential: " << m_vBlockList[i].m_bodyPotential << std::endl;
            std::cout << "LDE-Mean: " << m_vBlockList[i].m_lde.MEAN << std::endl;
            std::cout << "LDE-Deviation: " << m_vBlockList[i].m_lde.DEVIATION << std::endl;
        }
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "Num of Terminals: " << m_vPinList.size() << std::endl;
        for(int i = 0; i < m_vPinList.size(); ++i){
            std::cout << "----- Pin [" << i << "] -----" << std::endl;
            std::cout << "Name: " << m_vPinList[i].getName() << std::endl;
            std::cout << "X: " << m_vPinList[i].getX() << std::endl;
            std::cout << "Y: " << m_vPinList[i].getY() << std::endl;
        }
        std::cout << "=====================HASH_MAP========================" << std::endl;
        std::cout << "Num of Blks: " << m_BlockHash.size() << std::endl;
        for(auto iter = m_BlockHash.begin(); iter != m_BlockHash.end(); iter++){
            std::cout << "Name: " << iter->first << std::endl;
            std::cout << "W: " << iter->second.getWDrawn() << std::endl;
            std::cout << "L: " << iter->second.getLDrawn() << std::endl;
            std::cout << "NumFin: " << iter->second.getNFin() << std::endl;
            std::cout << "Type: " << iter->second.m_lde.eDeviceType << std::endl;
        }
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "Num of Terminals: " << m_PinHash.size() << std::endl;
        for(auto iter = m_PinHash.begin(); iter != m_PinHash.end(); ++iter){
            std::cout << "Name: " << iter->first << std::endl;
            std::cout << "X: " << iter->second.getX() << std::endl;
            std::cout << "Y: " << iter->second.getY() << std::endl;
        }
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

    /* parse # of blks */
    for(int i = 0; i < numNets; ++i){
        vec_str.clear();
        getline(inFile, str);
        splitString(str, vec_str);
        m_vNetList[i].setNetName(vec_str[1]);

        vec_str.clear();
        getline(inFile, str);
        splitString(str, vec_str);
        int ithNetDegree = std::stoi(vec_str[1]);
        for(int j = 0; j < ithNetDegree; ++j){
            vec_str.clear();
            getline(inFile, str);
            splitString(str, vec_str);
            
            if(m_BlockHash.count(vec_str[0])){  // is block
                auto iter = m_BlockHash.find(vec_str[0]);
                m_vNetList[i].addBlk(iter->second);
            }
            else if(m_PinHash.count(vec_str[0])){  // is pin
                auto iter = m_PinHash.find(vec_str[0]);
                m_vNetList[i].addPin(iter->second);
            }
            else{
                std::cerr << "Error: Neither a block nor a pin is named: " 
                          << vec_str[0] << std::endl;
                return false; 
            }
            
            //BlockV tmpBlkV(-1, vec_str[0]);
            //PinV tmpPinV(-1, vec_str[0]);

            /*check if in m_vBlockList*/            
            /*if(m_BlockHash.query(tmpBlkV)){
                //std::cout << "test" << std::endl;
                assert(tmpBlkV.getNo() >= 0);
                m_vNetList[i].addBlk(m_vBlockList[tmpBlkV.getNo()]);
            }
            // check if in m_vPinList 
            else if(m_PinHash.query(tmpPinV)){
                assert(tmpPinV.getNo() >= 0);
                m_vNetList[i].addPin(m_vPinList[tmpPinV.getNo()]);
            }
            else{
                std::cerr << "Error: Neither a block nor a pin is named: " 
                          << vec_str[0] << std::endl;
                return false; 
            }*/
        }
    }

    if(verbose){
        std::cout << "Num of nets: " << m_vNetList.size() << std::endl;
        for(int i = 0; i < m_vNetList.size(); ++i){
            std::cout << "Net name: " << m_vNetList[i].getNetName() << std::endl;
            std::cout << "# of blks of this net: " << m_vNetList[i].m_vBlks.size() << std::endl;
            for(int j = 0; j < m_vNetList[i].m_vBlks.size(); ++j){
                std::cout << "------------------------------- " << std::endl;
                std::cout << m_vNetList[i].m_vBlks[j].getName() << std::endl;
                std::cout << m_vNetList[i].m_vBlks[j].getWDrawn() << std::endl;
                std::cout << m_vNetList[i].m_vBlks[j].getLDrawn() << std::endl;
                std::cout << m_vNetList[i].m_vBlks[j].m_lde.eDeviceType << std::endl;
            }
            std::cout << "# of pins of this net: " << m_vNetList[i].m_vPins.size() << std::endl;
            if(m_vNetList[i].m_vPins.size() > 0){
                for(int j = 0; j < m_vNetList[i].m_vPins.size(); ++j){
                std::cout << "------------------------------- " << std::endl;
                std::cout << m_vNetList[i].m_vPins[j].getName() << std::endl;
                std::cout << m_vNetList[i].m_vPins[j].getX() << std::endl;
                std::cout << m_vNetList[i].m_vPins[j].getY() << std::endl;
                }
            }
        }
    }

    inFile.close();
    return true;
}

bool Placer::parseGrpFile(std::string grpFile, bool verbose){
    
    std::ifstream inFile(grpFile, std::ios::in);
    if(!inFile){
        std::cerr << "Error: Fail to open: " << grpFile << std::endl;
        return false;
    }

    std::string str;
    std::vector<std::string> vec_str;

    std::set<std::string> nIndexes;

    while(inFile.peek() != EOF){
        /* constraint_start */
        vec_str.clear();
        getline(inFile, str);   
        splitString(str, vec_str);

        Block tmpBlk;
        /* Priority */
        vec_str.clear();
        getline(inFile, str);   
        splitString(str, vec_str);
        tmpBlk.m_nPriority = std::stoi(vec_str[1]);
        m_level.insert(tmpBlk.m_nPriority);

        /* Type */
        vec_str.clear();
        getline(inFile, str);   
        splitString(str, vec_str);
        if(vec_str[1] == "symmetry"){
            tmpBlk.m_constraint.eConstraintType = SYMMETRY;
        }
        else if(vec_str[1] == "self"){
            tmpBlk.m_constraint.eConstraintType = SELF;
        }
        else{
            tmpBlk.m_constraint.eConstraintType = UNDEFINED_TYPE;
        }

        /* Axis */
        vec_str.clear();
        getline(inFile, str);   
        splitString(str, vec_str);
        tmpBlk.m_name = vec_str[1]; //use Axis name as block(group) name

        /* Members */
        vec_str.clear();
        getline(inFile, str);   
        splitString(str, vec_str);
        std::pair<std::string, std::string> pair;
        pair.first.clear(); pair.second.clear();
        for(int i = 1; i < vec_str.size(); ++i){
            //for(int j = 0; j < m_vBlockList.size(); ++j){
                if(m_BlockHash.count(vec_str[i])){    // is in the m_vBlockList(is MOS Blk)
                    auto iter = m_BlockHash.find(vec_str[i]);
                    //m_vBlockList[j].m_constraint.symAxisName = tmpBlk.m_name;
                    iter->second.m_constraint.symAxisName = tmpBlk.m_name;
                    nIndexes.insert(iter->first);
                    tmpBlk.m_vModuleList.push_back(iter->second);
                }
                //else{   
                // is not in the m_vBlockList(is MODULE(group) Blk)
                if(tmpBlk.m_constraint.eConstraintType == SELF){
                    m_levelMap[tmpBlk.m_name].push_back(vec_str[i]);
                }
                else if(tmpBlk.m_constraint.eConstraintType == SYMMETRY){
                    if(pair.first == ""){
                        pair.first = vec_str[i];
                    }
                    else if(pair.second == ""){
                        pair.second = vec_str[i];
                    }
                    else{ }
                }
                else{ }
                //}
            //}
        }
        if((pair.first != "") && (pair.second != "")){
            m_levelMapPair[tmpBlk.m_name].push_back(pair);
        }

        /* constraint end */
        vec_str.clear();
        getline(inFile, str);   
        splitString(str, vec_str);
        // SET PRIORITY
        if(tmpBlk.m_nPriority >= m_vConstraints.size()){
            m_vConstraints.push_back(std::vector<Block>());
        }
        m_vConstraints[tmpBlk.m_nPriority].push_back(tmpBlk);

        /* Blank Line */
        getline(inFile, str); 
    }

    for(int i = 0; i < m_vBlockList.size(); ++i){
        // the MOS block which is not in symmetry group or self group
        if(nIndexes.find(m_vBlockList[i].getName()) == nIndexes.end()){     
            m_mainBlockList.push_back(m_vBlockList[i]);
        }
    }
    nIndexes.clear();


    if(verbose){
        std::cout << "==================m_vBlockList==================" << std::endl;
        std::cout << "Size: " << m_vBlockList.size() << std::endl;
        for(int i = 0; i < m_vBlockList.size(); ++i){
            std::cout << "-----------m_vBlockList[" << i << "]----------" << std::endl;
            std::cout << "Name: " << m_vBlockList[i].getName() << std::endl;
            std::cout << "W: " << m_vBlockList[i].getWDrawn()
                      << "\t" << "L: " << m_vBlockList[i].getLDrawn() << std::endl;
            std::cout << "Axis: " << m_vBlockList[i].m_constraint.symAxisName << std::endl;
            std::cout << "This block ModuleList size: " << m_vBlockList[i].m_vModuleList.size() << std::endl;
            if(m_vBlockList[i].m_vModuleList.size() != 0){
                for(int j = 0; j < m_vBlockList[i].m_vModuleList.size(); ++j){
                    std::cout << "---Module List---" << std::endl;
                    std::cout << m_vBlockList[i].m_vModuleList[j].getName() << std::endl;
                }
            }
        }

        std::cout << "==================m_levelMap(SELF)==================" << std::endl;
        std::cout << "Size: " << m_levelMap.size() << std::endl;
        for(auto iter = m_levelMap.begin(); iter != m_levelMap.end(); ++iter){
            std::cout << "-----------Module: " << iter->first << "-----------" << std::endl;
            std::cout << "This module size: " << iter->second.size() << std::endl;
            for(int i = 0; i < iter->second.size(); ++i){
                std::cout << iter->second[i] << std::endl;
            }
        }

        std::cout << "==================m_levelMapPair(SYMMETRY)==================" << std::endl;
        std::cout << "Size: " << m_levelMapPair.size() << std::endl;
        for(auto iter = m_levelMapPair.begin(); iter != m_levelMapPair.end(); ++iter){
            std::cout << "-----------Module: " << iter->first << "-----------" << std::endl;
            std::cout << "# of Paires: " << iter->second.size() << std::endl;
            for(int i = 0; i < iter->second.size(); ++i){
                std::cout << iter->second[i].first << ", " << iter->second[i].second << std::endl;
            }
        }

        std::cout << "==================m_vConstraints==================" << std::endl;
        std::cout << "Size(# of priority): " << m_vConstraints.size() << std::endl;
        for(int i = 0; i < m_vConstraints.size(); ++i){
            std::cout << "----------------Priority i = " << i << "----------------" << std::endl;
            std::cout << "Priority:"<< i <<", # of moudle have this priority: " << m_vConstraints[i].size() << std::endl;
            for(int j = 0; j < m_vConstraints[i].size(); ++j){
                std::cout << "---module name: " << m_vConstraints[i][j].getName() << "---" << std::endl;
                std::cout << "This module's m_vModuleList size: ";
                std::cout << m_vConstraints[i][j].m_vModuleList.size() << std::endl;
                std::cout << "m_vModuleList: " << std::endl;
                for(int k = 0; k < m_vConstraints[i][j].m_vModuleList.size(); ++k){
                    std::cout << m_vConstraints[i][j].m_vModuleList[k].getName() << std::endl;
                }
            }
        }
        
        std::cout << "==================m_mainBlockList==================" << std::endl;
        std::cout << "Size: " << m_mainBlockList.size() << std::endl;
        for(int i = 0; i < m_mainBlockList.size(); ++i){
            std::cout << m_mainBlockList[i].getName() << std::endl;
        }
    }

    inFile.close();
    return true;
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