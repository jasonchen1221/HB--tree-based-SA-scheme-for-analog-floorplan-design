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
    
}