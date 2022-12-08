import pytest
import time
import _Placer

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

def main():
    btree = constructBTree()
    
    blk1 = constructBlk('B1', 5, 3)
    print("> Inserting B1")
    btree.insertT(blk1)
    showBlkInfo(btree)

    blk2 = constructBlk('B2', 7, 2)
    print("> Inserting B2")
    btree.insertT(blk2)
    showBlkInfo(btree)

    blk3 = constructBlk('B3', 6, 4)
    print("> Inserting B3")
    btree.insertT(blk3)
    showBlkInfo(btree)

    blk4 = constructBlk('B4', 2, 5)
    print("> Inserting B4")
    btree.insertT(blk4)
    showBlkInfo(btree)

    


if __name__ == "__main__": main()





