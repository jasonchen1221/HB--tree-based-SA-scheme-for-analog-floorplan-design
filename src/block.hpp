#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "globalParam.hpp"
#include <vector>
#include <iostream>

class Block{
    friend class BTree;
    friend class Placer;

public:
    Block();
    ~Block(){}

    /* setter & getter */ 
    void setName(std::string s) { m_name = s; }
    void setX(double x)         { m_x = x; }
    void setY(double y)         { m_y = y; }
    void setW(double w)         { m_width = w; }
    void setH(double h)         { m_height = h; }
    void setWDrawn(double w)    { m_wDrawn = w; }
    void setLDrawn(double l)    { m_lDrawn = l; }
    void setNFin(int n)         { m_nf = n; }
    void setSD(double sd)       { m_lde.SD = sd; }
    void setParent(Block* b)    { m_pParent = b; }
    void setLeft(Block* b)      { m_pLeft = b; }
    void setRight(Block* b)     { m_pRight = b; }
    
    std::string getName() const { return m_name; }
    double getX()         const { return m_x; }
    double getY()         const { return m_y; }
    double getW()         const { return m_width; }
    double getH()         const { return m_height; }
    double getWDrawn()    const { return m_wDrawn; }
    double getLDrawn()    const { return m_lDrawn; }
    int    getNFin()      const { return m_nf; }
    double getSD()        const { return m_lde.SD; }
    double getArea()      const { return m_width * m_height; }
    Block* getParent()    const { return m_pParent; }
    Block* getLeft()      const { return m_pLeft; }
    Block* getRight()     const { return m_pRight; }

    /* member functions */ 
    void laydown();
    void standup();
    void rotate();
    void rotateXY();

    /* info display & check */ 
    friend std::ostream& operator<<(std::ostream& os, const Block& blk);
    void displayPLR() const;    // Parent, Left, Right

private:
    std::string          m_name;
    double               m_x;
    double               m_y;
    double               m_width;
    double               m_height;
    double               m_wDrawn;
    double               m_lDrawn;
    int                  m_nf;

    LDEParam             m_lde;
    unsigned int         m_nPriority;

    Block*               m_pLeft;
    Block*               m_pRight;
    Block*               m_pParent;
    Block*               m_pChild;

    std::string          m_bodyPotential;

    Constraint           m_constraint;

    bool                 m_isHierarchical;
    bool                 m_isWellHiearchical;
    bool                 m_isSelf;
    bool                 m_isContour;
    
    std::vector<Block>   m_vModuleList;
    std::vector<Block>   m_vHLContour;
    std::vector<Contour> m_vVLContour;
    std::vector<Contour> m_vBotContour;
};

#endif  //BLOCK_HPP