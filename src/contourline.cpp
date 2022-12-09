#include "../headers/contourline.hpp"
#include <cassert>
#include <iostream>

/* ------------------------class ContourMgr------------------------ */
void ContourMgr::setWidth(double width){
    m_width = width;
    m_dummy->m_front = nullptr;
    m_dummy->m_back = new ContourLine(m_dummy, nullptr, Interval(0, m_width));
    // m_hight = INIT = 50
}

void ContourMgr::deleteL(ContourLine* die){
    /* delete horizontal contour */
    assert(die != m_dummy);
    die->m_front->m_back = die->m_back;
    if(die->m_back != nullptr){
        die->m_back->m_front = die->m_front;
    }
    delete die;
}

void ContourMgr::reset(){
    clearContour();
    setWidth(INT_MAX);
    m_maxY = 0;
}

void ContourMgr::printLines() const{
    ContourLine* ptr = m_dummy->m_back;
    while(ptr != nullptr){
        std::cout << ptr->m_height << " "
                  << "(" << ptr->getFirst() << ","
                  << ptr->getSecond() << ") ";
        /* print ptr->front */
        if(ptr->m_front != nullptr){
            std::cout << "F: " << ptr->m_front->m_intvLine.first << " "
                      << ptr->m_front->m_intvLine.second << " ";
        }
        else{
            std::cout << "F: xxx ";
        }
        /* print ptr->back */
        if(ptr->m_back != nullptr){
            std::cout << "B: " << ptr->m_back->m_intvLine.first << " "
                      << ptr->m_back->m_intvLine.second << " ";
        }
        else{
            std::cout << "B: xxx ";
        }

        std::cout << std::endl;
        ptr = ptr->m_back;
    }

}

void ContourMgr::clearContour(){
    ContourLine* ptr = m_dummy->m_back;
    while(ptr != nullptr){
        ContourLine* tmp = ptr->m_back;
        delete ptr;
        ptr = tmp;
    }
    m_dummy->m_back = m_dummy->m_front = nullptr;
}

double ContourMgr::insert(const Interval inter, double h){
    // this function will return lower y for the inserted block!!

    /* =====================start of horizontal contour lines===================== */    
    /* h: height of the inserted block */
    /* inter1.first = Xmin of the inserted block */
    /* inter1.second = Xmax of the inserted block */

    /* avoid the null node (no width) */ 
    if(inter.first == inter.second) return 0;
    assert( inter.first < inter.second );

    /* First, locate the begining line and ending line of the inserted block */
    ContourLine* firstLine = nullptr;
    ContourLine* lastLine  = nullptr;
    double max = 0;

    ContourLine* ptr = m_dummy->m_back;
    /* Locate first line where inter1.first should be in: */
    /* check the contour height of inter1.first */
    while(ptr != nullptr){
        if(ptr->getFirst() <= inter.first && ptr->getSecond() > inter.first){
            firstLine = ptr;
            if(ptr->m_height > max){
                max = ptr->m_height;
            }
            break;  // go out while loop
        }
        ptr = ptr->m_back;
    }

    /* Locate last line where inter.second should be in */
    while(ptr != nullptr){
        if(ptr->m_height > max){
            max = ptr->m_height;
        }
        if(ptr->getFirst() < inter.second && ptr->getSecond() >= inter.second ){
            lastLine = ptr;
            break;
        }
        ptr = ptr->m_back;
    }

    /* Finally, below are two cases for maintaining the list 
       "continuous" and delete the "invalid" lines */

    /* Case 1: only one line concerned */
    if(firstLine == lastLine){
        ContourLine* middleLine = new ContourLine(firstLine, nullptr, inter, max + h);
        ContourLine* backLine = new ContourLine(
            middleLine, 
            firstLine->m_back, 
            Interval(inter.second, firstLine->getSecond()),
            firstLine->m_height);

        middleLine->m_back = backLine;

        if(firstLine->m_back != nullptr){
            firstLine->m_back->m_front = backLine;
        }
        firstLine->m_back = middleLine;

        firstLine->reszieFromFront(inter.first);

        /* delete the invalid line (i.e. Interval(X,X)) with interval width == 0 */
        if(firstLine->isValid() == false) 
            deleteL(firstLine);
        if(backLine->isValid() == false)
            deleteL(backLine);
    }
    /* Case 2: across more than one line */ 
    else{
        firstLine->reszieFromFront(inter.first);
        lastLine->resizeFromBack(inter.second);

        ContourLine* middleLine = 
        new ContourLine(firstLine, lastLine, inter, max + h);

        ContourLine* ptr = firstLine->m_back;
        firstLine->m_back = middleLine;
        lastLine->m_front = middleLine;

        /* delete the lines covered by the new bigger one */
        while(ptr != lastLine && ptr != nullptr){
            ContourLine* tmp = ptr->m_back;
            delete ptr;
            ptr = tmp;
        }

        /* delete invalid line i.e. Interval with interval width == 0 */
        if(firstLine->isValid() == false)
            deleteL(firstLine);
        if(lastLine->isValid() == false)
            deleteL(lastLine);
    }

    //std::cout << "debug0: done here" << std::endl;

    /* update the _maxY of the overall contour line */
    if(max + h > m_maxY){
        m_maxY = max + h;
    }

    // return lower y for the inserted block!!
    return max;
}
