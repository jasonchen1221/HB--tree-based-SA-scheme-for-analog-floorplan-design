#include <iostream>
#include "placer.hpp"

using std::cout;
using std::endl;
time_t startTime;

int main(int argc, char* argv[]){
/*   
    startTime = clock();
    
    Placer placer;
    // alpha, blkFile, netFile, outputFile
    placer.parseAll(argv[1], argv[2], argv[3]);     

    placer.place();

    placer.writeResult(argv[4]);

    time_t endTime = clock();
    double t_used = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    cout << ">> Running Time: " << t_used << " sec." << endl;

*/


    // Testing BTree and Contourlines
    BTree btree;
    btree.reset();
    //btree.printTree();
    //btree.printList();

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

    cout << "b1: " << blk_1->getX() << "," << blk_1->getY() << endl; 
    cout << "b2: " << blk_2->getX() << "," << blk_2->getY() << endl; 
    cout << "b3: " << blk_3->getX() << "," << blk_3->getY() << endl; 
    cout << "b4: " << blk_4->getX() << "," << blk_4->getY() << endl; 
/*
    // test: generalSwap
    cout << "----------generalSwap----------" << endl;
    btree.generalSwap(blk_1, blk_3);
    cout << "Print Tree: " << endl;
    btree.printTree();
    cout << "Print Tree List: " << endl;
    btree.printList();
    //btree.packing();
    //btree.printContourLines();
    
    // test: connectSwap
    cout << "----------connectSwap----------" << endl;
    btree.connectSwap(blk_3, blk_2);
    cout << "Print Tree: " << endl;
    btree.printTree();
    cout << "Print Tree List: " << endl;
    btree.printList();

    // test: sameParentSwap
    cout << "----------sameParentSwap----------" << endl;
    btree.sameParentSwap(blk_4, blk_3);
    cout << "Print Tree: " << endl;
    btree.printTree();
    cout << "Print Tree List: " << endl;
    btree.printList();

    // test: leftRotateB

    // test: rightRotateB
*/

    vector<Block*> testVec{ blk_1, blk_2, blk_3, blk_4 };
    btree.dumpCaseResult(testVec);

    return 0;
}