#ifndef PIN_HPP
#define PIN_HPP

#include "globalParam.hpp"
#include <iostream>

class Pin{
public:
    Pin(): m_name("undefined_name"), m_x(0.0), m_y(0.0) { }
    ~Pin() { }

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

class PinV {
public:
   PinV(int n = 0, std::string str = ""): m_no(n), m_name(str) {}
   ~PinV() {}

   std::string getName() const            { return m_name; }
   int getNo () const                     { return m_no; }

   bool operator == (const PinV& n) const { return (m_name == n.getName()); }

   size_t operator () () const
   {
      size_t k = 5000011;
      int n = (m_name.length() > 4 ? 4 : m_name.length());
      for(int i = 0; i < n; ++i)
         k ^= ((size_t)m_name[i] << (i*8));
      return k;
   }
   
private:
   int    m_no;
   std::string m_name;
};



#endif  //PIN_HPP