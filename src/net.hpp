#ifndef NET_HPP
#define NET_HPP

#include "block.hpp"
#include "pin.hpp"
#include <limits>

class Net{
    friend class Placer;

public:
    Net();
    ~Net(){ }
    
    /* member functions */ 
    void addBlk(Block* b);
    void addPin(Pin* p);
    double getThisNetHPWL() const;

    //void setNetName(std::string s) { m_name = s; }
    //std::string getNetName() const { return m_name; }

    /* info display & check */
    friend std::ostream& operator<<(std::ostream& os, const Net& net);

private:
    //std::string m_name;
    std::vector<Block*> m_vBlks;
    std::vector<Pin*>   m_vPins;
};

#endif  //NET_HPP