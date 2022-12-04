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
}

double Net::getThisNetHPWL() const{    
    static double minX, maxX, minY, maxY;
    static double tmp_x, tmp_y, tmp_w, tmp_h, mid_x, mid_y;   // temp

    minX = std::numeric_limits<double>::max();
    maxX = std::numeric_limits<double>::lowest();
    minY = std::numeric_limits<double>::max();
    maxY = std::numeric_limits<double>::lowest();
    
    for(int i = 0; i < m_vBlks.size(); ++i){
        tmp_x = m_vBlks[i]->getX(); 
        tmp_y = m_vBlks[i]->getY();
        tmp_w = m_vBlks[i]->getW();
        tmp_h = m_vBlks[i]->getH();
        
        mid_x = tmp_x + (tmp_w / 2);
        mid_y = tmp_y + (tmp_h / 2);

        if(mid_x > maxX) maxX = mid_x;
        if(mid_x < minX) minX = mid_x;
        if(mid_y > maxY) maxY = mid_y;
        if(mid_y < minY) minY = mid_y;
    }

    for(int i = 0; i < m_vPins.size(); ++i){
        tmp_x = m_vPins[i]->getX();
        tmp_y = m_vPins[i]->getY();

        if(tmp_x > maxX) maxX = tmp_x;
        if(tmp_x < minX) minX = tmp_x;
        if(tmp_y > maxY) maxY = tmp_y;
        if(tmp_y < minY) minY = tmp_y;
    }

    return ((maxX - minX) + (maxY - minY));
}
/* info display & check */
std::ostream& operator<<(std::ostream& os, const Net& net){
    std::cout << "This net's degree: " << net.m_vBlks.size() + net.m_vPins.size() << std::endl;
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