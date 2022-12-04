#include "placer.hpp"
#include <fstream>

extern time_t startTime;

void Placer::writeResult(std::string outFile){
    cout << "Start writing results..." << endl;

    ofstream os;
    os.open(outFile);

    result(os);

    cout << "Start writing results...[Finished]" << endl;
}

void    Placer::result(ostream& os){
    os << getFinalCost() << endl;
    os << getTTLWireLen() << endl;
    os << getArea() << endl;
    os << getWidth() << " " << getHeight() << endl;
    os << getTime(startTime) << endl;

    for(int i = 0; i < m_vBlockList.size(); ++i){
        os << m_vBlockList[i]->getName() << " "
           << m_vBlockList[i]->getX() << " "
           << m_vBlockList[i]->getY() << " "
           << (m_vBlockList[i]->getX() + m_vBlockList[i]->getW()) << " "
           << (m_vBlockList[i]->getY() + m_vBlockList[i]->getH()) << endl;
    }
}

/*double  Placer::getCost() const{

}*/

double  Placer::getFinalCost() const{
    double a = m_Width * m_Height;
    double l = getTTLWireLen();
    double cost = (m_alpha * a) + ((1 - m_alpha) * l);

    return cost;
}

double  Placer::getTTLWireLen() const{
    int sum = 0;
    for(int i = 0, size = m_vNetList.size(); i < size; ++i){
        sum += m_vNetList[i]->getThisNetHPWL();
    }
    return sum;
}