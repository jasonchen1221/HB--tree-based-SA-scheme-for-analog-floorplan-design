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

void Placer::result(ostream& os){
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

double Placer::getCost() const{
    static double a = 0;
    static double l = 0;

    a = (double)getArea() / m_Anorm;
    l = (m_WireLen / m_Wnorm);

    double R  = m_Height > m_Width ?
                ((double)m_Width  / (double)m_Height):
                ((double)m_Height / (double)m_Width);
    double Rs = m_outlineH > m_outlineW ?
                ((double)m_outlineW / (double)m_outlineH):
                ((double)m_outlineH / (double)m_outlineW);
    double r  = R - Rs;

    assert(a >= 0);

    return (m_w * r * r + (m_x * a) + m_z * l);
    // initial: m_w = 0.5, m_x = 0.5, m_z = 0.0
        // In order to successfully fit in the fixed outline region, 
            // beta is set as small as possible initially
        // In order to successfully fit in the fixed outline region,
            // consider the penalty of out of bound
            // if fit in     -> r is negative, abs|r| very small
            // if not fin in -> r is positive, abs|r| is large
        // By using this way, 
            //if r < 0 -> fit in
            //if r > 0, but very close to 0 -> rotate the all and it will close to fit in
}

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

void Placer::writeRecord(){
    
}