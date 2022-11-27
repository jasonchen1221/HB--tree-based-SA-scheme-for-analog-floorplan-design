#include "placer.hpp"

void Placer::constructTree(){
    setSymmetryChild();
    showInfo(false, "After set Symmetry Child");

    mergeSymmetryConstraint();
    showInfo(false, "After merge symmetry constraint");

    

}

void Placer::setSymmetryChild(){
    for(int p = 0; p < m_level.size() - 1; ++p){
        for(int m = 0; m < m_vConstraints[p].size(); ++m){
            if(m_vConstraints[p][m]->m_vModuleList.size() > 0){
                switch ( m_vConstraints[p][m]->m_constraint.eConstraintType ){
                    case SYMMETRY:{
                        m_vConstraints[p][m]->m_vModuleList[0]->m_pChild = m_vConstraints[p][m]->m_vModuleList[1];
                        m_vConstraints[p][m]->m_vModuleList[1]->m_pChild = m_vConstraints[p][m]->m_vModuleList[0];
                    }
                    break;
                    
                    case SELF: {
                        m_vConstraints[p][m]->m_vModuleList[0]->m_isSelf = true;
                        m_vConstraints[p][m]->m_vModuleList[0]->setW( m_vConstraints[p][m]->m_vModuleList[0]->m_width / 2.0 );
                        
                        Block* newBlk = new Block;
                        m_vConstraints[p][m]->m_vModuleList[0]->m_pChild = newBlk;

                        newBlk->setName( m_vConstraints[p][m]->m_vModuleList[0]->getName() );
                        newBlk->setW( m_vConstraints[p][m]->m_vModuleList[0]->getW() );
                        newBlk->setH( m_vConstraints[p][m]->m_vModuleList[0]->getH() );
                        newBlk->setNFin( m_vConstraints[p][m]->m_vModuleList[0]->getNFin() );
                        newBlk->m_bodyPotential = m_vConstraints[p][m]->m_vModuleList[0]->m_bodyPotential;
                        newBlk->m_lde.SD = m_vConstraints[p][m]->m_vModuleList[0]->m_lde.SD;
                        newBlk->m_isSelf = m_vConstraints[p][m]->m_vModuleList[0]->m_isSelf;
                        newBlk->m_constraint.symAxisName = m_vConstraints[p][m]->m_vModuleList[0]->m_constraint.symAxisName;
                        newBlk->m_pChild= m_vConstraints[p][m]->m_vModuleList[0];
                        //m_vConstraints[p][m]->m_vModuleList[1]->m_pChild = m_vConstraints[p][m]->m_vModuleList[0];    
                        
                        m_vConstraints[p][m]->m_vModuleList.push_back(newBlk);
                    }
                    break;

                    default:
                        break;
                }
            }
        }
    }
}


void Placer::mergeSymmetryConstraint(){
    for(int p = 0; p < m_level.size(); ++p){
        for(int iterI = 0; iterI < m_vConstraints[p].size(); ++iterI){

            if(m_vConstraints[p][iterI]->m_constraint.eConstraintType != SYMMETRY &&
               m_vConstraints[p][iterI]->m_constraint.eConstraintType != SELF){
                continue;    
            }

            std::string currAxisName = m_vConstraints[p][iterI]->m_name;
            for(int iterJ = iterI + 1; iterJ < m_vConstraints[p].size(); ++iterJ){
                if((currAxisName == m_vConstraints[p][iterJ]->getName()) && 
                  ((m_vConstraints[p][iterJ]->m_constraint.eConstraintType == SYMMETRY) || 
                   (m_vConstraints[p][iterJ]->m_constraint.eConstraintType == SELF) ) ){
                    // Combine 2 symmetry constraint    
                    int nMiddleI = m_vConstraints[p][iterI]->m_vModuleList.size() / 2;
                    int nMiddleJ = m_vConstraints[p][iterJ]->m_vModuleList.size() / 2;
                    
                    // Insert to middle
                        // vector_name.insert(position, size, val)
                    m_vConstraints[p][iterI]->m_vModuleList.insert(
                        m_vConstraints[p][iterI]->m_vModuleList.begin() + nMiddleI, 
                        m_vConstraints[p][iterJ]->m_vModuleList.begin(),
                        m_vConstraints[p][iterJ]->m_vModuleList.begin() + nMiddleJ);
                    // Insert to end
                    m_vConstraints[p][iterI]->m_vModuleList.insert(
                        m_vConstraints[p][iterI]->m_vModuleList.end(),
                        m_vConstraints[p][iterJ]->m_vModuleList.begin() + nMiddleJ,
                        m_vConstraints[p][iterJ]->m_vModuleList.end());
                    
                    // remove
                    delete m_vConstraints[p][iterJ];
                    m_vConstraints[p].erase(m_vConstraints[p].begin() + iterJ);
                    --iterJ;
                }
            }
        }
    }
}




void Placer::clearLists(){
    for(int  i = 0; i < m_mainBlockList.size(); ++i){
        delete m_mainBlockList[i];
    }      

    m_vBlockList.clear();    
    m_vPinList.clear();
    m_vNetList.clear();
       
       /*
       for(map<string, Btree*>::const_iterator it = _bstar.begin();
       it != _bstar.end(); ++it)
          delete it->second;
       _bstar.clear();
        */
    m_mainBlockList.clear();
    

    /*
     if(_hbtree)delete _hbtree;
     if(_recoverMsg) delete _recoverMsg;
    */
}