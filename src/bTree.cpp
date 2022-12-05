#include "bTree.hpp"

#include <assert.h>

/* --------------------------------Debugging Functions-------------------------------- */
void BTree::printTree() const{
    // print the tree Graph
    if(isEmpty()){
        std::cout << "Tree is empty." << std::endl;
    }
    else{
        treePrint(m_root, 0);
    }
}             
    
void BTree::printList() const{
    // traverse the tree to print list
    Block* p_Blk = treeMin(m_root);
    int i = 0;
    while(p_Blk != m_dummyNode){
        std::cout << "[" << i++ << "]" << p_Blk->getName() << std::endl;
        p_Blk = treeSuccessor(p_Blk);
    }
}             
    
void BTree::printInvList() const{
    // inversely traverse the tree to print inverse list
    Block* p_Blk = treeMax(m_root);
    int i = 0;
    while(p_Blk != m_dummyNode){
        std::cout << "[" << i++ << "]" << p_Blk->getName() << std::endl;
        p_Blk = treePredeccessor(p_Blk);
    }
}
             
void BTree::printContourLines() const{
    // print the contourlines
    m_contourMgr.printLines();
}
    
void BTree::treePrint(Block* blk, int n) const{
    for(int i = 0; i < n; ++i){
        std::cout << "  ";
    }

    if(blk == nullptr){
        std::cout << "[0]" << std::endl;
        return;
    }
    else{
        std::cout << blk->getName() << std::endl;
        treePrint(blk->m_pRight, n + 1);    // upper info: right children
        treePrint(blk->m_pLeft, n + 1);     // lower info: left children
    }
}

void BTree::play(){

}

/* --------------------------------Binary Tree Operations-------------------------------- */
void BTree::insertT(Block* blk){
    /*
        0.  Tree structure: m_dummyNode -> m_root -> ...
        1.  Insert one new block into binary tree
        2.  If tree is not empty, when insert a new blk,
            start from m_root
            if new blk is wider than ptr's -> add as left child
            else if new blk is not wider than ptr's -> add as right child
    */
    if(isEmpty()){
        m_root = blk;
        m_dummyNode->m_pLeft = m_dummyNode->m_pRight = m_root;
        blk->m_pParent = m_dummyNode;
    }
    else{
        Block* ptr = m_root;
        Block* pos = ptr;
        
        while(ptr != nullptr){
            pos = ptr;
            if(blk->m_width > ptr->m_width){  // wider width goes to left child
                ptr = ptr->m_pLeft;     // move to left
            }
            else{
                ptr = ptr->m_pRight;    // move to right
            }
        }

        if(blk->m_width > pos->m_width){
            pos->m_pLeft = blk;
        }
        else{
            pos->m_pRight = blk;
        }

        blk->m_pParent = pos;
    }

    m_size++;

    assert(m_size > 0);
}  

void BTree::insertRnd(Block* blk){
    /*
        1.  Insert one new blk
        2.  50% possibility put to right
            50% possibility put to left
    */
    static bool LR = false; // true: left ; false: right

    if(isEmpty()){
        m_root = blk;
        m_dummyNode->m_pLeft = m_dummyNode->m_pRight = m_root;
        blk->m_pParent = m_dummyNode;
    }
    else{
        Block* ptr = m_root;
        Block* pos = ptr;

        while(ptr != nullptr){
            pos = ptr;
            if(rand() % 2){   // 50% possibility put to right
                ptr = ptr->m_pRight;                
                LR = false;
            }
            else{   // 50% possibility put to left
                ptr = ptr->m_pLeft;
                LR = true;
            }
        }

        if(LR == true){
            pos->m_pLeft = blk;
        }
        else{
            pos->m_pRight = blk;
        }

        blk->m_pParent = pos;
    }

    m_size++;

    assert(m_size > 0);
}

void BTree::deleteT(Block* ptr){
    /*
        1.  delete one block(which is ptr pointing to) in the binary tree
        2.  case1 : leaf node: just delete
            case2 : w/ left children but w/o right child : replace the deleted one with it's left child
            case3 : w/ right children but w/o left child : replace the deleted one with it's right child
            case4 : w/ left children and w/ right children
                 first find the ptr's right child's left-most child (Min)
                 replace the deleted one with this "Min node"
                 ("Min node" is replaced by its right child)
    */
    
    assert(m_size != 0);

    bool isRoot = false;
    if(ptr->m_pParent == m_dummyNode){
        isRoot = true;
    }

    // Case1
    if(ptr->m_pLeft == nullptr && ptr->m_pRight == nullptr){
        if(isRoot == false){
            if(ptr == (ptr->m_pParent)->m_pLeft){
                (ptr->m_pParent)->m_pLeft = nullptr;
            }
            else{
                (ptr->m_pParent)->m_pRight= nullptr;
            }
        }
        else{   // isRoot == true
            m_dummyNode->m_pLeft = nullptr;
            m_dummyNode->m_pRight = nullptr;
            m_root = m_dummyNode;
        }
    }
    // Case2
    else if(ptr->m_pLeft != nullptr && ptr->m_pRight == nullptr){
        if(isRoot == false) treeNodeDieCover(ptr, ptr->m_pLeft, true);
        else                setRoot(ptr->m_pLeft, (ptr->m_pLeft)->m_pLeft, (ptr->m_pLeft)->m_pRight);
    }
    // Case3
    else if(ptr->m_pLeft == nullptr && ptr->m_pRight != nullptr){
        if(isRoot == false) treeNodeDieCover(ptr, ptr->m_pRight, true);
        else                setRoot(ptr->m_pRight, (ptr->m_pRight)->m_pLeft, (ptr->m_pRight)->m_pRight);
    }
    // Case4
    else{   // ptr has left child and right child
        Block* Min = treeMin(ptr->m_pRight);
        
        if(isRoot == false){
            if(Min->m_pParent != ptr){
                treeNodeDieCover(Min, Min->m_pRight, false);
                Min->m_pRight = ptr->m_pRight;
                (Min->m_pRight)->m_pParent = Min;
            }

            Min->m_pParent = ptr->m_pParent;
            if(ptr == (ptr->m_pParent)->m_pRight){
                (ptr->m_pParent)->m_pRight = Min;
            }
            else{
                (ptr->m_pParent)->m_pLeft = Min;
            }

            Min->m_pLeft = ptr->m_pLeft;
            (Min->m_pLeft)->m_pParent = Min;
        }
        else{
            if(Min->m_pParent != ptr){
                treeNodeDieCover(Min, Min->m_pRight, false);
                Min->m_pRight = ptr->m_pRight;
                if((ptr->m_pRight)->m_pRight != nullptr){
                    (Min->m_pRight)->m_pParent = Min;
                }
            }
            setRoot(Min, ptr->m_pLeft, Min->m_pRight);
            (ptr->m_pLeft)->m_pParent = m_root;
        }
    }

    --m_size;
    assert(m_size >= 0);
    return;
}

void BTree::setRoot(Block* r, Block* left = nullptr, Block* right = nullptr){
    /*
        set m_root as r
        set left and right by arguments
    */
    m_root = r;
    m_root->m_pParent = m_dummyNode;
    m_dummyNode->m_pLeft = m_dummyNode->m_pRight = m_root;
    m_root->m_pLeft = left;
    m_root->m_pRight = right;
}

void BTree::treeNodeDieCover(Block* die, Block* cover, bool kill = true){
    if(cover == nullptr){   // Block die become nullptr (Block cover(nullptr) replace Block die)
        if(die == (die->m_pParent)->m_pLeft){
            (die->m_pParent)->m_pLeft = nullptr;
        }
        else{
            (die->m_pParent)->m_pRight = nullptr;
        }
    }
    else{   // Block cover replace Block die
        cover->m_pParent = die->m_pParent;
        if(die == (cover->m_pParent)->m_pLeft){
            (cover->m_pParent)->m_pLeft = cover;
        }
        else{
            (cover->m_pParent)->m_pRight = cover;
        }
    }
}

void BTree::reset(){
    /*
        clear the binary tree
    */
    m_root = m_dummyNode;
    m_dummyNode->m_pLeft = m_dummyNode->m_pRight = nullptr;
    m_size = 0;
}

Block* BTree::treeMin(Block* ptr) const{
    assert(ptr != nullptr);

    while(ptr->m_pLeft != nullptr){
        ptr = ptr->m_pLeft;
    }
    return ptr;
}

Block* BTree::treeMax(Block* ptr) const{
    assert(ptr != nullptr);

    while(ptr->m_pRight != nullptr){
        ptr = ptr->m_pRight;
    }
    return ptr;
}

Block* BTree::treeSuccessor(Block* ptr) const{
    /*
        1.  This function: to get ptr's successor
        2.  If ptr has right child, return ptr's right child's treeMin
            If ptr doesn't have right child and ptr is someone's left child, return ptr's parent 
            If not, recursively goes up until the node is not someone's right child
    */
    assert(ptr != nullptr);
    
    if(ptr->m_pRight != nullptr){
        return treeMin(ptr->m_pRight);
    }

    Block* tmp = ptr->m_pParent;
    while(tmp != nullptr && ptr == tmp->m_pRight){
        ptr = tmp;
        tmp = tmp->m_pParent;
    }

    if(tmp == nullptr){
        return ptr;
    }
    else{
        return tmp;     // the last one will return dummy
    }
}

Block* BTree::treePredeccessor(Block* ptr) const{
    assert(ptr != nullptr);
    if(ptr->m_pLeft != nullptr){
        return treeMax(ptr->m_pLeft);
    }    

    Block* tmp = ptr->m_pParent;
    while(tmp != nullptr && ptr == tmp->m_pLeft){
        ptr = tmp;
        tmp = tmp->m_pParent;
    }

    if(tmp == nullptr){
        return ptr;
    }
    else{
        return tmp;     // the last one will return dummy
    }
}


/* -------------------------------------Packing------------------------------------- */
void BTree::compactX(Block* blk, bool isFirst){
    /* x-direction compaction */
    /* set vertical contourlines */
    if(blk == nullptr && isFirst){
        blk = m_root;
        m_contourMgr.reset();
        m_root->m_x = 0;
        m_root->m_y = 0;
        m_contourMgr.insert(
            Interval(m_root->m_y, m_root->m_y + m_root->m_height),
            m_root->m_width
        );
    }

    Block* left  = blk->m_pLeft;
    Block* right = blk->m_pRight;
    Interval inter;

    // update m_width * m_height
    if(blk->m_x + blk->m_width > m_width)
        m_width = blk->m_x + blk->m_width;
    if(blk->m_y + blk->m_height > m_height)
        m_height = blk->m_y + blk->m_height;

    if(right != nullptr){
        inter.first = right->m_y;
        inter.second = right->m_y + right->m_height;
        right->m_x = m_contourMgr.insert(inter, right->m_width);
        compactX(right, false);
    }
    if(left != nullptr){
        inter.first = left->m_y;
        inter.second = left->m_y + left->m_height;
        left->m_x = m_contourMgr.insert(inter, left->m_width);
        compactX(left, false);
    }

    return;
}

void BTree::compactY(Block* blk){
    /* y-direction compaction */
    /* set horizontal contourlines */
    Block* left = blk->m_pLeft;
    Block* right = blk->m_pRight;
    Interval inter;

    if(left != nullptr){
        inter.first = left->m_x;
        inter.second = left->m_x + left->m_width;
        left->m_y = m_contourMgr.insert(inter, left->m_height);
        compactY(left);
    }

    if(right != nullptr){
        inter.first = right->m_x;
        inter.second = right->m_x + right->m_width;
        right->m_y = m_contourMgr.insert(inter, right->m_height);
        compactY(right);
    }

    return;
}

void BTree::treePack(Block* blk){
    Block* left  = blk->m_pLeft;
    Block* right = blk->m_pRight;
    Interval inter;

    // Update the boundary of placement
    if(blk->m_x + blk->m_width > m_width){
        m_width = blk->m_x + blk->m_width;
    }
    if(blk->m_y + blk->m_height > m_height){
        m_height = blk->m_y + blk->m_height;
    }

    //std::cout << "debug9: done here" << std::endl;

    if(left != nullptr){
        //std::cout << "debug10: done here" << std::endl;
        left->m_x = blk->m_x + blk->m_width;
        inter.first = left->m_x;
        inter.second = left->m_x + left->m_width;
        left->m_y = m_contourMgr.insert(inter, left->m_height);
        treePack(left);
    }

    if(right != nullptr){
        right->m_x = blk->m_x;
        inter.first = right->m_x;
        inter.second = right->m_x + right->m_width;
        right->m_y = m_contourMgr.insert(inter, right->m_height);
        treePack(right);
    }

    return;
}

void BTree::packing(){
    if(isEmpty()) return;

    m_width = 0;
    m_height = 0;
    m_contourMgr.reset();
    m_root->m_x = 0;
    m_root->m_y = 0;

    m_contourMgr.insert(
        Interval(m_root->m_x, m_root->m_x + m_root->m_width),
        m_root->m_height
    );

    treePack(m_root);
    
    assert(m_contourMgr.getMaxY() == m_height);
}


/* -----------------------------B*-tree Perturbation Methods----------------------------- */

void BTree::swapBlk(Block* blkA, Block* blkB){
    assert(blkA != nullptr && blkB != nullptr);

    if(blkA == blkB->m_pParent){
        // blkA is blkB's parent
        connectSwap(blkA, blkB);
    } 
    else if(blkA == blkB->m_pLeft || blkA == blkB->m_pRight){   
        // blkB is blkA's parent
        connectSwap(blkB, blkA);
    }
    else if(blkA->m_pParent == blkB->m_pParent){
        // blkA and blkB have same parent (blkA & blkB are siblings)
        sameParentSwap(blkA, blkB);
    }
    else{
        // blkA and blkB are not parent-child relationship, also not siblings
        generalSwap(blkA, blkB);
    }
}

void BTree::generalSwap(Block* blkA, Block* blkB){
    // generalSwap: blkA and blkB are not parent-child relationship, also not siblings

    Block* A_parent = blkA->m_pParent;
    Block* A_left   = blkA->m_pLeft;
    Block* A_right  = blkA->m_pRight;

    // replace blkB with blkA, and then raplace blkA with blkB

    // replace blkB with blkA
    blkA->m_pParent = blkB->m_pParent;
    blkA->m_pLeft   = blkB->m_pLeft;
    blkA->m_pRight  = blkB->m_pRight;

    if(blkB == blkB->m_pParent->m_pLeft)
        blkB->m_pParent->m_pLeft = blkA;
    else
        blkB->m_pParent->m_pRight = blkA;
    
    if(blkB->m_pLeft != nullptr)
        blkB->m_pLeft->m_pParent = blkA;
    if(blkB->m_pRight != nullptr)
        blkB->m_pRight->m_pParent = blkA;
    
    if(blkB->m_pParent == m_dummyNode){
        m_dummyNode->m_pLeft = m_dummyNode->m_pRight = blkA;
        m_root = blkA;
    }

    // raplace blkA with blkB
    blkB->m_pParent = A_parent;
    blkB->m_pLeft   = A_left;
    blkB->m_pRight  = A_right;

    if(blkA == A_parent->m_pLeft)
        A_parent->m_pLeft = blkB;
    else
        A_parent->m_pRight = blkB;

    if(A_left != nullptr)
        A_left->m_pParent = blkB;
    if(A_right != nullptr)
        A_right->m_pParent = blkB;
    
    if(A_parent == m_dummyNode){
        m_dummyNode->m_pLeft = m_dummyNode->m_pRight = blkB;
        m_root = blkB;
    }
    
}

void BTree::connectSwap(Block* parent, Block* child){
    static bool isLeft; // true: child == parent->left ; fasle: child == parent->right

    Block* tmp_parent = parent->m_pParent;
    Block* tmp_left   = parent->m_pLeft;
    Block* tmp_right  = parent->m_pRight;

    if(child == parent->m_pLeft) isLeft = true;
    else                         isLeft = false;

    if(isLeft == true){ // child is parent's left child
        // set parent
        parent->m_pLeft  = child->m_pLeft;
        parent->m_pRight = child->m_pRight;
        if(child->m_pLeft != nullptr)  
            child->m_pLeft->m_pParent = parent;
        if(child->m_pRight != nullptr) 
            child->m_pRight->m_pParent = parent;
        parent->m_pParent = child;

        // set child
        child->m_pLeft   = parent;
        child->m_pRight  = tmp_right;
        if(tmp_right != nullptr) 
            tmp_right->m_pParent = child;

        child->m_pParent = tmp_parent;

        if(parent == tmp_parent->m_pLeft)
            tmp_parent->m_pLeft = child;
        else
            tmp_parent->m_pRight= child;
    }
    else{   // isLeft == false  // child is parent's right child
        // set parent
        parent->m_pLeft  = child->m_pLeft;
        parent->m_pRight = child->m_pRight;
        if(child->m_pLeft != nullptr)  
            child->m_pLeft->m_pParent = parent;
        if(child->m_pRight != nullptr) 
            child->m_pRight->m_pParent = parent;
        parent->m_pParent = child;
        
        // set child
        child->m_pRight = parent;
        child->m_pLeft  = tmp_left;
        if(tmp_left != nullptr) 
            tmp_left->m_pParent = child;
        
        child->m_pParent = tmp_parent;
        
        if(parent == tmp_parent->m_pLeft) 
            tmp_parent->m_pLeft = child;
        else                               
            tmp_parent->m_pRight  = child;
    }

    if(tmp_parent == m_dummyNode){
        m_dummyNode->m_pLeft = m_dummyNode->m_pRight = child;
        m_root = child;
    }
}

void BTree::sameParentSwap(Block* blkA, Block* blkB){
    static bool isLeft; // true:  blkA is it's parent's left child
                        // false: blkA is it's parent's right child

    if(blkA == blkB) return;

    if(blkA == blkA->m_pParent->m_pLeft) isLeft = true;
    else                                 isLeft = false;

    Block* A_parent = blkA->m_pParent;
    Block* A_left   = blkA->m_pLeft;
    Block* A_right  = blkA->m_pRight;

    if(isLeft == true){
        A_parent->m_pLeft  = blkB;
        A_parent->m_pRight = blkA;
    }
    else{
        A_parent->m_pLeft  = blkA;
        A_parent->m_pRight = blkB;
    }

    blkA->m_pLeft  = blkB->m_pLeft;
    blkA->m_pRight = blkB->m_pRight;
    if(blkB->m_pLeft  != nullptr) blkB->m_pLeft->m_pParent = blkA;
    if(blkB->m_pRight != nullptr) blkB->m_pRight->m_pParent = blkA;

    blkB->m_pLeft  = A_left;
    blkB->m_pRight = A_right;
    if(A_left  != nullptr) A_left->m_pParent  = blkB;
    if(A_right != nullptr) A_right->m_pParent = blkB;

}

void BTree::leftRotateB(Block* blk){
    assert(blk != nullptr);

    Block* y = blk->m_pRight;
    if(y == nullptr) return;

    blk->m_pRight = y->m_pLeft;

    if(y->m_pLeft != nullptr){
        y->m_pLeft->m_pParent = blk;
    }

    y->m_pParent = blk->m_pParent;

    if(blk->m_pParent == m_dummyNode){
        m_dummyNode->m_pRight = m_dummyNode->m_pLeft = y;
        y->m_pParent = m_dummyNode;
        m_root = y;
    }
    else if(blk == blk->m_pParent->m_pLeft){
        blk->m_pParent->m_pLeft = y;
    }
    else{
        blk->m_pParent->m_pRight = y;
    }

    y->m_pLeft = blk;
    blk->m_pParent = y;
    
}

void BTree::rightRotateB(Block* blk){
    assert(blk != nullptr);

    Block* y = blk->m_pLeft;
    if(y == nullptr) return;

    blk->m_pLeft = y->m_pRight;

    if(y->m_pRight != nullptr){
        y->m_pRight->m_pParent = blk;
    }

    y->m_pParent = blk->m_pParent;

    if(blk->m_pParent == m_dummyNode){
        m_dummyNode->m_pLeft = m_dummyNode->m_pRight = y;
        y->m_pParent = m_dummyNode;
        m_root = y;
    }
    else if(blk == blk->m_pParent->m_pLeft){
        blk->m_pParent->m_pLeft = y;
    }
    else{
        blk->m_pParent->m_pRight = y;
    }

    y->m_pRight = blk;
    blk->m_pParent = y;

}

Block* BTree::deleteAndInsert(Block* blk){
    int r = rand() % m_nulls.size();    // choose a null node on the tree
    swapBlk(blk, m_nulls[r]);
    return m_nulls[r];  // ruturen the chosen null block*
}
