import pytest
import time
import _Placer
import plot as pltPacking

def constructBTree():
    btree = _Placer.BTree(1)
    btree.reset()
    return btree

def constructBlk(name, w, h):
    blk = _Placer.Block()
    blk.setName(name)
    blk.setW(w)
    blk.setH(h)
    return blk

def showBlkInfo(btree):
    print("=================Tree==================")
    btree.printTree()
    print("---------------Tree List---------------")
    btree.printList()
    print("------------------End------------------")

def test_treeOptPack():
    print("> Start testing tree operations and packing")
    btree = constructBTree()
    
    blk1 = constructBlk('B1', 5, 3); print("> Inserting B1")
    btree.insertT(blk1);             showBlkInfo(btree)

    blk2 = constructBlk('B2', 7, 2); print("> Inserting B2")
    btree.insertT(blk2);             showBlkInfo(btree)

    blk3 = constructBlk('B3', 6, 4); print("> Inserting B3")
    btree.insertT(blk3);             showBlkInfo(btree)

    blk4 = constructBlk('B4', 2, 5); print("> Inserting B4")
    btree.insertT(blk4);             showBlkInfo(btree)

    btree.packing()
    btree.printContouLines()

    print("b1: ", blk1.getX(), ",", blk1.getY())
    print("b2: ", blk2.getX(), ",", blk2.getY())
    print("b3: ", blk3.getX(), ",", blk3.getY())
    print("b4: ", blk4.getX(), ",", blk4.getY())

    blkList = list([blk1, blk2, blk3, blk4])
    btree.dumpCaseResult(blkList, "fig1")

    pltPacking.main()

def test_SAPlacement():
    print("> Start testing simulated annealing based placement")
    print("> On the case: ", "")


def test_alphaValue():
    print("> Start testing alpha value and performance")


def main():
    test_treeOptPack()
    #test_SAPlacement()
    #test_alphaValue()

if __name__ == "__main__": main()





