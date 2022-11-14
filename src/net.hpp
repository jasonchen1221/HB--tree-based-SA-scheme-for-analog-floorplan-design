#ifndef NET_HPP
#define NET_HPP

#include "block.hpp"
#include "pin.hpp"
#include <limits>

class Net{
    friend class Placer;

public:
    Net();
    ~Net() {};
    
    /* member functions */ 
    void addBlk(Block b);
    void addPin(Pin p);
    double getThisNetHPWL();

    /* info display & check */
    friend std::ostream& operator<<(std::ostream& os, const Net& net);

private:
    std::vector<Block> m_vBlks;
    std::vector<Pin>   m_vPins;
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();
};

#endif  //NET_HPP