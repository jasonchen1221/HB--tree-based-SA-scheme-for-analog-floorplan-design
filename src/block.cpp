#include "block.hpp"

Block::Block(): 
    m_name("undefined_name"), m_x(0), m_y(0),
    m_width(0), m_height(0), m_pLeft(nullptr), 
    m_pRight(nullptr), m_pParent(nullptr) 
    { }

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
    std::swap(m_width, m_height);
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

size_t BlockV::operator () () const{
    size_t k = 5000011;
    int n = (m_name.length() > 4 ? 4 : m_name.length());
    for(int i = 0; i < n; ++i)
        k ^= ((size_t)m_name[i] << (i*8));
    return k;
}

