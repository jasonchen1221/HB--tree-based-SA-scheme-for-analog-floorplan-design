#include "placer.hpp"
#include <map>
#include <cmath>

using std::cout;
using std::endl;

extern time_t startTime;

/* ---------------------------------Placer--------------------------------- */
/*void Placer::place(){
    cout << "Start Place..." << endl;

    int iter = 0;
    int numProcess = 0;
    int numTry = 0;

    while(isTimeout(startTime) == false){
        initSol();
        int rejectNum = 0;
        ++numProcess;
        iter = 0;
        m_N = 0;
        
        while(m_T > T_FROZEN) {
            ++numTry;
            ++m_N;
            ++iter;
            m_delta_cost_cnt = 0;
            m_delta_sum = 0;
            rejectNum = 0;
            
            for(int i = 0; i < m_L; ++i){
                genNeighbor();
                m_delta = getCost() - m_Cost;

                m_delta_cost_cnt++;
                m_delta_sum += fabs(m_delta);

                if(m_delta <= 0){   // move better
                    updateCost();
                    record();
                }
                else{   // move worse
                    if(shinbwei() == true){ //accept with prob.
                        updateCost();
                        record();
                    }
                    else{
                        recover();
                        ++rejectNum;
                    }
                }
            }

            m_T = T(iter);
            if(iter > m_I){
                break;
            }
        }

        if(m_optSol.empty() == false) break;
    }

    writeRecord();

    cout << "> #Process: " << numProcess << endl;
    cout << "> Try " << numTry << " placements." << endl;
    cout << "> Placement result:            " << endl;
    cout << "  - Chip Boundary = (0,0) (" << m_Width << "," << m_Height << ")" << endl;
    cout << "  - Chip Area     = " << getArea() << endl;
    cout << "  - Wire Length   = " << getTTLWireLen() << endl;
    cout << "  - Final Cost    = " << getFinalCost() << endl;

    cout << "Start Place[FINISHED]" << endl;
}*/

void Placer::initSol(){
    if(!(rand() % 6)){  // with prob. 5/6
        srand((unsigned)time(NULL));
    }

    /* Build a new B*-tree */
    if(m_BTree != nullptr) delete m_BTree;
    m_BTree = new BTree(m_vBlockList.size());

    /* Random generate a tree */
    for(int i = 0; i < m_vBlockList.size(); ++i){
        m_vBlockList[i]->setParent(nullptr);
        m_vBlockList[i]->setLeft(nullptr);
        m_vBlockList[i]->setRight(nullptr);
        m_BTree->insertRnd(m_vBlockList[i]);
    }

    m_BTree->packing();

    /* Initial Perturbation */
    initPerturb();

    /* Initialize SA related arguments */
    m_Cost = getCost();
    m_optSol.resize(0);
    m_uphill_sum        = 0;
    m_uphill_avg_cnt    = 0;
    m_delta_sum         = 0;
    m_delta_cost_cnt    = 0;

    m_T = T_INIT;           // 100000.0 (Inital temp.)
    m_N = N_INIT;           // 0.0
    m_P = P_INIT;           // 0.987 (Initial acceptance rate)
    m_k = K_INIT;           // 7     (User specified)
    //m_L = L_INIT;         // 200
    m_L = m_vBlockList.size() * 2 + 10;     // # of neighbors to search in a iteration
    m_C = C_INIT;           // 100   (User specified)
    //m_I = ITER_LIMIT;     // 1000
    m_I = m_vBlockList.size() * 20 + 10;    // # of iter upper bound

    m_w = W_INIT;           // 0.5
    m_x = X_INIT;           // 0.5 
    m_y = Y_INIT;           // 0.0
    m_z = Z_INIT;           // 0.0

    /* Testing and debug */
    m_BTree->printTree();
    m_BTree->printList();
    m_BTree->printInvList();
    m_BTree->printContourLines();

}

void Placer::initPerturb(){
    assert(m_BTree != nullptr);

    m_Anorm = m_BTree->getArea();
    m_Wnorm = getTTLWireLen();

    m_w = W_INIT;   // 0.5
    m_x = X_INIT;   // 0.5
    m_y = Y_INIT;   // 0.0
    m_z = Z_INIT;   // 0.0

    double curCost      = 0;
    double lastCost     = 0;
    double delta        = 0;
    double uphillsum    = 0;
    double sumA         = 0;
    double sumW         = 0;
    int    uphillcnt    = 0;

    for(int i = 0; i < INIT_PURTURB_NUM; ++i){  // 100
        genNeighbor();
        curCost = getCost();
        delta = curCost - lastCost;
        if(delta > 0){  // move worse
            uphillsum += delta;
            ++uphillcnt;
        }
        sumA     += getArea();
        sumW     += getTTLWireLen();
        lastCost  = curCost;
    }

    m_P     = P_INIT;                  // 0.987 (Initial acceptance rate)
    m_Anorm = sumA / INIT_PURTURB_NUM;
    m_Wnorm = sumW / INIT_PURTURB_NUM;

    m_T1 = ((double)uphillsum / (double)uphillcnt) / ((-1)*log(m_P));
    m_T  = m_T1 = 4;
}