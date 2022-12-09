#ifndef BTREE_HPP
#define BTREE_HPP

#include "globalParam.hpp"
#include "block.hpp"
#include "contourline.hpp"

using std::map; using std::string; using std::pair;
using std::vector;

class BTree{
    friend class Placer;

public:
    BTree(size_t ns = NULL_SIZE) {
        m_dummyNode = new Block;    // set root as dummy node
        m_root = m_dummyNode;
        m_size = 0;
        m_nulls.resize(ns);
        for(int i = 0; i < m_nulls.size(); ++i){
            m_nulls[i] = new Block;
            insertRnd(m_nulls[i]);
        }
    }
    ~BTree() { 
        delete m_dummyNode;
        for(int i = 0; i < m_nulls.size(); ++i){
            delete m_nulls[i];
        }
    }

/* Debugging Functions */
    void printTree() const;             // print the tree Graph
    void printList() const;             // traverse the tree to print list
    void printInvList() const;          // inversely traverse the tree to print inverse list
    void printContourLines() const;     // print the contourlines
    void treePrint(Block* blk, int n) const;
    void play();

/* Binary Tree Operations */
    void    insertT(Block* blk);  // Basic Tree Insertion (key comparison have to be decided)
    void    insertRnd(Block* blk);
    void    deleteT(Block* ptr);
    void    setRoot(Block* root, Block* left, Block* right);
    void    treeNodeDieCover(Block* die, Block* cover, bool kill);
    void    reset();
    Block*  treeMin(Block* ptr) const;
    Block*  treeMax(Block* ptr) const;
    Block*  treeSuccessor(Block* ptr) const;
    Block*  treePredeccessor(Block* ptr) const;

/* Setters & Getters */
    void    setRoot(Block* b)               { m_root = b; }
    void    setContourMgrWidth(double w)    { m_contourMgr.setWidth(w); }
    double  getWidth() const                { return m_width; }
    double  getHeight() const               { return m_height; }
    double  getArea() const                 { return m_width * m_height; }
    bool    isEmpty() const                 { return (m_size == 0); }
    Block*  getRoot() const                 { return m_root; }

/* Packing */
    void compactX(Block* blk, bool isFirst);
    void compactY(Block* blk);
    void treePack(Block* blk);
    void packing();

/* B*-tree Perturbation Methods */
    void    swapBlk(Block* blkA, Block* blkB);
    void    generalSwap(Block* blkA, Block* blkB);
    void    connectSwap(Block* parent, Block* child);
    void    sameParentSwap(Block* blkA, Block* blkB);
    void    leftRotateB(Block* blk);
    void    rightRotateB(Block* blk);    
    Block*  deleteAndInsert(Block* blk); //ruturen the chosen null block*

/* B*-Tree write results */
    void    dumpCaseResult(vector<Block*> v_blks, std::string flag);

private:
    Block*                          m_dummyNode;
    Block*                          m_root;
    int                             m_size;
    double                          m_width;
    double                          m_height;
    ContourMgr                      m_contourMgr;
    vector<Block*>                  m_nulls;
};



#endif  //BTREE_HPP