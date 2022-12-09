#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include "globalParam.hpp"
#include <vector>
#include <climits>

class ContourLine{
    friend class ContourMgr;

public:
    ContourLine(
        ContourLine* front = nullptr, ContourLine* back = nullptr, 
        Interval intvl = Interval(0,0), double h = 0):
            m_front(front), 
            m_back(back),
            m_intvLine(intvl), 
            m_height(h) { }
    ~ContourLine() { }

    double getFirst()  const       { return m_intvLine.first; }
    double getSecond() const       { return m_intvLine.second; }
    void reszieFromFront(double s) { m_intvLine.second = s; }
    void resizeFromBack(double s)  { m_intvLine.first = s; }
    bool isValid() const           { return ((m_intvLine.second - m_intvLine.first) > 0); }

private:
    ContourLine* m_front;    // closer to m_dummy
    ContourLine* m_back;     // far from m_dummy
    //typedef pair<double, double> Interval;
    //typedef pair<double, double> Interval_v;
    Interval     m_intvLine; // interval of this line
    double       m_height;   // the height of this interval    
};

class ContourMgr{
    friend class BTree;
    friend class Placer;

public:
    ContourMgr(){
        m_dummy = new ContourLine;
        m_maxY = 0;
        setWidth(INT_MAX);
    }
    
    ~ContourMgr() {
        clearContour();
        delete m_dummy;
    }

    void setWidth(double width);
    double getMaxY() const { return m_maxY; }

    double insert(const Interval inter1, double h);

    void deleteL(ContourLine* die);     /* delete horizontal contourLine */
    void reset();

    void printLines() const;

    void clearContour();

private:
    ContourLine*            m_dummy;
    double                  m_width;
    double                  m_maxY;
};


#endif  //#define CONTOUR_HPP