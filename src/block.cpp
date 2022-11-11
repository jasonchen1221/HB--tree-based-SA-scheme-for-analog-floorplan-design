#include "block.hpp"

Block::Block(): 
    m_name("undefined_name"), m_x(0), m_y(0),
    m_width(0), m_height(0), m_wDrawn(0), m_lDrawn(0),
    m_nf(0), m_pLeft(nullptr), m_pRight(nullptr), 
    m_pParent(nullptr), m_pChild(nullptr),
    m_bodyPotential(""), m_isHierarchical(false),
    m_isWellHiearchical(false), m_isSelf(false),
    m_isContour(false) {
    m_vModuleList.resize(0);
    m_vHLContour.resize(0);
    m_vVLContour.resize(0);
    m_vBotContour.resize(0);
}

