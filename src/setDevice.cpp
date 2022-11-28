#include "placer.hpp"
#include <cmath>

void Placer::setDevice(){
    // mobility miltiplier of LOD -> 1
    // eq(7), eq(8), eq(9)
    minimizeLOD(false);  //set SD(poly-to-poly spaceing) & NF(#of fingers)
    
    // to ensure better mathcing and further LOD minumization
    insertDummy();

    //@@..
    double a = 1 + ( P_lkvtho / (pow( (0.36e-06)+P_xl, P_llodkvtho )));
    double b = ( P_wlodkvtho / (pow(1.1725e-06 + P_xw + P_wlod, P_wlodkvtho)) );
    double c = pow( (0.36e-06) + P_xl, P_llodkvtho) * pow(1.1725e-06 + P_xw + P_wlod, P_wlodkvtho );
    c = P_pkvtho / c;
    double Kstress_u0 = a + b + c;

    determineSize();
    showInfo(false, "After Determine Size");
    
}

void Placer::minimizeLOD(bool verbose){
    
    if(verbose){
        std::cout << "******************Before Device Setting******************" << std::endl;
        std::cout << "==================m_vConstraints==================" << std::endl;
        std::cout << "Size(# of priority): " << m_vConstraints.size() << std::endl;
        for(int i = 0; i < m_vConstraints.size(); ++i){
            std::cout << "----------------Priority i = " << i << "----------------" << std::endl;
            std::cout << "Priority:"<< i <<", # of moudle have this priority: " << m_vConstraints[i].size() << std::endl;
            for(int j = 0; j < m_vConstraints[i].size(); ++j){
                std::cout << "---module name: " << m_vConstraints[i][j]->getName() << "---" << std::endl;
                std::cout << "This module's m_vModuleList size: ";
                std::cout << m_vConstraints[i][j]->m_vModuleList.size() << std::endl;
                std::cout << "m_vModuleList: " << std::endl;
                for(int k = 0; k < m_vConstraints[i][j]->m_vModuleList.size(); ++k){
                    std::cout << "Block Name: " << m_vConstraints[i][j]->m_vModuleList[k]->getName() << std::endl;
                    std::cout << "W_Drawn: " << m_vConstraints[i][j]->m_vModuleList[k]->getWDrawn() << std::endl;
                    std::cout << "L_Drawn: " << m_vConstraints[i][j]->m_vModuleList[k]->getLDrawn() << std::endl;
                    std::cout << "N Fin: " << m_vConstraints[i][j]->m_vModuleList[k]->getNFin() << std::endl;
                    std::cout << "SD: " << m_vConstraints[i][j]->m_vModuleList[k]->getSD() << std::endl;
                }
            }
        }
        
        std::cout << "==================m_mainBlockList==================" << std::endl;
        std::cout << "Size: " << m_mainBlockList.size() << std::endl;
        for(int i = 0; i < m_mainBlockList.size(); ++i){
            std::cout << m_mainBlockList[i]->getName() << std::endl;
            std::cout << "W_Drawn: " << m_mainBlockList[i]->getWDrawn() << std::endl;
            std::cout << "L_Drawn: " << m_mainBlockList[i]->getLDrawn() << std::endl;
            std::cout << "N Fin: " << m_mainBlockList[i]->getNFin() << std::endl;
            std::cout << "SD: " << m_mainBlockList[i]->getSD() << std::endl;
        }
    }
    
    // priority: 0, 1, 2 -> do 0 & 1
    for(int p = 0; p < m_level.size() - 1; ++p){
        // w/ priority p, -> module list of m
        for(int m = 0; m < m_vConstraints[p].size(); ++m){
            if(m_vConstraints[p][m]->m_vModuleList.size() > 0){
                // initial finger number
                int NF = m_vConstraints[p][m]->m_vModuleList[0]->getNFin();
                // transistor_width/NF
                double FW = m_vConstraints[p][m]->m_vModuleList[0]->getWDrawn() / NF;

                // Simulated Annealing
                double sum = 0;
                double n = 0;

                double currCost = 0;
                double lastCost = 0;

                double uphillSum = 0;
                int uphillCnt = 0;
                
                double totalNFin; 
                double mobRelChan, mobRelChanRef;

                double thisLdrawn = m_vConstraints[p][m]->m_vModuleList[0]->getLDrawn();
                // m_LAnorm is different by process
                // @_@ estimate the width of device @_@
                m_LAnorm = (2 * SA + NF * thisLdrawn + (NF-1) * 0.2 ) * FW;


                while(FW > MIN_FW){
                    double SD = 0.2;    // initial poly-to-poly spacing
                    totalNFin = NF + m_vConstraints[p][m]->m_vModuleList[0]->m_lde.DUM;
                    double LDrawn = m_vConstraints[p][m]->m_vModuleList[0]->getLDrawn();

                    while(SD < MAX_SD){
                        if(m_vConstraints[p][m]->m_vModuleList[0]->m_lde.eDeviceType == PMOS){
                            mobRelChan    = mobRelChangeFunc_PMOS(SA * 1e-6, totalNFin, FW * 1e-6, SD * 1e-6, LDrawn * 1e-6);
                            mobRelChanRef = mobRelChangeFunc_PMOS(SA_REF * 1e-6, totalNFin, FW * 1e-6, SD * 1e-6, LDrawn * 1e-6);
                        }
                        else{   // NMOS
                            mobRelChan    = mobRelChangeFunc_NMOS(SA * 1e-6, totalNFin, FW * 1e-6, SD * 1e-6, LDrawn * 1e-6);
                            mobRelChanRef = mobRelChangeFunc_NMOS(SA_REF * 1e-6, totalNFin, FW * 1e-6, SD * 1e-6, LDrawn * 1e-6);
                        }

                        currCost = getLODCost(FW, totalNFin, mobRelChan, mobRelChanRef, SD, LDrawn);
                        m_deltaCost = currCost - lastCost;
                        if(m_deltaCost > 0){
                            uphillSum += m_deltaCost;
                            uphillCnt++;
                        }
                        lastCost = currCost;
                        sum += FW * (2 * SA + NF * LDrawn + (NF-1) * SD);
                        n++;
                        SD = SD + 0.01;
                    }
                    NF++;
                    FW = m_vConstraints[p][m]->m_vModuleList[0]->getWDrawn() / NF;
                }

                m_LAnorm = sum / n;
                m_Temp = ( (double)uphillSum / (double)uphillCnt ) / ( (-1)*log(P_INIT) );
                m_LODCost = currCost;

                int iter = 0;
                int _N = 0;
                int _C = C_INIT;
                int _K = K_INIT;

                while(m_Temp > T_FROZEN){
                    ++_N;
                    ++iter;
                    m_deltaCostCnt = 0;
                    m_deltaSum = 0;

                    NF = m_vConstraints[p][m]->m_vModuleList[0]->getNFin();
                    FW = m_vConstraints[p][m]->m_vModuleList[0]->getWDrawn() / NF;
                    while(FW > MIN_FW){
                        double _SD = 0.2;
                        totalNFin = NF + m_vConstraints[p][m]->m_vModuleList[0]->m_lde.DUM;
                        double _LDrawn = m_vConstraints[p][m]->m_vModuleList[0]->getLDrawn();

                        double _SA = SA;
                        while( _SD <= MAX_SD ){
                            if(m_vConstraints[p][m]->m_vModuleList[0]->m_lde.eDeviceType == PMOS){
                                mobRelChan    = mobRelChangeFunc_PMOS(_SA * 1e-6, totalNFin, FW * 1e-6, _SD * 1e-6, _LDrawn * 1e-6);
                                mobRelChanRef = mobRelChangeFunc_PMOS(SA_REF * 1e-6, totalNFin, FW * 1e-6, _SD * 1e-6, _LDrawn * 1e-6);
                            }
                            else{   //NMOS
                                mobRelChan    = mobRelChangeFunc_NMOS(_SA * 1e-6, totalNFin, FW * 1e-6, _SD * 1e-6, _LDrawn * 1e-6);
                                mobRelChanRef = mobRelChangeFunc_NMOS(SA_REF * 1e-6, totalNFin, FW * 1e-6, _SD * 1e-6, _LDrawn * 1e-6);
                            }

                            m_deltaCost = getLODCost(FW, totalNFin, mobRelChan, mobRelChanRef, _SD, _LDrawn) - m_LODCost;
                            if(m_deltaCost <= 0){   //move to better 
                                m_LODCost = getLODCost(FW, totalNFin, mobRelChan, mobRelChanRef, _SD, _LDrawn);
                                m_vConstraints[p][m]->m_vModuleList[0]->setNFin(NF);
                                if(NF > 1){
                                    m_vConstraints[p][m]->m_vModuleList[0]->setSD(_SD);
                                }
                            }
                            _SD += 0.01;
                        }
                        NF++;
                        if(NF == 2) NF++;
                        if(NF % 2 != 0) NF++;
                        FW = m_vConstraints[p][m]->m_vModuleList[0]->getWDrawn() / NF;
                    }
                    m_Temp = updateT(iter);
                    if(iter > 25) { break; }
                }

                if(m_vConstraints[p][m]->m_constraint.eConstraintType == SYMMETRY){
                    m_vConstraints[p][m]->m_vModuleList[1]->setNFin(m_vConstraints[p][m]->m_vModuleList[0]->getNFin());
                    m_vConstraints[p][m]->m_vModuleList[1]->setSD(m_vConstraints[p][m]->m_vModuleList[0]->getSD());
                }
            }
        }
    }

    for(int iter = 0; iter < m_mainBlockList.size(); ++iter){
        m_LODCost = DBL_MAX_EXP;
        double NF = 1;
        double FW = m_mainBlockList[iter]->getWDrawn() / NF;
        double sum = 0;
        int n = 0;

        double L_Drawn = m_mainBlockList[iter]->getLDrawn();
        double W_Drawn = m_mainBlockList[iter]->getWDrawn();

        while(FW > MIN_FW){
            double _SD = 0.2;
            while(_SD <= MAX_SD){
                sum += ( 2 * SA + (NF+2) * L_Drawn + (NF+1) * _SD ) * FW;
                n++;
                _SD += 0.01;
            }
            NF++;
            FW = W_Drawn / NF;
        }
        m_LAnorm = sum / n;
        NF = m_mainBlockList[iter]->getNFin();
        FW = W_Drawn / NF;
        while(FW > MIN_FW){
            double _SD = 0.2;
            double totalNFIN = 2 + NF;
            double mobRelChan;
            double mobRelChanRef;
            while(_SD < MAX_SD){
                if(m_mainBlockList[iter]->m_lde.eDeviceType == PMOS){
                    mobRelChan = mobRelChangeFunc_PMOS(SA, totalNFIN, FW, _SD, L_Drawn);
                    mobRelChanRef = mobRelChangeFunc_PMOS(SA_REF, totalNFIN, FW, _SD, L_Drawn);
                }
                else{   // NMOS
                    mobRelChan = mobRelChangeFunc_NMOS(SA, totalNFIN, FW, _SD, L_Drawn);
                    mobRelChanRef = mobRelChangeFunc_NMOS(SA_REF, totalNFIN, FW, _SD, L_Drawn);
                }
                
                //move to better
                if(m_LODCost - getLODCost(FW, totalNFIN, mobRelChan, mobRelChanRef, _SD, L_Drawn) > 0){
                    m_LODCost = getLODCost(FW, totalNFIN, mobRelChan, mobRelChanRef, _SD, L_Drawn);
                    m_mainBlockList[iter]->setNFin(NF);
                    m_mainBlockList[iter]->setSD(_SD);
                }
                _SD += 0.01;
            }
            NF++;
            FW = W_Drawn / NF;
        }
    }

    
    if(verbose){
        std::cout << "******************After Device Gen******************" << std::endl;
        std::cout << "==================m_vConstraints==================" << std::endl;
        std::cout << "Size(# of priority): " << m_vConstraints.size() << std::endl;
        for(int i = 0; i < m_vConstraints.size(); ++i){
            std::cout << "----------------Priority i = " << i << "----------------" << std::endl;
            std::cout << "Priority:"<< i <<", # of moudle have this priority: " << m_vConstraints[i].size() << std::endl;
            for(int j = 0; j < m_vConstraints[i].size(); ++j){
                std::cout << "---module name: " << m_vConstraints[i][j]->getName() << "---" << std::endl;
                std::cout << "This module's m_vModuleList size: ";
                std::cout << m_vConstraints[i][j]->m_vModuleList.size() << std::endl;
                std::cout << "m_vModuleList: " << std::endl;
                for(int k = 0; k < m_vConstraints[i][j]->m_vModuleList.size(); ++k){
                    std::cout << "Block Name: " << m_vConstraints[i][j]->m_vModuleList[k]->getName() << std::endl;
                    std::cout << "W_Drawn: " << m_vConstraints[i][j]->m_vModuleList[k]->getWDrawn() << std::endl;
                    std::cout << "L_Drawn: " << m_vConstraints[i][j]->m_vModuleList[k]->getLDrawn() << std::endl;
                    std::cout << "N Fin: " << m_vConstraints[i][j]->m_vModuleList[k]->getNFin() << std::endl;
                    std::cout << "SD: " << m_vConstraints[i][j]->m_vModuleList[k]->getSD() << std::endl;
                }
            }
        }
        
        std::cout << "==================m_mainBlockList==================" << std::endl;
        std::cout << "Size: " << m_mainBlockList.size() << std::endl;
        for(int i = 0; i < m_mainBlockList.size(); ++i){
            std::cout << m_mainBlockList[i]->getName() << std::endl;
            std::cout << "W_Drawn: " << m_mainBlockList[i]->getWDrawn() << std::endl;
            std::cout << "L_Drawn: " << m_mainBlockList[i]->getLDrawn() << std::endl;
            std::cout << "N Fin: " << m_mainBlockList[i]->getNFin() << std::endl;
            std::cout << "SD: " << m_mainBlockList[i]->getSD() << std::endl;
        }
    }

}

void Placer::insertDummy(){
    using std::pair;
    using std::make_pair;
    using std::map;
    using std::string;

    map< pair<string, double>, pair<string, pair<int,int>> > P_Stress;
    map< pair<string, double>, pair<string, pair<int,int>> > N_Stress;

    for(int p = 0; p < m_level.size()-1; ++p){  // priority
        for(int iter = 0; iter < m_vConstraints[p].size(); ++iter){
            if(m_vConstraints[p][iter]->m_vModuleList.size() > 0){
                double a = 0.0, b = 0.0, c = 0.0;
                double L_Drawn = m_vConstraints[p][iter]->m_vModuleList[0]->getLDrawn();
                double SD = m_vConstraints[p][iter]->m_vModuleList[0]->m_lde.SD;
                double NF = m_vConstraints[p][iter]->m_vModuleList[0]->getNFin();
                
                if(m_vConstraints[p][iter]->m_vModuleList[0]->getNFin() % 2 != 0){    // with odd # of fins
                    double k = floor(m_vConstraints[p][iter]->m_vModuleList[0]->getNFin() / 2);
                    k = k - 1;
                    a = SA + k * (L_Drawn + SD) + L_Drawn / 2.0;
                    a = 1 / a;
                    b = SA + (NF - (k+1)) * (L_Drawn + SD) + (L_Drawn / 2.0);
                    b = 1 / b;
                    a = a + b;
                    if(m_vConstraints[p][iter]->m_vModuleList[0]->m_lde.eDeviceType == PMOS){
                        P_Stress[make_pair(m_vConstraints[p][iter]->m_vModuleList[0]->getName(),a)] = 
                        make_pair(m_vConstraints[p][iter]->m_lde.CHECK, make_pair(p, iter));   
                    }
                    else{   //NMOS
                        N_Stress[make_pair(m_vConstraints[p][iter]->m_vModuleList[0]->getName(),a)] = 
                        make_pair(m_vConstraints[p][iter]->m_lde.CHECK, make_pair(p, iter));   
                    }
                }
                else{   // with even # of fins
                    double k = m_vConstraints[p][iter]->m_vModuleList[0]->getNFin() / 2;
                    k = k - 1;
                    a = SA + k * (L_Drawn + SD) + L_Drawn / 2.0;
                    a = 1 / a;
                    b = SA + (NF - (k+1)) * (L_Drawn + SD) + (L_Drawn / 2.0);
                    b = 1 / b;
                    c += a + b;
                    k ++;
                    
                    a = SA + k * (L_Drawn + SD) + L_Drawn / 2.0;
                    a = 1 / a;
                    b = SA + (NF - (k+1)) * (L_Drawn + SD) + (L_Drawn / 2.0);
                    b = 1 / b;
                    c += a + b;

                    c = c / 2.0;
                    if(m_vConstraints[p][iter]->m_vModuleList[0]->m_lde.eDeviceType == PMOS){
                        P_Stress[make_pair(m_vConstraints[p][iter]->m_vModuleList[0]->getName(),a)] = 
                        make_pair(m_vConstraints[p][iter]->m_lde.CHECK, make_pair(p, iter));   
                    }
                    else{   //NMOS
                        N_Stress[make_pair(m_vConstraints[p][iter]->m_vModuleList[0]->getName(),a)] = 
                        make_pair(m_vConstraints[p][iter]->m_lde.CHECK, make_pair(p, iter));   
                    }
                }
            }
        }
    }
    
    //@@
    for(int i = 0; i < m_vConstraints[0].size(); ++i){
        if(m_vConstraints[0][i]->m_vModuleList[0]->getName() == "M6"){
            m_vConstraints[0][i]->m_vModuleList[0]->m_nf = 2;
            m_vConstraints[0][i]->m_vModuleList[1]->m_nf = 2;
        }
    }
    
    //map< pair<string, double>, pair<string, pair<int,int>> > P_Stress;
    //map< pair<string, double>, pair<string, pair<int,int>> > N_Stress;
    //@@
    for(auto iter = P_Stress.begin(); iter != P_Stress.end(); ++iter){
        if(iter->second.first == "check1"){
            m_vConstraints[iter->second.second.first][iter->second.second.second]->m_vModuleList[0]->m_lde.DUM = 2;
        }
        if(iter->second.first == "check2"){
            m_vConstraints[iter->second.second.first][iter->second.second.second]->m_vModuleList[0]->m_lde.DUM = 1;
            m_vConstraints[iter->second.second.first][iter->second.second.second]->m_vModuleList[0]->m_lde.SD = 0.2;
        }
    }

    for(auto iter = N_Stress.begin(); iter != N_Stress.end(); ++iter){
        if(iter->second.first == "check1"){
            m_vConstraints[iter->second.second.first][iter->second.second.second]->m_vModuleList[0]->m_lde.DUM = 2;
        }
        if(iter->second.first == "check2"){
            m_vConstraints[iter->second.second.first][iter->second.second.second]->m_vModuleList[0]->m_lde.DUM = 1;
            m_vConstraints[iter->second.second.first][iter->second.second.second]->m_vModuleList[0]->m_lde.SD = 0.2;
        }
    }

}

void Placer::determineSize(){
    double yy = 0.16;
    for(int p = 0; p < m_level.size(); ++p){    // priority
        for(int m = 0; m < m_vConstraints[p].size(); ++m){  //module
            if(m_vConstraints[p][m]->m_vModuleList.size() > 0){  
                double NF = m_vConstraints[p][m]->m_vModuleList[0]->getNFin();
                double TotalNF = NF + m_vConstraints[p][m]->m_vModuleList[0]->m_lde.DUM;
                double LDrawn = m_vConstraints[p][m]->m_vModuleList[0]->getLDrawn();
                double WDrawn = m_vConstraints[p][m]->m_vModuleList[0]->getWDrawn();
                double SD = m_vConstraints[p][m]->m_vModuleList[0]->getSD();

                double LOD = (2 * SA) + (TotalNF * LDrawn) + ((TotalNF-1) * SD);
                double FW = WDrawn / NF;

                switch (m_vConstraints[p][m]->m_vModuleList[0]->m_lde.eDeviceType){
                    case NMOS:{
                        if(m_vConstraints[p][m]->m_vModuleList[0]->m_lde.isBodyDetached){ // is BodyDetached
                            double BL = m_vConstraints[p][m]->m_vModuleList[0]->m_lde.BL;
                            double BR = m_vConstraints[p][m]->m_vModuleList[0]->m_lde.BR;
                            if((BL && !BR) || (!BL && BR)){
                                //if(WDrawn == yy) 
                                    //m_vConstraints[p][m]->m_vModuleList[0]->m_width = LOD + 2*(0.01+0.12) + 0.60;
                                //else 
                                    //m_vConstraints[p][m]->m_vModuleList[0]->m_width = LOD + 2*(0.01+0.12) + 0.47;
                            }
                            else if(!BL && !BR){
                                //m_vConstraints[p][m].m_vModuleList[0].m_width = LOD + 2 * (0.01+0.12);
                            }
                            else if(BL && BR){
                                //if(WDrawn == yy) 
                                    //m_vConstraints[p][m].m_vModuleList[0].m_width = LOD + 2 * (0.01+0.12+0.60);
                                //else 
                                    //m_vConstraints[p][m].m_vModuleList[0].m_width = 2 * (0.01+0.12+0.47) + LOD;
                            }
                            else{ }
                            
                            if(BL){
                                if(WDrawn == yy) 
                                    m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SA = SA + 0.60;
                                else 
                                    m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SA = SA + 0.47;
                            }
                            
                            if(BR){
                                if(WDrawn == yy) 
                                    m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SB = SA + 0.60;
                                else 
                                    m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SB = SA + 0.47;
                            }
                            //m_vConstraints[p][m].m_vModuleList[0].m_height = FW + 2 * (0.14+0.02);
                            m_vConstraints[p][m]->m_vModuleList[0]->m_lde.Y2 = 0.2;
                        }
                        else{   //not BodyDetached
                            //m_vConstraints[p][m].m_vModuleList[0].m_width = LOD + 2*(0.01+0.12);
                            //m_vConstraints[p][m].m_vModuleList[0].m_height = FW + 2*(0.14+0.02);
                            m_vConstraints[p][m]->m_vModuleList[0]->m_lde.Y2 = 0.2;
                        }
                    }
                    break;

                    case PMOS:{
                        if(m_vConstraints[p][m]->m_vModuleList[0]->m_lde.isBodyDetached){ // is BodyDetached
                            double BL = m_vConstraints[p][m]->m_vModuleList[0]->m_lde.BL;
                            double BR = m_vConstraints[p][m]->m_vModuleList[0]->m_lde.BR;
                            if((BL && !BR) || (!BL && BR)){
                                //if(WDrawn == yy) 
                                //    m_vConstraints[p][m].m_vModuleList[0].m_width = LOD + 2*(0.01+0.12) + 0.60;
                                //else 
                                //    m_vConstraints[p][m].m_vModuleList[0].m_width = LOD + 2*(0.01+0.12) + 0.47;
                            }
                            else if(!BL && !BR){
                                //m_vConstraints[p][m].m_vModuleList[0].m_width = LOD + 2 * (0.01+0.12);
                            }
                            else if(BL && BR){
                                //if(WDrawn == yy) 
                                 //   m_vConstraints[p][m].m_vModuleList[0].m_width = LOD + 2 * (0.01+0.12+0.60);
                                //else 
                                 //   m_vConstraints[p][m].m_vModuleList[0].m_width = 2 * (0.01+0.12+0.47) + LOD;
                            }
                            else{ }
                            
                            if(BL){
                                if(WDrawn == yy) 
                                    m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SA = SA + 0.60;
                                else 
                                    m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SA = SA + 0.47;
                            }
                            
                            if(BR){
                                if(WDrawn == yy) 
                                    m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SB = SA + 0.60;
                                else 
                                    m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SB = SA + 0.47;
                            }
                            //m_vConstraints[p][m].m_vModuleList[0].m_height = FW + 2 * (0.14+0.02);
                            m_vConstraints[p][m]->m_vModuleList[0]->m_lde.Y2 = 0.2;
                        }
                        else{   //not BodyDetached
                            //m_vConstraints[p][m].m_vModuleList[0].m_width = LOD + 2*(0.01+0.12);
                            //m_vConstraints[p][m].m_vModuleList[0].m_height = FW + 2*(0.14+0.02);
                            m_vConstraints[p][m]->m_vModuleList[0]->m_lde.Y2 = 0.2;
                        }
                    }
                    break;
                    
                    default:
                        break;
                }

                if( m_vConstraints[p][m]->m_constraint.eConstraintType == SYMMETRY ){
                    //m_vConstraints[p][m].m_vModuleList[1].setW(m_vConstraints[p][m].m_vModuleList[0].getW());
                    //m_vConstraints[p][m].m_vModuleList[1].setH(m_vConstraints[p][m].m_vModuleList[0].getH());
                    m_vConstraints[p][m]->m_vModuleList[1]->m_lde.LDE_SA = m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SA;
                    m_vConstraints[p][m]->m_vModuleList[1]->m_lde.LDE_SB = m_vConstraints[p][m]->m_vModuleList[0]->m_lde.LDE_SB;
                    m_vConstraints[p][m]->m_vModuleList[1]->m_lde.Y2 = m_vConstraints[p][m]->m_vModuleList[0]->m_lde.Y2;
                }
            }
        }
    }



    for(int iter = 0; iter < m_mainBlockList.size(); ++iter){
        double NF = m_mainBlockList[iter]->getNFin();
        double LDrawn = m_mainBlockList[iter]->getLDrawn();
        double WDrawn = m_mainBlockList[iter]->getWDrawn();
        double SD = m_mainBlockList[iter]->m_lde.SD;
        double FW = WDrawn / NF;
        double LOD = (2 * SA) + ( NF + 2 ) * (LDrawn) + ( NF + 1 ) * SD;
        double BL = m_mainBlockList[iter]->m_lde.BL;
        double BR = m_mainBlockList[iter]->m_lde.BR;
        double BT = m_mainBlockList[iter]->m_lde.BT;
        double BB = m_mainBlockList[iter]->m_lde.BB;

        switch (m_mainBlockList[iter]->m_lde.eDeviceType){
            case NMOS:{
                if(m_mainBlockList[iter]->m_lde.isBodyDetached){ // is BodyDetached
                    if((BL && !BR) || (!BL && BR)){
                        //m_mainBlockList[iter].m_width = 0.3 + 0.12 + LOD;
                    }
                    else if(!BL && !BR){
                        //m_mainBlockList[iter].m_width = LOD;
                    }
                    else if(BL && BR){
                        //m_mainBlockList[iter].m_width = 2 * (0.3 + 0.12) + LOD;
                    }
                    else { }

                    if(BL)
                        m_mainBlockList[iter]->m_lde.LDE_SA = SA + 0.42;
                    if(BR)
                        m_mainBlockList[iter]->m_lde.LDE_SB = SA + 0.42;

                    if( (BT && !BB) || (!BT && BB) ){
                        //m_mainBlockList[iter].m_height = FW + 0.36 + 0.12;
                    }
                    else if(!BT && !BB){
                        //m_mainBlockList[iter].m_height = FW + 2 * 0.12;
                    }
                    else if(BT && BB){
                        //m_mainBlockList[iter].m_height = FW + 2 * (0.36 + 0.12);
                    }
                    else { }

                    if(BB)
                        m_mainBlockList[iter]->m_lde.Y2 = 0.36;

                }
                else{   // not BodyDetached
                    //m_mainBlockList[iter].m_width = LOD;
                    //m_mainBlockList[iter].m_height = FW + 2 * 0.12;
                }
            }
            break;

            case PMOS:{
                if(m_mainBlockList[iter]->m_lde.isBodyDetached){ // is BodyDetached
                    if((BL && !BR) || (!BL && BR)){
                        //m_mainBlockList[iter].m_width = 0.36 + 0.12 + LOD;
                    }
                    else if(!BL && !BR){
                        //m_mainBlockList[iter].m_width = LOD;
                    }
                    else if(BL && BR){
                        //m_mainBlockList[iter].m_width = 2 * (0.36 + 0.12) + LOD;
                    }
                    else { }

                    if(BL)
                        m_mainBlockList[iter]->m_lde.LDE_SA = SA + 0.48;
                    if(BR)
                        m_mainBlockList[iter]->m_lde.LDE_SB = SA + 0.48;

                    if( (BT && !BB) || (!BT && BB) ){
                        //m_mainBlockList[iter].m_height = FW + 0.42 + 0.12;
                    }
                    else if(!BT && !BB){
                        //m_mainBlockList[iter].m_height = FW + 2 * 0.12;
                    }
                    else if(BT && BB){
                        //m_mainBlockList[iter].m_height = FW + 2 * (0.42 + 0.12);
                    }
                    else { }

                    if(BB)
                        m_mainBlockList[iter]->m_lde.Y2 = 0.42;

                }
                else{   // not BodyDetached
                    //m_mainBlockList[iter].m_width = LOD;
                    //m_mainBlockList[iter].m_height = FW + 2 * 0.12;
                }
            } 
            break;

            default:
                break;
        }

    }

}

double Placer::mobRelChangeFunc_PMOS
(double SA, double NF, double FW, double SD, double LDrawn){
    double invSA = 0;
    for(int i = 0; i < NF; ++i){
        invSA += (1 / (SA + 0.5*LDrawn + i*(SD+LDrawn)));
    }
    invSA = invSA / NF;

    double a = 1 + ( P_lkuo / pow(LDrawn + P_xl, P_llodkuo) );
    double b = ( P_wkuo / pow(FW + P_xw + P_wlod, P_wlodkuo) );
    //double c = ( P_kvtho * ( TNOM + (P_tkuo * (TEMP-TNOM)) ) );
    //c = c / (TNOM * pow(LDrawn+P_xl, P_llodkuo) * pow(FW+P_xw+P_wlod, P_wlodkuo));
    double c = ( P_kuo / ( pow(LDrawn + P_xl, P_llodkuo) * pow(FW + P_xw + P_wlod, P_wlodkuo) ) );
    double d = ( 1 + P_tkuo * (TEMP/TNOM - 1));

    double Kstress_u0 = (a + b + c) * d;

    double mobility_relative_change = (2 * P_kuo / Kstress_u0) * invSA;

    return mobility_relative_change;
}

double Placer::mobRelChangeFunc_NMOS
(double SA, double NF, double FW, double SD, double LDrawn){
    double invSA = 0;
    for(int i = 0; i < NF; ++i){
        invSA += (1 / (SA + 0.5*LDrawn + i*(SD+LDrawn)));
    }
    invSA = invSA / NF;

    double a = 1 + ( N_lkuo / pow(LDrawn + N_xl, N_llodkuo) );
    double b = ( N_wkuo / pow(FW + N_xw + N_wlod, N_wlodkuo) );
    //double c = ( P_kvtho * ( TNOM + (P_tkuo * (TEMP-TNOM)) ) );
    //c = c / (TNOM * pow(LDrawn+P_xl, P_llodkuo) * pow(FW+P_xw+P_wlod, P_wlodkuo));
    double c = ( N_kuo / ( pow(LDrawn + N_xl, N_llodkuo) * pow(FW + N_xw + N_wlod, N_wlodkuo) ) );
    double d = ( 1 + N_tkuo * (TEMP/TNOM - 1));

    double Kstress_u0 = (a + b + c) * d;

    double mobility_relative_change = (2 * P_kuo / Kstress_u0) * invSA;

    return mobility_relative_change;
}

double Placer::getLODCost
(double FW, double NFIN, double mobRelChan, double mobRelChanRef, double SD, double LDrawn){
    double length = FW;
    double breadth = 2 * SA + NFIN * LDrawn + (NFIN-1) * SD;

    double R  = (length > breadth) ? 
                (breadth / length) : (length / breadth);
    double Rs = (m_outlineH > m_outlineW) ? 
                (m_outlineW / m_outlineH) : (m_outlineH / m_outlineW);
    double r = R - Rs;

    double a = abs(mobRelChan - mobRelChanRef);
    a = a / (1 + mobRelChanRef);
    
    double b = (2 * SA + NFIN * LDrawn + (NFIN-1) * SD) * FW;
    b = b / m_LAnorm;

    // define the cost weight    
    double totalCost = (0.5) * a + (0.25) * b + (0.25) * r * r;

    return totalCost;
}

double Placer::updateT(int iter){
    if(iter <= m_K){
        return (m_Temp1 * abs( (m_deltaSum/m_deltaCostCnt) / m_N / m_C ) );
    }
    else{    // if(iter > m_K)
        double tmp = ( m_Temp1 * abs( (m_deltaSum/m_deltaCostCnt) / m_N ) );
        return tmp;
    }
}



