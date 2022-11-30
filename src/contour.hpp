#ifndef CONTOUR_HPP
#define CONTOUR_HPP


class ContourLine{

};

class ContourLine_v{

};

class ContourMgr{
public:
    
    void setWidth(double w) { m_width = w; }

    void printLines() const;

private:
    double m_width;
};


#endif  //#define CONTOUR_HPP