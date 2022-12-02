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

class ContourLine_v{
    friend class ContourMgr;
public:
    ContourLine_v(
        ContourLine_v* bottom = nullptr, ContourLine_v* up = nullptr, 
        Interval_v intvl_v = Interval_v(0,0), double w = 0):
            m_bottom(bottom), 
            m_up(up),
            m_intvLine_v(intvl_v), 
            m_width(w) { }
    ~ContourLine_v() { }

    double getFirst_v()  const       { return m_intvLine_v.first; }
    double getSecond_v() const       { return m_intvLine_v.second; }
    void reszieFromFront_v(double s) { m_intvLine_v.second = s; }
    void resizeFromBack_v(double s)  { m_intvLine_v.first = s; }
    bool isValid_v() const           { return ((m_intvLine_v.second - m_intvLine_v.first) > 0); }
private:
    ContourLine_v* m_bottom;    
    ContourLine_v* m_up;     
    Interval_v     m_intvLine_v; 
    double         m_width;   
};

class ContourMgr{
    friend class BTree;
    friend class Placer;
public:
    ContourMgr(){
        m_dummy = new ContourLine;
        m_tail = new ContourLine_v;
        m_maxX = 0;
        m_maxY = 0;
        setWidth(INT_MAX);
        setHeight(0, INT_MAX);
        setWidth_HB(INT_MAX);
        setHeight_HB(0, INT_MAX);
    }
    
    ~ContourMgr() {
        clearContour();
        delete m_dummy;
        delete m_tail;
    }

    void setWidth(double);
    void setHeight(double, double);
    void setWidth_HB(double);
    void setHeight_HB(double, double);
    
    double getMaxX() const { return m_maxX; }
    double getMaxY() const { return m_maxY; }

    double insert(const Interval, double, double);
    double insert_HB(const Interval, double, double, bool, bool, bool, bool, bool);

    void deleteL(ContourLine* die);
    void deleteR(ContourLine_v* die);
    void reset();
    void reset_HB();

    void printLines() const;

    void clearContour();
    void clearContour_HB();

private:
    ContourLine*            m_dummy;
    ContourLine_v*          m_tail;
    double                  m_wide;
    double                  m_tall;
    double                  m_origin;
    double                  m_maxX;
    double                  m_maxY;
    std::vector<Contour*>   m_horizontal;
    std::vector<Contour*>   m_vertical;
};


#endif  //#define CONTOUR_HPP