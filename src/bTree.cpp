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
}  

void BTree::insertRnd(Block* blk, bool isSelf){
    static bool LR = false; // true: left ; false: right

    if(isEmpty() || (isSelf && isEmpty())){
        m_root = blk;
        m_dummyNode->m_pLeft = m_dummyNode->m_pRight = m_root;
        blk->m_pParent = m_dummyNode;
    }
    else{
        Block* ptr = m_root;
        Block* pos = ptr;

        while(ptr != nullptr){
            pos = ptr;
            if(rand() % 2 || isSelf){   // is self must put to right , or 50% possibility put to right
                ptr = ptr->m_pRight;                
                LR = false;
            }
            else{   // 50% possibility put to right
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
}

void BTree::insertRnd_Hierarchy(Block* blk, bool isSelf){

}

void BTree::insertRnd_HB(Block* blk){

}

void BTree::deleteT(Block* blk){

}

void BTree::setRoot(Block* r, Block* left = nullptr, Block* right = nullptr){
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
    m_root = m_dummyNode;
    m_dummyNode = m_dummyNode = nullptr;
    m_size = 0;
}

Block* BTree::treeMin(Block* root) const{
    assert(root != nullptr);

    
}

Block* BTree::treeMax(Block* root) const{

}

Block* BTree::treeSuccessor(Block* root) const{

}

Block* BTree::treePredeccessor(Block* root) const{

}

/* -----------------------------B*-tree Perturbation Methods----------------------------- */
/*
void BTree::swapBlk(Block* blkA, Block* blkB){
    if(blkA == nullptr && blkB == nullptr){
        std::cerr << "error: function swapB, both blocks are nullptr" << std::endl;
    }

    if(blkA == blkB->m_pParent){
        // blkA is blkB's parent
        connectSwap(blkA, blkB);
    } 
    else if(blkA == blkB->m_pLeft || blkA == blkB->m_pRight){   
        // blkB is blkA's parent
        connectSwap(blkB, blkA);
    }
    else if(blkA->m_pParent == blkB->m_pParent){
        // blkA and blkB have same parent
        sameParentSwap(blkA, blkB);
    }
    else{
        generalSwap(blkA, blkB);
    }
}

void BTree::generalSwap(Block* blkA, Block* blkB){
    Block* A_parent = blkA->m_pParent;
    Block* A_left   = blkA->m_pLeft;
    Block* A_right  = blkA->m_pRight;

    // replace blkA with blkB, and then raplace blkB with blkA
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
        child->m_pParent = tmp_parent;
        child->m_pLeft   = parent;
        child->m_pRight  = tmp_right;
        if(tmp_right != nullptr) 
            tmp_right->m_pParent = child;
        if(tmp_parent->m_pRight == parent)
            tmp_parent->m_pRight = child;
        else
            tmp_parent->m_pLeft = child;
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
        child->m_pParent = tmp_parent;
        child->m_pLeft  = tmp_left;
        child->m_pRight = parent;
        if(tmp_left != nullptr) 
            tmp_left->m_pParent = child;
        if(tmp_parent->m_pRight == parent) 
            tmp_parent->m_pRight = child;
        else                               
            tmp_parent->m_pLeft  = child;
    }

    if(tmp_parent == m_dummyNode){
        m_dummyNode->m_pLeft = m_dummyNode->m_pRight = child;
        m_root = child;
    }
}

void BTree::sameParentSwap(Block* blkA, Block* blkB){

}

void BTree::swapBlk_Hierarchy(Block* blkA, Block* blkB){

}

void BTree::generalSwap_Hierarchy(Block* blkA, Block* blkB){

}

void BTree::rightConnectSwap_Hierarchy(Block* blkA, Block* blkB){

}

void BTree::leftConnectSwap_Hierarchy(Block* blkA, Block* blkB){

}

void BTree::sameParentSwap_Hierarchy(Block* blkA, Block* blkB){

}

Block* BTree::repChange(Block* blkA, Block* blkB){

}

void BTree::leftRotateB(Block* blk){

}

void BTree::rightRotateB(Block* blk){

}

Block* BTree::deleteAndInsert(Block* blk){

}
*/