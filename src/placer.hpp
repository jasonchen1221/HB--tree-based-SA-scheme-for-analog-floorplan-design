#ifndef PLACER_HPP
#define PLACER_HPP

#include "globalParam.hpp"
#include "block.hpp"
#include "net.hpp"
#include "pin.hpp"
#include "bTree.hpp"
#include "recoverMsg.hpp"
#include "contourline.hpp"
#include "hashSet.hpp"

#include <cstdlib>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <float.h>

class Placer{
public:
    Placer(){
        m_T             = T_INIT;
        m_P             = P_INIT;
        m_Cost          = 0.0;
        m_Anorm         = 1.0;
        m_Wnorm         = 1.0;
        m_delta         = 0;
        m_Width         = INT_MAX;
        m_Height        = INT_MAX;
        m_WireLen       = 2;
        m_BTree         = nullptr;
        m_recoverMsg    = new RecoverMsg;
        srand((unsigned)time(NULL));
        m_minCost       = INT_MAX;
    }
    ~Placer(){
        clear();
    } 

/* Parsers */
    void    parseAll(std::string alpha, std::string blkFile, std::string netFile);
    bool    parseAlpha(std::string alpha, bool verbose);
    bool    parseBlkFile(std::string blkFile, bool verbose);
    bool    parseNetFile(std::string netFile, bool verbose);

/* Placer */
    void    place();
    void    initPerturb();
    bool    isTimeout(const time_t&) const;
    void    initSol();
    void    recover();
    void    genNeighbor();
    void    updateCost(); // update _Anorm, _Wnorm, and _Cost
    bool    shinbwei() const; 
    void    rotateAll();
    void    record();
    void    writeRecord();
    PerturbType rndSelectType() const;
    double  T(int iter);

/* Perturbation Modes: Generate Neighbor */ 
    void    genLeftRotate();
    void    genRightRotate();
    void    genDeleteInsert();
    void    genSwapTwoBlocks();
    void    genRotateBlock();

/* Result(Dumper) */ 
    void    writeResult(std::string);
    void    result(ostream&);
    //void    resultGUI(ostream&);
    //void    resultGnuplot();
    double  getCost() const;
    double  getFinalCost() const;
    double  getArea() const          { return (m_Width * m_Height); }
    double  getOutlineArea() const   { return (m_outlineW * m_outlineH); }
    double  getTTLWireLen() const;
    double  getWidth() const         { return m_Width; }
    double  getHeight() const        { return m_Height; }

/* Debugging msg */
    void    printBlockList() const;
    void    printPinList() const;
    void    printNetList() const;
    void    printRnd() const;

    void    clear();

    BTree*  getBTree() { return m_BTree; }

private:
/* alpha */
    double m_alpha;
/* outline */
    double m_outlineW;
    double m_outlineH;
/* Block */
    std::vector<Block*> m_vBlockList;
    HashSet<BlockV> m_blockHashSet;
    
/* Net */
    std::vector<Net*> m_vNetList;
/* Pin */
    std::vector<Pin*> m_vPinList;
    HashSet<PinV> m_PinHashSet;

/* B-tree */
    BTree*                        m_BTree;
    RecoverMsg*                   m_recoverMsg;

/* */
    double             m_Cost;
    double             m_Anorm;
    double             m_Wnorm;
    double             m_delta;
    double             m_P;
    double             m_C;
    double             m_T;
    double             m_T1;

    double             m_k;
    double             m_L; 
    double             m_N;
    double             m_I; // #iter upper bound
    double             m_Width;
    double             m_Height;
    double             m_WireLen;

    double             m_w;
    double             m_x;
    double             m_y;
    double             m_z;

    double             m_uphill_avg_cnt;
    double             m_uphill_sum;
    double             m_delta_cost_cnt;
    double             m_delta_sum;

    double             m_minCost;
    double             m_totalArea;
    vector<BlockR>     m_optSol;


/* Help Functions */
    void    splitString(std::string str, std::vector<std::string>& vec);
    bool    isFloat(string s);
    double  getTime(const time_t& start) const;
    double  getRnd() const;
    size_t  getHashSize(size_t s) const;
    bool    isFitin() const     { return (m_Width <= m_outlineW && m_Height <= m_outlineH); }
    bool    isInvFit() const;
    void    calTTLArea();
};



#endif //PLACER_HPP