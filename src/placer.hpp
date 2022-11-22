#ifndef PLACER_HPP
#define PLACER_HPP

#include "globalParam.hpp"
#include "block.hpp"
#include "net.hpp"
#include "pin.hpp"
#include <cstdlib>
#include <set>
#include <map>
#include <unordered_map>

class Placer{
public:
    Placer()  {}
    ~Placer() {} 

    /* Parsers */
    void parseAll(std::string alpha, std::string blkFile, std::string netFile, std::string grpFile);
    bool parseAlpha(std::string alpha, bool verbose);
    bool parseBlkFile(std::string blkFile, bool verbose);
    bool parseNetFile(std::string netFile, bool verbose);
    bool parseGrpFile(std::string grpFile, bool verbose);

private:
/* alpha */
    double m_alpha;
/* outline */
    double m_outlineW;
    double m_outlineH;
/* Block */
    std::vector<Block> m_vBlockList;
    //HashSet<BlockV> m_BlockHash;
    std::unordered_map<std::string, Block> m_BlockHash;
    std::vector<Block> m_mainBlockList;
/* Net */
    std::vector<Net> m_vNetList;
/* Pin */
    std::vector<Pin> m_vPinList;
    //HashSet<PinV> m_PinHash;
    std::unordered_map<std::string, Pin> m_PinHash;
/* Constraints */
    std::vector<std::vector<Block>> m_vConstraints;

/* Level */
    std::set<int> m_level;
    std::map<std::string, std::vector<std::string>> m_levelMap;
    std::map<std::string, std::vector<std::pair<std::string,std::string>>> m_levelMapPair;

/* Help Functions */
    void splitString(std::string str, std::vector<std::string>& vec);

};



#endif //PLACER_HPP