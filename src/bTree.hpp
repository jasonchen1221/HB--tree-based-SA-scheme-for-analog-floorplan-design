#ifndef BTREE_HPP
#define BTREE_HPP

#include "globalParam.hpp"
#include "block.hpp"
#include "contour.hpp"

using std::map; using std::string; using std::pair;
using std::vector;

class BTree{
    friend class Placer;

public:
    BTree() {
        m_dummyNode = new Block;    // set root as dummy node
        m_root = m_dummyNode;
        m_size = 0;
        m_STIW = 0;
    }
    ~BTree() { delete m_dummyNode; }

/* Debugging Functions */
    void printTree() const;             // print the tree Graph
    void printList() const;             // traverse the tree to print list
    void printInvList() const;          // inversely traverse the tree to print inverse list
    void printContourLines() const;     // print the contourlines
    void treePrint(Block* blk, int n) const;
    void play();

/* Binary Tree Operations */
    void insertT(Block* blk);  // Basic Tree Insertion (key comparison have to be decided)
    void insertRnd(Block* blk, bool isSelf);
    void insertRnd_Hierarchy(Block* blk, bool isSelf);
    void insertRnd_HB(Block* blk);
    void deleteT(Block* blk);
    void setRoot(Block* root, Block* left = nullptr, Block* right = nullptr);
    //void treeTransplate(Block* die, Block* cover, bool kill = true);
    void treeNodeDieCover(Block* die, Block* cover, bool kill = true);
    void reset();
    Block* treeMin(Block* root) const;
    Block* treeMax(Block* root) const;
    Block* treeSuccessor(Block* root) const;
    Block* treePredeccessor(Block* root) const;

/* Setters & Getters */
    void    setRoot(Block* b)               { m_root = b; }
    void    setContourMgrWidth(double w)    { m_contourMgr.setWidth(w); }
    void    setWidth(double w)              { m_width = w; }
    void    setHeight(double h)             { m_height = h; }
    double  getWidth() const                { return m_width; }
    double  getHeight() const               { return m_height; }
    double  getArea() const                 { return m_width * m_height; }
    bool    isEmpty() const                 { return (m_size == 0); }
    Block*  getRoot() const                 { return m_root; }

/* B*-tree Perturbation Methods */
    void swapBlk(Block* blkA, Block* blkB);
    void generalSwap(Block* blkA, Block* blkB);
    void connectSwap(Block* parent, Block* child);
    void sameParentSwap(Block* blkA, Block* blkB);

    void swapBlk_Hierarchy(Block* blkA, Block* blkB);
    void generalSwap_Hierarchy(Block* blkA, Block* blkB);
    void rightConnectSwap_Hierarchy(Block* blkA, Block* blkB);
    void leftConnectSwap_Hierarchy(Block* blkA, Block* blkB);
    void sameParentSwap_Hierarchy(Block* blkA, Block* blkB);

    Block* repChange(Block* blkA, Block* blkB);
    void leftRotateB(Block* blk);
    void rightRotateB(Block* blk);
    Block* deleteAndInsert(Block* blk); //ruturen the chosen null block*

/* Packing */
    void packing();
    void packing_Hierarchy();
    void packing_HB();
    
    void treePack(Block* blk);
    void treePack_Hierarchy(Block* blk);
    void treePack_HB(Block* blk);
    void checkSym(Block* blk, bool compre, bool check, bool HI, bool VI, bool pairCheck);
    void checkSym_Hierarchy(Block* blk, bool compre, bool check, bool HI, bool VI, bool pairCheck);
    void compactX(Block* blk, bool isFirst);
    void compactY(Block* blk);

private:
    Block*                          m_dummyNode;
    Block*                          m_root;
    int                             m_size;
    double                          m_width;
    double                          m_height;
    map<string,pair<double,double>> m_REF;
    double                          m_STIW;
    ContourMgr                      m_contourMgr;
    vector<Block*>                  m_nulls;
};



#endif  //BTREE_HPP