#include "block.hpp"

Block::Block(): 
    m_name("undefined_name"), m_x(0), m_y(0),
    m_width(0), m_height(0), m_wDrawn(0), m_lDrawn(0),
    m_nf(0), m_pLeft(nullptr), m_pRight(nullptr), 
    m_pParent(nullptr), m_pChild(nullptr),
    m_bodyPotential(""), m_isHierarchical(false),
    m_isWellHiearchical(false), m_isSelf(false),
    m_isContour(false) {
    m_vModuleList.resize(0);
    m_vHLContour.resize(0);
    m_vVLContour.resize(0);
    m_vBotContour.resize(0);
}

/* member functions */ 
void Block::laydown(){
    if(m_width >= m_height) return;
    std::swap(m_width, m_height);
}

void Block::standup(){
    if(m_height >= m_width) return;
    std::swap(m_width, m_height);
}
        
void Block::rotate(){
    if(m_isSelf){
        double tmp_w = m_width;
        m_width = m_height / 2;
        m_height = tmp_w * 2; 
    }
    else{
        std::swap(m_width, m_height);
    }
}

void Block::rotateXY(){
    rotate();
    std::swap(m_x, m_y);
}

/* info display & check */ 
std::ostream& operator<<(std::ostream& os, const Block& blk){
    os << blk.m_name << ", " << blk.m_width << ", " << blk.m_height;
    return os;
}

void Block::displayPLR() const{ // Parent, Left, Right
    std::cout << m_name << ", "
              << "P: " 
              << (m_pParent == nullptr ? "nullptr" : m_pParent->getName()) << ", "
              << "L: "
              << (m_pLeft == nullptr ? "nullptr" : m_pLeft->getName()) << ", "
              << "R: "
              << (m_pRight == nullptr ? "nullptr" : m_pRight->getName()) << std::endl;
}             