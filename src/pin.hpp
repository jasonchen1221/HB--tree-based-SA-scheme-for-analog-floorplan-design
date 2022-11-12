#ifndef PIN_HPP
#define PIN_HPP

#include "globalParam.hpp"
#include <iostream>

class Pin{
public:
    Pin(): m_name("undefined_name"), m_x(0.0), m_y(0.0) {}
    ~Pin();

    /* setter & getter */
    void setName(std::string s) { m_name = s; }
    void setX(double x)         { m_x = x; }
    void setY(double y)         { m_y = y; } 

    std::string getName() const { return m_name; }
    double getX()         const { return m_x; }
    double getY()         const { return m_y; }

    /* info display & check */
    friend std::ostream& operator<<(std::ostream& os, const Pin& pin){
        os << pin.m_name << ", " << pin.m_x << ", " << pin.m_y;
        return os;
    }

private:
    std::string m_name;
    double      m_x;
    double      m_y;
};

#endif  //PIN_HPP