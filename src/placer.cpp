#include "placer.hpp"
#include <map>

extern time_t startTime;

/* ---------------------------Pre works of Placer--------------------------- */
void Placer::constructHierarchicalModuleClusteringTree(){
    std::cout << "Construct Hierarchical Module Clustering Tree..." << std::endl;
    
    setSymmetryChild();
    showInfo(false, "After set Symmetry Child");

    mergeSymmetryConstraint();
    showInfo(false, "After merge symmetry constraint");
    m_avgMobWPE = 0;

    setWellIsland();
    //need to add on showing out m_isWellHiearchical, WT, WB, WR, WL
    showInfo(false, "After set Well Island");

    std::cout << "Construct Hierarchical Module Clustering Tree[FINSHED]" << std::endl;
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

void Placer::setWellIsland(){
    // same body potential -> set as well island
    for(auto &p : m_vConstraints[0]){     // with priority == 0's modules
        bool flag = false;
        for(int i = 0; i < p->m_vModuleList.size(); ++i){
            if(p->m_vModuleList[i]->m_bodyPotential == p->m_vModuleList[0]->m_bodyPotential){
                flag = true;
            }
            else{
                flag = false;
                break;
            }
        }
        if(flag == true){
            p->m_isWellHiearchical = true;
            for(int s = 0; s < p->m_vModuleList.size(); s++){
                p->m_vModuleList[s]->m_isWellHiearchical = true;
            }
        }
    }

    for(int p = 0; p < m_level.size(); ++p){
        for(int m = 0; m < m_vConstraints[p].size(); ++m){
            if(m_vConstraints[p][m]->m_isWellHiearchical == false){
                for(int k = 0; k < m_vConstraints[p][m]->m_vModuleList.size()/2; ++k){
                    interWellPlaceModules(m_vConstraints[p][m]->m_vModuleList[k]);
                    int size = m_vConstraints[p][m]->m_vModuleList.size()/2;
                    
                    m_vConstraints[p][m]->m_vModuleList[k+size]
                    ->setW(m_vConstraints[p][m]->m_vModuleList[k]->m_width);
                    m_vConstraints[p][m]->m_vModuleList[k+size]
                    ->setH(m_vConstraints[p][m]->m_vModuleList[k]->m_height);

                    //set well top/bot/right/left spacing
                    m_vConstraints[p][m]->m_vModuleList[k]->m_pChild->m_lde.WT
                    = m_vConstraints[p][m]->m_vModuleList[k]->m_lde.WT;
                    m_vConstraints[p][m]->m_vModuleList[k]->m_pChild->m_lde.WB
                    = m_vConstraints[p][m]->m_vModuleList[k]->m_lde.WB;
                    m_vConstraints[p][m]->m_vModuleList[k]->m_pChild->m_lde.WR
                    = m_vConstraints[p][m]->m_vModuleList[k]->m_lde.WR;
                    m_vConstraints[p][m]->m_vModuleList[k]->m_pChild->m_lde.WL
                    = m_vConstraints[p][m]->m_vModuleList[k]->m_lde.WL;
                }
            }
        }
    }

    double yy = 0.16;

    for(auto &iBlk : m_mainBlockList){
        interWellPlaceModules(iBlk);
        iBlk->setH(iBlk->m_height + 2 * RING_SPACING);
    }

}

double Placer::interWellPlaceModules(Block* blk){
    // SCL, SCT , SCR, and SCB are the "mean distances" from the left, top, right and bottom 
    // well boundary, respectively, as shown in Fig. 3.1. 
    // SC_REF is a reference distance to a single well edge
    double scL, edgeL;
    double scR, edgeR;
    double scT, edgeT;
    double scB, edgeB;

    double WPE_SCA; //  SCA : summation for scattered well dopant
    bool flagWPE = false;

    blk->m_lde.WR = 0.09;
    blk->m_lde.WL = 0.09;
    blk->m_lde.WT = 0.06;
    blk->m_lde.WB = 0.06;

    double u_eff = 0;

    double *blkWL = &blk->m_lde.WL;
    double *blkWR = &blk->m_lde.WR;
    double *blkWT = &blk->m_lde.WT;
    double *blkWB = &blk->m_lde.WB;

    while (flagWPE == false){
        WPE_SCA = scL = edgeL = scR = edgeR = scT = edgeT = scB = edgeB = 0;
        //WPE_SCA = scL = edgeL = scR = edgeR = scT = scB = 0;
        for(int i = 0; i < blk->m_nf; ++i){
            scL = (blk->m_lde.WL + 0.01 + 0.12 + blk->m_lde.LDE_SA + i*(blk->m_lDrawn + blk->m_lde.SD));
            scL = scL * (scL + blk->m_lDrawn);
            scL = 1 / scL;
            edgeL += scL;
        }
        edgeL /= blk->m_nf;

        for(int i = 0; i < blk->m_nf; ++i){
            scR = (blk->m_lde.WR + 0.01 + 0.12 + blk->m_lde.LDE_SB + i*(blk->m_lDrawn + blk->m_lde.SD));
            scR = scR * (scR + blk->m_lDrawn);
            scR = 1 / scR;
            edgeR += scR;
        }
        edgeR /= blk->m_nf;

        scT = blk->m_lde.WT + 0.14 + 0.02;
        scT = scT * (scT + (blk->getWDrawn() / blk->getNFin()));
        edgeT = 1 / scT;

        scB = blk->m_lde.WB + 0.14 + 0.02;
        scB = scB * (scB + (blk->getWDrawn() / blk->getNFin()));
        edgeB = 1 / scB;

        WPE_SCA = edgeL + edgeR + edgeT + edgeB;
        WPE_SCA = SC_REF * SC_REF * WPE_SCA;

        if(blk->m_lde.eDeviceType == PMOS)
            u_eff = P_u0 * WPE_SCA;
        else
            u_eff = N_u0 * WPE_SCA;

        if( (u_eff >= blk->m_lde.MEAN - (3 * blk->m_lde.DEVIATION)) 
            && (u_eff <= blk->m_lde.MEAN + (3 * blk->m_lde.DEVIATION)) ){
            flagWPE = false;
        }
        else{
            flagWPE = true;
        }

        if(flagWPE == false){
            using std::pair;
            using std::make_pair;
            std::multimap<double, pair<double*,std::string>> mm;
            mm.insert(make_pair(edgeL, make_pair(blkWL, "WL")));
            mm.insert(make_pair(edgeR, make_pair(blkWR, "WR")));
            mm.insert(make_pair(edgeB, make_pair(blkWB, "WB")));
            mm.insert(make_pair(edgeT, make_pair(blkWT, "WT")));
            auto iter = mm.rbegin();
            auto range = mm.equal_range(iter->first);
            for(auto iter = range.first; iter != range.second; ++iter){
                *(iter->second.first) += 0.1;
            }
        }
    }
    
    if(blk->m_isSelf){
        blk->m_width = (2.0 * blk->m_width) + blk->m_lde.WL + blk->m_lde.WR + 2 * RING_SPACING;
        blk->m_width = blk->m_width / 2.0;
    }
    else{
        blk->m_width = blk->m_width + blk->m_lde.WL + blk->m_lde.WR + 2 * RING_SPACING;
    }

    if(blk->m_lde.WT > 0.18) blk->m_height = blk->m_height + blk->m_lde.WT;
    else                     blk->m_height = blk->m_height + 0.18;
    if(blk->m_lde.WB > 0.18) blk->m_height = blk->m_height + blk->m_lde.WB;
    else                     blk->m_height = blk->m_height + 0.18;

    if(blk->m_lde.eDeviceType == NMOS) m_avgMobWPE += (N_kuowe * WPE_SCA) * 100;
    else                               m_avgMobWPE += (P_kuowe * WPE_SCA) * 100;

    return WPE_SCA;
}

/* ---------------------------------Placer--------------------------------- */
void Placer::place(){
    std::cout << "Start Place..." << std::endl;

    for(int i = 0; i < m_vConstraints[0].size(); ++i){  // all modules w/ priority == 0
        clearSAList();  // clear m_saBlockList, m_saSymList, m_saBlockHash, m_saNewBlockList

        m_saBlockList.insert(m_saBlockList.begin(), 
                             m_vConstraints[0][i]->m_vModuleList.begin(), 
                             m_vConstraints[0][i]->m_vModuleList.begin() + (m_vConstraints[0][i]->m_vModuleList.size()/2));

        m_saSymList.insert(m_saSymList.begin(),
                           m_vConstraints[0][i]->m_vModuleList.begin() + (m_vConstraints[0][i]->m_vModuleList.size()/2),
                           m_vConstraints[0][i]->m_vModuleList.end());

        m_saBlockHash.reserve(m_saBlockList.size());
        for(auto iter = m_saBlockHash.begin(); iter != m_saBlockHash.end(); ++iter){
            iter->second = new Block;
        }

        int iter = 0;
        int numProcess = 0;
        int numTry = 0;

        while(isTimeout(startTime) == false){   // TIME_LIMIT: 3000 sec
            initialPlace(m_vConstraints[0][i]->getName(), m_vConstraints[0][i]);
 


        }


    }
    

    std::cout << "Start Place[FINISHED]" << std::endl;
}

void Placer::initialPlace(std::string, Block* blk){
    if( (rand() % 6) == 0 ) srand((unsigned)time(NULL));




}