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
#include <float.h>

class Placer{
public:
    Placer(){
        m_Temp = T_INIT;
        m_P = P_INIT;
        //m_Cost = 0.0;
        m_LODCost = DBL_MAX_EXP;
        //m_Anorm = m_Wnorm = 1.0;
        m_deltaCost = 0;
        //
        //
        //
        //
        //
        //
    }
    ~Placer(){

    } 

/* Parsers */
    void parseAll(std::string alpha, std::string blkFile, std::string netFile, std::string grpFile);
    bool parseAlpha(std::string alpha, bool verbose);
    bool parseBlkFile(std::string blkFile, bool verbose);
    bool parseNetFile(std::string netFile, bool verbose);
    bool parseGrpFile(std::string grpFile, bool verbose);
/* Device Generation, LOD Minimization */
    void genDevice();
    void minimizeLOD(bool verbose);
    void insertDummy();
    void determineSize();
    void showGenDeviceInfo(bool verbose, std::string flag);

    double m_LAnorm;
    double mobRelChangeFunc_PMOS(double SA, double NF, double FW, double SD, double LDrawn);
    double mobRelChangeFunc_NMOS(double SA, double NF, double FW, double SD, double LDrawn);
    double getLODCost(double FW, double NFIN, double mobRelChan, double mobRelChanRef, double SD, double LDrawn);
    double m_LODCost;

    void mergeSymConstraint();
    void interWellPlaceModules();

/* SA */
    double updateT(int iter);
    double m_deltaCost;
    double m_P;
    double m_C;
    double m_Temp;
    double m_Temp1;
    double m_deltaCostCnt;
    double m_deltaSum;
    double m_K;
    double m_L;
    double m_N;
    double m_I; // # of iter upper bound

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
    std::map<std::string, std::vector<std::string>> m_levelMap; //SELF 
    std::map<std::string, std::vector<std::pair<std::string,std::string>>> m_levelMapPair;  //SYMMETRY

/* Help Functions */
    void splitString(std::string str, std::vector<std::string>& vec);

};



#endif //PLACER_HPP