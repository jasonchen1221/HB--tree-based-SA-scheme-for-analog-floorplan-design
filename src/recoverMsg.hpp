#ifndef RECOVER_MSG_H
#define RECOVER_MSG_H

#include "globalParam.hpp"
#include "block.hpp"
#include "bTree.hpp"

/*  << RecoverMsg Protocol >> */
/*
[0] LEFT_ROTATE
      m_blocks[0] = rootblock(after rotation) (i.e. the rotated node's parent after rotation)

[1] RIGHT_ROTATE 
      m_blocks[0] = rootblock(after rotation) (i.e. the rotated node's parent after rotation)
      
[2] DELETE_INSERT 
      m_blocks[0] = the chosenNullnode 
      m_blocks[1] = deleteAndInsert Block
      
[3] SWAP_TWO_BLOCK
      m_blocks[0] = block1
      m_blocks[1] = block2

[4] ROTATE_BLOCK
      m_blocks[0] = rotated block
*/

class RecoverMsg{
public:
    RecoverMsg() { reset(); }
    ~RecoverMsg() { }
    void reset() {
        m_vBlocks.resize(0);
        m_parentBlock = nullptr;
        m_type = OPT_TOT;
        m_parentType = OPT_TOT;
    }

/* getter & setter */
    PertubType getType() const              { return m_type; }
    PertubType getParentType() const        { return m_parentType; }
    Block* getithBlk(unsigned int i) const {
        if(i < m_vBlocks.size()){
            return m_vBlocks[i];
        }
        else{
            return nullptr;
        }
    }
    Block* getParentBlk()                   { return m_parentBlock; }
    int getSize()                           { return m_vBlocks.size(); }

    void setType(PertubType t)              { m_type = t; }
    void setParentType(PertubType t)        { m_parentType = t; }
    void setParentBlk(Block* b)             { m_parentBlock = b;}
    void addBlk(Block* b)                   { m_vBlocks.push_back(b); }

private:
    PertubType          m_type;
    PertubType          m_parentType;
    std::vector<Block*> m_vBlocks;
    Block*              m_parentBlock;
};

#endif  //RECOVER_MSG_H