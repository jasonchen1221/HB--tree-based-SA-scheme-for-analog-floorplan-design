#include "net.hpp"

Net::Net(){
    m_vBlks.resize(0);
    m_vPins.resize(0);
}
  
/* member functions */ 
void Net::addBlk(Block* b){
    m_vBlks.push_back(b);
}
    
void Net::addPin(Pin* p){
    m_vPins.push_back(p);

    minX = std::min(minX, p->getX());
    maxX = std::max(maxX, p->getX());
    minY = std::min(minY, p->getY());
    maxY = std::max(maxY, p->getY());
}

double Net::getThisNetHPWL() {    
    return ((maxX - minX) + (maxY - minY));
}
/* info display & check */
std::ostream& operator<<(std::ostream& os, const Net& net){
    std::cout << "This net's degree: " << net.m_vBlks.size() << std::endl;
    // display connected blks
    std::cout << "connected blks:" << std::endl;
    for(int i = 0; i < net.m_vBlks.size(); ++i){
        os << net.m_vBlks[i]->getName() << std::endl;
    }
    // display connected pins
    std::cout << "connected pins:" << std::endl;
    for(int i = 0; i < net.m_vPins.size(); ++i){
        os << net.m_vPins[i]->getName() << std::endl;
    }
    return os;
}