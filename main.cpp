#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/attr.h>
#include "headers/placer.hpp"

namespace py = pybind11;

time_t startTime;

PYBIND11_MODULE(_Placer, m){
    m.doc() = "pybind11 plugin";
    py::class_<Block>(m, "Block")
        .def(py::init())
        .def("setName", &Block::setName)
        .def("setW",    &Block::setW)
        .def("setH",    &Block::setH)
        .def("getX",    &Block::getX)
        .def("getY",    &Block::getY);
    
    py::class_<BTree>(m, "BTree")
        .def(py::init<size_t>())
        .def("reset",            &BTree::reset)
        .def("insertT",          &BTree::insertT)
        .def("printTree",        &BTree::printTree)
        .def("printList",        &BTree::printList)
        .def("packing",          &BTree::packing)
        .def("printContouLines", &BTree::printContourLines)
        .def("generalSwap",      &BTree::generalSwap)
        .def("connectSwap",      &BTree::connectSwap)
        .def("sameParentSwap",   &BTree::sameParentSwap)
        .def("leftRotateB",      &BTree::leftRotateB)
        .def("rightRotateB",     &BTree::rightRotateB)
        .def("dumpCaseResult",   &BTree::dumpCaseResult)
        ;

    ;
}


