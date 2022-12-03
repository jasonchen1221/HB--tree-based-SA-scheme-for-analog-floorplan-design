#include <iostream>
#include "placer.hpp"

using std::cout;
using std::endl;
time_t startTime;

int main(int argc, char* argv[]){
    
    startTime = clock();
    
    Placer placer;
    // alpha, blkFile, netFile, grpFile
    //placer.parseAll(argv[1], argv[2], argv[3], argv[4]);    
        /* testing parser */
/*        placer.parseAlpha(argv[1], false);
        placer.parseBlkFile(argv[2], false);
        placer.parseNetFile(argv[3], false);
        placer.parseGrpFile(argv[4], false);
    
    placer.setDevice();
    
    placer.constructHierarchicalModuleClusteringTree();
    placer.place();
*/  

    // Testing BTree and Contourlines
    BTree btree;

    Block* blk_1 = new Block; blk_1->setName("b1"); blk_1->setW(5); blk_1->setH(3);
    Block* blk_2 = new Block; blk_2->setName("b2"); blk_2->setW(7); blk_2->setH(2);
    Block* blk_3 = new Block; blk_3->setName("b3"); blk_3->setW(6); blk_3->setH(4);
    Block* blk_4 = new Block; blk_4->setName("b4"); blk_4->setW(2); blk_4->setH(5);

    btree.insertT(blk_1);
    cout << "Inserting blk_1" << endl;
    cout << "Print Tree: " << endl;
    btree.printTree();
    cout << "Print Tree List: " << endl;
    btree.printList();
    cout << blk_1->getX() << "," << blk_1->getY() << endl; 
    
    btree.insertT(blk_2);
    cout << "Inserting blk_2" << endl;
    cout << "Print Tree: " << endl;
    btree.printTree();
    cout << "Print Tree List: " << endl;
    btree.printList();
    cout << blk_2->getX() << "," << blk_2->getY() << endl; 

    btree.insertT(blk_3);
    cout << "Inserting blk_3" << endl;
    cout << "Print Tree: " << endl;
    btree.printTree();
    cout << "Print Tree List: " << endl;
    btree.printList();
    cout << blk_3->getX() << "," << blk_3->getY() << endl; 

    btree.insertT(blk_4);
    cout << "Inserting blk_4" << endl;
    cout << "Print Tree: " << endl;
    btree.printTree();
    cout << "Print Tree List: " << endl;
    btree.printList();
    cout << blk_4->getX() << "," << blk_4->getY() << endl; 

    cout << "test root:" << endl;
    cout << btree.getRoot()->getName() << " " 
         << btree.getRoot()->getX() << " "
         << btree.getRoot()->getW() << " "
         << btree.getRoot()->getH() << endl;

    // ContourLines will be set when packing
    cout << "----------Packing----------" << endl;
    btree.packing();
    std::cout << "Print Tree ContourLines: " << std::endl;
    btree.printContourLines();



    return 0;
}