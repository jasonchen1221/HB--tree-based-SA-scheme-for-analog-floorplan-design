#ifndef PLACER_HPP
#define PLACER_HPP

#include "globalParam.hpp"
#include "block.hpp"
#include "net.hpp"
#include "pin.hpp"
#include <cstdlib>

class Placer{
public:
    Placer()  {}
    ~Placer() {}

    /* Parsers */
    void parseAll(std::string alpha, std::string blkFile, std::string netFile, std::string grpFile);
    

private:
    /* alpha */
    double m_alpha;
    /* outline */
    double m_outlineW;
    double m_outlineH;

    /* Parsers */
    bool parseAlpha(std::string alpha);
    bool parseBlkFile(std::string blkFile);
    bool parseNetFile(std::string netFile);
    bool parseGrpFile(std::string grpFile);
    void splitString(std::string str, std::vector<std::string>& vec);

};



#endif //PLACER_HPP