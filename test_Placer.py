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
    
    blk1 = constructBlk('B1', 5, 5);    print("> Inserting B1")
    btree.insertT(blk1);                showBlkInfo(btree)

    blk2 = constructBlk('B2', 7, 2);    print("> Inserting B2")
    btree.insertT(blk2);                showBlkInfo(btree)

    blk3 = constructBlk('B3', 6, 4);    print("> Inserting B3")
    btree.insertT(blk3);                showBlkInfo(btree)

    blk4 = constructBlk('B4', 2, 5);    print("> Inserting B4")
    btree.insertT(blk4);                showBlkInfo(btree)

    blk5 = constructBlk('B5', 4, 7);    print("> Inserting B5")
    btree.insertT(blk5);                showBlkInfo(btree)

    blk6 = constructBlk('B6', 1.5, 2);    print("> Inserting B6")
    btree.insertT(blk6);                showBlkInfo(btree)

    blk7 = constructBlk('B7', 9, 12);   print("> Inserting B7")
    btree.insertT(blk7);                showBlkInfo(btree)

    blk8 = constructBlk('B8', 5.5, 3);  print("> Inserting B8")
    btree.insertT(blk8);                showBlkInfo(btree)

    btree.packing()
    #btree.printContouLines()

    #print("b1: ", blk1.getX(), ",", blk1.getY())
    #print("b2: ", blk2.getX(), ",", blk2.getY())
    #print("b3: ", blk3.getX(), ",", blk3.getY())
    #print("b4: ", blk4.getX(), ",", blk4.getY())

    blkList = list([blk1, blk2, blk3, blk4, blk5, blk6, blk7, blk8])
    btree.dumpCaseResult(blkList, "0_origin")

    btree.generalSwap(blk1, blk7)
    btree.packing()
    btree.dumpCaseResult(blkList, "1_after_swap_blk1_blk7")

    btree.connectSwap(blk2, blk1)
    btree.packing()
    btree.dumpCaseResult(blkList, "2_after_swap_blk2_blk1")

    btree.sameParentSwap(blk1, blk4)
    btree.packing()
    btree.dumpCaseResult(blkList, "3_after_swap_blk1_blk4")

    btree.rightRotateB(blk4)
    btree.packing()
    btree.dumpCaseResult(blkList, "4_after_rightRotateB_blk4")

    # show rotate blk
    blk2.rotate();
    btree.packing()
    btree.dumpCaseResult(blkList, "5_after_rotate_blk2")

    pltPacking.main()


def test_SAPlacement():
    print(">> Start testing simulated annealing based placement")
    #print("> On the case: ", "")
    tStart = time.process_time()

    alpha = "0.1"
    inDirectory = "./examples/"
    blkCaseList = list(["0.block", "ami33.block", "ami49.block", "apte.block", "hp.block", "xerox.block"])
    netCaseList = list(["0.nets",  "ami33.nets",  "ami49.nets",  "apte.nets",  "hp.nets",  "xerox.nets" ])
    
    outDirectory = "./results/"
    outCaseList = list(["0.txt",   "ami33.txt",   "ami49.txt",   "apte.txt",   "hp.txt",   "xerox.txt" ])
    
    placer = _Placer.Placer()
    placer.parseAll(alpha, inDirectory+blkCaseList[1], inDirectory+netCaseList[1])
    placer.place()
    placer.writeResult(outDirectory+outCaseList[1])

    tUsed = time.process_time() - tStart
    print(">> Running Time: ", tUsed, " sec.")

    pltPacking.main()


def test_performance():
    print("> Start testing performance")


def main():
    #test_treeOptPack()
    #test_SAPlacement()
    test_performance()

if __name__ == "__main__": main()
