#include "contourline.hpp"
#include <cassert>
#include <iostream>

/* ------------------------class ContourMgr------------------------ */
void ContourMgr::setWidth(double width){
    m_wide = width;
    m_dummy->m_front = nullptr;
    m_dummy->m_back = new ContourLine(m_dummy, nullptr, Interval(INIT, m_wide), INIT);
    // m_hight = INIT = 50
}

void ContourMgr::setHeight(double h1, double h2){
    m_maxX = 0;
    m_origin = h1;
    m_tall = h2;
    m_tail->m_bottom = nullptr;
    m_tail->m_up = new ContourLine_v(m_tail, nullptr, Interval_v(m_origin, m_tall), INIT);
    // m_width = INIT = 50
}

void ContourMgr::setWidth_HB(double width){
    m_wide = width;
    m_dummy->m_front = nullptr;
    m_dummy->m_back = new ContourLine(m_dummy, nullptr, Interval(0, m_wide));
    // m_hight = 0   
}

void ContourMgr::setHeight_HB(double h1, double h2){
    m_maxX = 0;
    m_origin = h1;
    m_tall = h2;
    m_tail->m_bottom = nullptr;
    m_tail->m_up = new ContourLine_v(m_tail, nullptr, Interval_v(m_origin, m_tall));
    // m_width = 0 
}

void ContourMgr::deleteL(ContourLine* die){
    /* delete horizontal contour */
    assert(die != nullptr);
    die->m_front->m_back = die->m_back;
    if(die->m_back != nullptr){
        die->m_back->m_front = die->m_front;
    }
    delete die;
}

void ContourMgr::deleteR(ContourLine_v* die){
    /* delete vertical contour */
    assert(die != nullptr);
    die->m_bottom->m_up = die->m_up;
    if(die->m_up != nullptr){
        die->m_up->m_bottom = die->m_bottom;
    }
    delete die;
}

void ContourMgr::reset(){
    clearContour();
    setWidth(INT_MAX);
    m_maxY = 0;
    m_maxX = 0;
}

void ContourMgr::reset_HB(){
    clearContour_HB();
    setWidth_HB(INT_MAX);
    m_maxY = 0;
}

void ContourMgr::printLines() const{
    ContourLine* ptr = m_dummy->m_back;
    while(ptr != nullptr){
        std::cout << ptr->m_height << " "
                  << "(" << ptr->getFirst() << ","
                  << ptr->getSecond() << ")";
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

    ContourLine_v* ptr_v = m_tail->m_up;
    while(ptr_v != nullptr){
        ContourLine_v* tmp_v = ptr_v->m_up;
        delete ptr_v;
        ptr_v = tmp_v;
    }
    m_tail->m_up = m_tail->m_bottom = nullptr;

}

void ContourMgr::clearContour_HB(){
    /* Same as clearContour() */
    ContourLine* ptr = m_dummy->m_back;
    while(ptr != nullptr){
        ContourLine* tmp = ptr->m_back;
        delete ptr;
        ptr = tmp;
    }
    m_dummy->m_back = m_dummy->m_front = nullptr;

    ContourLine_v* ptr_v = m_tail->m_up;
    while(ptr_v != nullptr){
        ContourLine_v* tmp_v = ptr_v->m_up;
        delete ptr_v;
        ptr_v = tmp_v;
    }
    m_tail->m_up = m_tail->m_bottom = nullptr;
}

double ContourMgr::insert(const Interval inter1, double h, double w){
    Interval_v inter2;
    
    /* =====================start of horizontal contour lines===================== */    
    /* h: height of the inserted block */
    /* inter1.first = Xmin of the inserted block */
    /* inter1.second = Xmax of the inserted block */

    /* avoid the null node (no width) */ 
    if(inter1.first == inter1.second) return 0;
    assert( inter1.first < inter1.second );

    /* First, locate the begining line and ending line of the inserted block */
    ContourLine* firstLine = nullptr;
    ContourLine* lastLine  = nullptr;
    double max = 0;

    ContourLine* ptr = m_dummy->m_back;
    /* Locate first line where inter1.first should be in: */
    /* check the contour height of inter1.first */
    while(ptr != nullptr){
        if(ptr->getFirst() <= inter1.first && ptr->getSecond() > inter1.first){
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
        if(ptr->getFirst() < inter1.second && ptr->getSecond() >= inter1.second ){
            lastLine = ptr;
            break;
        }
        ptr = ptr->m_back;
    }

    /* Finally, below are two cases for maintaining the list 
       "continuous" and delete the "invalid" lines */

    /* Case 1: only one line concerned */
    if(firstLine == lastLine){
        ContourLine* middleLine = new ContourLine(firstLine, nullptr, inter1, max + h);
        ContourLine* backLine = new ContourLine(
            middleLine, 
            firstLine->m_back, 
            Interval(inter1.second, firstLine->getSecond()),
            firstLine->m_height);
        middleLine->m_back = backLine;

        if(firstLine->m_back != nullptr){
            firstLine->m_back->m_front = backLine;
        }
        firstLine->m_back = middleLine;

        firstLine->reszieFromFront(inter1.first);

        /* delete the invalid line (i.e. Interval(X,X)) with interval width == 0 */
        if(firstLine->isValid() == false) 
            deleteL(firstLine);
        if(backLine->isValid() == false)
            deleteL(backLine);
    }
    /* Case 2: across more than one line */ 
    else{
        firstLine->reszieFromFront(inter1.first);
        lastLine->resizeFromBack(inter1.second);

        ContourLine* middleLine = 
        new ContourLine(firstLine, lastLine, inter1, max + h);

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

    /* update horizontal contour lines, i.e. vector<Contour*> m_vHorizontal */
    m_vHorizontal.clear();
    ptr = m_dummy->m_back;
    while(ptr != nullptr){
        Contour* tmp = new Contour;
        if( (m_vHorizontal.empty() == false) && (ptr->m_height == m_vHorizontal.back()->max) ){
            m_vHorizontal.back()->y = ptr->getSecond();
            m_vHorizontal.back()->w = ptr->getSecond() - m_vHorizontal.back()->x;
        }
        else{
            tmp->x = ptr->getFirst();
            tmp->y = ptr->getSecond();
            tmp->max = ptr->m_height;
            tmp->w = ptr->getSecond() - ptr->getFirst();
            m_vHorizontal.push_back(tmp);
        }
        ptr = ptr->m_back;
    }

    if(m_vHorizontal.back()->max == INIT){
        m_vHorizontal.pop_back();
    }

    //std::cout << "debug1: done here" << std::endl;
    
    for(int c = 0, size = m_vHorizontal.size(); c < size; ++c){
        if(c == 0){
            m_vHorizontal[c]->x = 
            (2 * INIT - (m_vHorizontal[c]->x + (double)m_vHorizontal[c]->w/(double)2))
            - (double)m_vHorizontal[c]->w/(double)2;

            m_vHorizontal[c]->w = m_vHorizontal[c]->y - m_vHorizontal[c]->x;
        }
        else{
            Contour* t = new Contour;
            t->x =
            (2 * INIT - (m_vHorizontal[c]->x + (double)m_vHorizontal[c]->w/(double)2))
            - (double)m_vHorizontal[c]->w/(double)2;
            t->y = t->x + m_vHorizontal[c]->w;
            t->w = m_vHorizontal[c]->w;
            t->max = m_vHorizontal[c]->max;
            m_vHorizontal.push_back(t);
        }
    }

    //std::cout << "debug2: done here" << std::endl;
    /* =====================End of horizontal contour lines===================== */  

    
    /* =====================Start of vertical contour lines===================== */
    /* h: height of the inserted block */
    /* inter2.first = Ymin of the inserted block */
    /* inter2.second = Ymax of the inserted block */
    int k = 0;
    inter2.first = max;
    inter2.second = max + h;

    /* avoid the null node (no height) */ 
    if(inter2.first == inter2.second) return 0;
    assert(inter2.first < inter2.second);
    
    /* First, locate the begining line and ending line of the inserted block */
    ContourLine_v* bottomLine = nullptr;
    ContourLine_v* upLine     = nullptr;
    double max2 = 0;

    ContourLine_v* pt = m_tail->m_up;

    /* Locate bottom line where inter2.first should be in: */
    /* check the contour wid location of inter2.first */
    while(pt != nullptr){
        if(pt->getFirst_v() <= inter2.first && pt->getSecond_v() > inter2.first){
            bottomLine = pt;
            if(pt->m_width == inter1.first && pt->m_width > max2){
                max2 = pt->m_width;
            }
            else if(pt->m_width > inter1.first && inter1.first > max2){
                max2 = inter1.first;
                k = 1;
            }
            else{
                max2 = inter1.first;
            }
            break;  // go out while loop
        }
        pt = pt->m_up;
    }

    /* Locate up line where inter.second should be in */
    while(pt != nullptr){
        if(pt->m_width == inter1.first && pt->m_width > max2){
            max2 = pt->m_width;
        }
        else if(pt->m_width > inter1.first && inter1.first > max2){
            max2 = inter1.first;
            k = 1;
        }
        else{
            max2 = inter1.first;
        }

        if(pt->getFirst_v() < inter2.second && pt->getSecond_v() >= inter2.second ){
            upLine = pt;
            break;
        }
        pt = pt->m_up;
    }

    //std::cout << "debug3: done here" << std::endl;

    if(k == 0){     
        /* Finally, below are two cases for maintaining the list 
       "continuous" and delete the "invalid" lines */

        /* Case 1: only one line concerned */
        if(bottomLine == upLine){
            ContourLine_v* midLine = new ContourLine_v(bottomLine, nullptr, inter2, max2 + w);
            ContourLine_v* bakLine = new ContourLine_v(midLine, bottomLine->m_up, 
                            Interval_v(inter2.second, bottomLine->getSecond_v()), bottomLine->m_width);
            midLine->m_up = bakLine;
            
            if(bottomLine->m_up != nullptr){
                bottomLine->m_up->m_bottom = bakLine;
            }
            bottomLine->m_up = midLine;
            bottomLine->reszieFromFront_v(inter2.first);
            
            /* delete the invalid line (i.e. Interval(X,X)) with interval height == 0 */
            if(bottomLine->isValid_v() == false)
                deleteR(bottomLine);
            if(bakLine->isValid_v() == false)
                deleteR(bakLine);
        }
        /* Case 2: across more than one line */ 
        else{
            bottomLine->reszieFromFront_v(inter2.first);
            upLine->resizeFromBack_v(inter2.second);
            ContourLine_v* midLine = new ContourLine_v(bottomLine, upLine, inter2, max2 + w);

            bottomLine->m_up = midLine;
            upLine->m_bottom = midLine;

            if(bottomLine->isValid_v() == false)
                deleteR(bottomLine);
            if(upLine->isValid_v() == false)
                deleteR(upLine);
        }

        //std::cout << "debug4: done here" << std::endl;
    
    }

    if(k == 1){ 
        if(bottomLine == upLine){

        }
        else{
            upLine->resizeFromBack_v(inter2.second);
            ContourLine_v* midLine = new ContourLine_v(bottomLine, upLine, 
                Interval_v(bottomLine->getSecond_v(), inter2.second), max2 + w);

            bottomLine->m_up = midLine;
            upLine->m_bottom = midLine;
        }
    }

    /* update the _maxX of the overall contour line */
    if( (max2+ w) > m_maxX ){
        m_maxX = max2 + w;
    }

    //std::cout << "debug5: done here" << std::endl;

    /* update vertical contour lines, i.e. vector<Contour*> m_vVertical */
    m_vVertical.clear();
    pt = m_tail->m_up;

    while(pt != nullptr){
        Contour* tmp = new Contour;
        if( (m_vVertical.empty() == false) && pt->m_width == m_vVertical.back()->max ){
            m_vVertical.back()->y = pt->getSecond_v();
            m_vVertical.back()->w = pt->getSecond_v() - m_vVertical.back()->x;
        }
        else{
            tmp->x = pt->getFirst_v();
            tmp->y = pt->getSecond_v();
            tmp->max = pt->m_width;
            tmp->w = pt->getSecond_v() - pt->getFirst_v();
            m_vVertical.push_back(tmp);
        }
        pt = pt->m_up;
    }

    if(m_vVertical.back()->max == INIT){
        m_vVertical.pop_back();
    }

    //std::cout << "debug6: done here" << std::endl;
    //std:: cout << "m_vVertical.size(): " << m_vVertical.size() << std::endl;

    for(int c = 0, size = m_vVertical.size(); c < size; ++c){
        Contour* t = new Contour;
        t->x = m_vVertical[c]->x;
        t->y = m_vVertical[c]->y;
        t->w = m_vVertical[c]->w;
        t->max = 2 * INIT - m_vVertical[c]->max;
        m_vVertical.push_back(t);
    }
    /* =====================End of vertical contour lines===================== */
    
    //std::cout << "debug8: done here" << std::endl;

    return max;
}

double ContourMgr::insert_HB(const Interval inter, double h, double w, bool cm, bool val, bool HI, bool VI, bool paircheck){
    /* HI : Horizontally insert contour */
    /* VI : Vertically insert contour */
    
    double max = 0;
    Interval_v inter2;
    ContourLine* pt = m_dummy->m_back;

    if(inter.first == inter.second) return 0;

    if(inter.first > inter.second && inter.second == 0){
        while(pt != nullptr){
            if(pt->getFirst() <= inter.first && pt->getSecond() > inter.first){
                if(pt->m_height > max){
                    max = pt->m_height;
                }
            }
            pt = pt->m_back;
        }
        return max;     // return the lower y of the inserted block
    }

    assert(inter.first < inter.second);

/* cm == true */
    if(cm == true){
        while(pt != nullptr){
            if(pt->getFirst() <= inter.first && pt->getSecond() > inter.first ){
                if(pt->m_height > max  && pt->m_height > h){
                    max = pt->m_height - h;
                }
                break;
            }
            pt = pt->m_back;
        }
        
        // Locate last line where inter.second should be in
        while(pt != nullptr){
            if(pt->m_height - h > max){
                max = pt->m_height - h;
            }
            if(pt->getFirst() < inter.second && pt->getSecond() >= inter.second){
                break;
            }
            pt = pt->m_back;
        }
        return max;
    }

/* HI == true */
    if(HI == true){
        ContourLine* firstLine = nullptr;
        ContourLine* lastLine = nullptr;

        // Locate first line where inter.first should be in
        while(pt != nullptr){
            if(pt->getFirst() <= inter.first && pt->getSecond() > inter.first){
                firstLine = pt;
                if(pt->m_height > max){
                    max = pt->m_height;
                }
                break;
            }
            pt = pt->m_back;
        }

        // Locate last line where inter.second should be in
        while(pt != nullptr){
            if(pt->m_height > max){
                max = pt->m_height;
            }
            if(pt->getFirst() < inter.second && pt->getSecond() >= inter.second){
                lastLine = pt;
                break;
            }
            pt = pt->m_back;
        }

        if(val == true){
            h = h - max;
        }

        // case 1 : the inserted contour just inside one line
        if(firstLine == lastLine){
            ContourLine* middleLine = new ContourLine(firstLine, nullptr, inter, max + h);
            ContourLine* backLine   = new ContourLine(middleLine, firstLine->m_back,
                        Interval(inter.second, firstLine->getSecond()), firstLine->m_height);
            middleLine->m_back = backLine;
            if(firstLine->m_back != nullptr){
                firstLine->m_back->m_front = backLine;
            }
            firstLine->m_back = middleLine;
            firstLine->reszieFromFront(inter.first);

            // delete the invalid line (i.e. Interval(X,X)) with interval width = 0
            if(firstLine->isValid() == false)
                deleteL(firstLine);
            if(backLine->isValid() == false)
                deleteL(backLine);
        }
        // Case 2: across more than one line
        else{
            firstLine->reszieFromFront(inter.first);
            lastLine->resizeFromBack(inter.second);
            ContourLine* middleLine = new ContourLine(firstLine, lastLine, inter, max + h);

            ContourLine* pt = firstLine->m_back;

            firstLine->m_back = middleLine;
            lastLine->m_front = middleLine;

            // delete the lines covered by the new bigger one
            while(pt != lastLine && pt != nullptr){
                ContourLine* tmp = pt->m_back;
                delete pt;
                pt = tmp;
            }

            // delete invalid line (i.e. Interval(X,X)) with interval width = 0
            if(firstLine->isValid() == false)
                deleteL(firstLine);
            if(lastLine->isValid() == false)
                deleteL(lastLine);
        }

        // update the _maxY of the overall contour line
        if( (max + h) > m_maxY ){
            m_maxY = max + h;
        }

        // update the m_vHorizontal
        m_vHorizontal.clear();
        pt = m_dummy->m_back;

        while(pt != nullptr){
            Contour* temp = new Contour;
            if( (m_vHorizontal.empty() == false) && (pt->m_height == m_vHorizontal.back()->max)){
                m_vHorizontal.back()->y = pt->getSecond();
                m_vHorizontal.back()->w = pt->getSecond() - m_vHorizontal.back()->x;
            }
            else{
                temp->x = pt->getFirst();
                temp->y = pt->getSecond();
                temp->max = pt->m_height;
                temp->w = pt->getSecond() - pt->getFirst();
                m_vHorizontal.push_back(temp);
            }
            pt = pt->m_back;
        }

        if(m_vHorizontal.back()->max == INIT){
            m_vHorizontal.pop_back();
        }
    }


/* VI == true */
    if(VI == true){
        int k = 0;
        double inter_first;
        
        if(paircheck == true){
            inter2.first = max;
            inter2.second = max + h;
            inter_first = inter.first;
        }
        else{
            inter2.first = inter.first;
            inter2.second = inter.second;
            inter_first = h;
        }

        if(inter2.first == inter2.second) return 0;
        assert(inter2.first < inter2.second);

        ContourLine_v* bottomLine = nullptr;
        ContourLine_v* upLine = nullptr;
        double max2 = 0;

        ContourLine_v* ptr = m_tail->m_up;

        while(ptr != nullptr){
            if(ptr->getFirst_v() <= inter2.first && ptr->getSecond_v() > inter2.first){
                bottomLine = ptr;
                if(ptr->m_width == inter_first && ptr->m_width > max2){
                    max2 = ptr->m_width;
                }
                else if(ptr->m_width > inter_first && inter_first > max2){
                    max2 = inter_first;
                    k = 1;
                }
                else{
                    max2 = inter_first;
                }
                break;
            }
            ptr = ptr->m_up;
        }

        while(ptr != nullptr){
            if(ptr->m_width == inter_first && ptr->m_width > max2){
                max2 = ptr->m_width;
            }
            else if(ptr->m_width > inter_first && inter_first > max2){
                max2 = inter_first;
                k = 1;
            }
            else{
                max2 = inter_first;
            }

            if(ptr->getFirst_v() < inter2.second && ptr->getSecond_v() >= inter2.second){
                upLine = ptr;
                break;
            }
            ptr = ptr->m_up;
        }

        if(k == 0){
            if(bottomLine == upLine){
                ContourLine_v* midLine = new ContourLine_v(bottomLine, nullptr, inter2, max2 + w);
                ContourLine_v* bakLine = new ContourLine_v(midLine, bottomLine->m_up,
                    Interval_v(inter2.second, bottomLine->getSecond_v()), bottomLine->m_width);
                
                midLine->m_up = bakLine;
                if(bottomLine->m_up != nullptr){
                    bottomLine->m_up->m_bottom = bakLine;
                }
                bottomLine->m_up = midLine;
                bottomLine->reszieFromFront_v(inter2.first);

                if(bottomLine->isValid_v() == false)
                    deleteR(bottomLine);
                if(bakLine->isValid_v() == false)
                    deleteR(bakLine);
            }
            else{
                bottomLine->reszieFromFront_v(inter2.first);
                upLine->resizeFromBack_v(inter2.second);
                ContourLine_v* midLine = new ContourLine_v(bottomLine, upLine, inter2, max2 + w);

                bottomLine->m_up = midLine;
                upLine->m_bottom = midLine;

                if(bottomLine->isValid_v() == false)
                    deleteR(bottomLine);
                if(upLine->isValid_v() == false)
                    deleteR(upLine);
            }
        }

        if(k == 1){
            if(bottomLine == upLine){
                // ContourLine_v* midLine=bottomLine;
            }
            else{
                upLine->resizeFromBack_v(inter2.second);
                ContourLine_v* midLine = new ContourLine_v(bottomLine, upLine, 
                    Interval_v(bottomLine->getSecond_v(), inter2.second), max2 + w);

                bottomLine->m_up = midLine;
                upLine->m_bottom = midLine;
            }
        }

        // update the packing right most boundary
        if((max2 + w) > m_maxX){
            m_maxX = max2 + w;
        } 

        // update the vertical contourlines
        m_vVertical.clear();
        ptr = m_tail->m_up;

        while(ptr != nullptr){
            Contour* tmp = new Contour;
            if( (m_vVertical.empty() == false) && ptr->m_width == m_vVertical.back()->max ){
                m_vVertical.back()->y = ptr->getSecond_v();
                m_vVertical.back()->w = ptr->getSecond_v() - m_vVertical.back()->x;
            }
            else{
                tmp->x = ptr->getFirst_v();
                tmp->y = ptr->getSecond_v();
                tmp->max = ptr->m_width;
                tmp->w = ptr->getSecond_v() - ptr->getFirst_v();
                m_vVertical.push_back(tmp);
            }
            ptr = ptr->m_up;
        }
        
        if(m_vVertical.back()->max == INIT){
            m_vVertical.pop_back();
        }

        for(int c = 0, size = m_vVertical.size(); c < size; ++c){
            Contour* t = new Contour;
            t->x = m_vVertical[c]->x;
            t->y = m_vVertical[c]->y;
            t->w = m_vVertical[c]->w;
            t->max = 2 * INIT - m_vVertical[c]->max;

            m_vVertical.push_back(t);
        }
    }

    else { }

    return max; // return lower y for the inserted block!!
}