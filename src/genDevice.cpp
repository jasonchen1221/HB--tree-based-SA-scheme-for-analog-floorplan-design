#include "placer.hpp"
#include <cmath>

void Placer::genDevice(){
    // mobility miltiplier of LOD -> 1
    // eq(7), eq(8), eq(9)
    minimizeLOD();
    // to ensure better mathcing and further LOD minumization
    insertDummy();  



}

void Placer::minimizeLOD(){
    // priority: 0, 1, 2 -> do 0 & 1
    for(int p = 0; p < m_level.size() - 1; ++p){
        // w/ priority p, -> module list of m
        for(int m = 0; m < m_vConstraints[p].size(); ++m){
            if(m_vConstraints[p][m].m_vModuleList.size() > 0){
                // initial finger number
                int NF = m_vConstraints[p][m].m_vModuleList[0].getNFin();
                // transistor_width/NF
                double FW = m_vConstraints[p][m].m_vModuleList[0].getWDrawn() / NF;

                double sum = 0;
                double cnt = 0;

                double currCost = 0;
                double lastCost = 0;

                double uphillSum = 0;
                int uphillCnt = 0;
                
                double totalNFin; 
                double mobRelChan, mobRelChanRef;

                double thisLdrawn = m_vConstraints[p][m].m_vModuleList[0].getLDrawn();
                // m_LAnorm is different by process
                // @_@ estimate the width of device @_@
                m_LAnorm = (2 * SA + NF * thisLdrawn + (NF-1) * 0.2 ) * FW;


                while(FW > MIN_NF){
                    double SD = 0.2;    // initial poly-to-poly spacing
                    totalNFin = NF + m_vConstraints[p][m].m_vModuleList[0].m_lde.DUM;
                    double LDrawn = m_vConstraints[p][m].m_vModuleList[0].getLDrawn();

                    while(SD < MAX_SD){
                        if(m_vConstraints[p][m].m_vModuleList[0].m_lde.eDeviceType == PMOS){
                            mobRelChan    = mobRelChangeFunc_PMOS(SA * 1e-6, totalNFin, FW * 1e-6, SD * 1e-6, LDrawn * 1e-6);
                            mobRelChanRef = mobRelChangeFunc_PMOS(SA_REF * 1e-6, totalNFin, FW * 1e-6, SD * 1e-6, LDrawn * 1e-6);
                        }
                        else{   // NMOS
                            mobRelChan    = mobRelChangeFunc_NMOS(SA * 1e-6, totalNFin, FW * 1e-6, SD * 1e-6, LDrawn * 1e-6);
                            mobRelChanRef = mobRelChangeFunc_NMOS(SA_REF * 1e-6, totalNFin, FW * 1e-6, SD * 1e-6, LDrawn * 1e-6);
                        }

                        currCost = getLODCost(FW, totalNFin, mobRelChan, mobRelChanRef, SD, LDrawn);
                        m_delta = currCost - lastCost;
                        if(m_delta > 0){
                            uphillSum += m_delta;
                            uphillCnt++;
                        }
                        lastCost = currCost;
                        sum += FW * (2 * SA + NF * LDrawn + (NF-1) * SD);
                        cnt++;
                        SD = SD + 0.01;
                    }
                    NF++;
                    FW = m_vConstraints[p][m].m_vModuleList[0].getWDrawn() / NF;
                }

                m_LAnorm = sum / cnt;
                
            
            }
        }
    }
}

void Placer::insertDummy(){

}

void Placer::determineSize(){
    
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