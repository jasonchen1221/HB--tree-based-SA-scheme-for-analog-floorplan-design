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
    ~Block(){ }

    /* setter & getter */ 
    void setName(std::string s) { m_name = s; }
    void setX(double x)         { m_x = x; }
    void setY(double y)         { m_y = y; }
    void setW(double w)         { m_width = w; }
    void setH(double h)         { m_height = h; }
    
    void setParent(Block* b)    { m_pParent = b; }
    void setLeft(Block* b)      { m_pLeft = b; }
    void setRight(Block* b)     { m_pRight = b; }
    
    std::string getName() const { return m_name; }
    double getX()         const { return m_x; }
    double getY()         const { return m_y; }
    double getW()         const { return m_width; }
    double getH()         const { return m_height; }

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

    Block*               m_pParent;
    Block*               m_pLeft;
    Block*               m_pRight;
};


class BlockV {
public:
    BlockV(int n = 0, std::string str = ""): m_no(n), m_name(str) {}
    ~BlockV() {}

    std::string getName() const              { return m_name; }
    int getNo () const                       { return m_no; }

    bool operator == (const BlockV& n) const { return (m_name == n.getName()); }
    size_t operator () () const;
private:
    int         m_no;
    std::string m_name;   
};


class BlockR {
   friend class Placer;

public:
   BlockR(int x = 0, int y = 0, int w = 0, int h = 0):
            m_x(x), m_y(y), m_width(w), m_height(h) {}
   ~BlockR() {}

private:
   int m_x;
   int m_y;
   int m_width;
   int m_height;
};


#endif  //BLOCK_HPP