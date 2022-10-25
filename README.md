# HB*-tree based SA scheme for analog floorplan design


## Basic Information
Repository & URL: [jasonchen1221](https://github.com/jasonchen1221) for [HB*-tree based SA scheme for analog floorplan design](https://github.com/jasonchen1221/HB--tree-based-SA-scheme-for-analog-floorplan-design.git)
 
As far as I know, in modern VLSI circuit design, layout synthesis of digital circuit design(automated placement and routing) have developed quite maturely. However, the layout synthesis technique of analog circuit layout is far less than that of digital circuits, and most of them are still customized manually by professional layout person. 

The professional layout engineer focus on reduce the effects of parasitic mismatched and circuit sensitivity to process variations for analog circuits. Some pairs of modules need to be placed symmetrically with respect to common axis for better electrical properties. Therefore, if they lack some good reference topologies and their corresponding toatal area and total wirelength to refer to, it will cost a lot of time for analog layout topology design.

Therefore, it is important to handle the symmetrical modules and non-symmetrical modules simultaneously and efficiently, when we try to search feasible topologies with minimum area and wirelength in large solution space.

I did some researches and found that there are some works dedicated to this area. So I want to try this topic and apply what I have learned in class.

The example picture shown below(reference from [1]) is a resulting placement of biasynth_2p4g with three symmetry group(with 64 modules, including 25 symmetric modules).
![](https://i.imgur.com/dHend78.png)

## Problem to Solve
### Goal
Search for a desired floorplanning topology based on simulated annealing scheme.

The cost function of placement is as below, user can decide the weight of area and wire length depend on what they want,
```
    cost = alpha * Ap + (1-alpha) * Wp
    
    # Ap: area of bounding rectangle for the placement
    # Wp: toal half-perimeter wire length
    # alpha: user-specified parameters
```

### Data Structure & Algorithms
A fundamental problem to floorplanning/placement lies in the representation of geometric relationship among modules. As the problem size becomes larger and larger, the representation profoundly affects the operations of modules and the complexity of floorplan/placement design process. Its is that desired to develop an efficient, flexiblem and effective representation of geometric relationship for floorplan/placement designs.

In [1], Lin proposed an Hierarchical B*-tree(HB*-tree) data structure and algorithms that can efficiently handle the symmetric and non-symmetric modules when doing the floorplan/placement. It is based on concept of B*-tree which was proposed by Chang in [2]. Operations on HB*-tree nodes to do the packing and perturbation for analog placement only need time complexity O(lgn) and O(n), respectively. It is very efficient compare to other methods mentioned in [2]. 

Hence, I will use HB*-tree as the data structure to handle the modules. And the algorithms are as below:
1. HB*-tree
    1. Insertion
    1. Deletion
    1. Contour nodes update
2. HB*-tree perturbation in floorplan process
    1. Op1: Rotate a module
    2. Op2: Move a node to another place
    3. Op3: Swap two nodes
    4. Op4: Change a representative
    5. Op5: Convert a symmetry type

## Perspective Users
Analog circuit designer, layout designer.


## System Architecture
### Flow Chart
![](https://i.imgur.com/otG0uNf.png)

1. Inputs
    1. Alpha value
    2. Symmetry informations
    3. Nets informations
    4. Blocks informations
2. Outputs
    1. Text file: Coordinates informations for each blocks after placement
    2. Picture : placement topology and HPWL analysis


## API Description
1. Use pytest to test the correctness of implementation of HB*-tree, also plot the HB*-tree packing results using Matplotlib in python
2. Use pybind11 as an interface to set the input files, alpha value, and simulated annealing time.

## Engineering Infrastructure
1. Compilation & Execution: use make to compile, run, plot, clean
2. Version control: use git

## Schedule
**Before Week 1:**
* Planning phase (from 10/10 to 10/31): 
    1. Study related paper and works
    2. Prepare proposal

**Week 1 (10/31):**
* Implement parser and basic data structures(Block)
* Start the presentation skeleton. Contents will be added to the presentation along with the code development.


**Week 2 (11/7):**
* Develop HB*-tree data structure and related operations
    1. Node insertion, deletion
    2. contour line
    3. packing 
* Construcy the pytest structure to test the correctness of HB*-tree

**Week 3 (11/14):**
* Develop HB*-tree data structure and related operations(cont.)
* Using Matplotlib to implement the Visualization of the HB*-tree packing results.(Also for viewing perturbation operations results in later weeks)

**Week 4 (11/21):**
* Refactoring week (and accommodation of overflown work).

**Week 5 (11/28)**
* Develop HB*-tree perturbation operations

**Week 6 (12/5):**
* Develop simulated annealing framework for floorplan/placement process
* Apply the larger testcases to the program to see if anywhere need to modify, debug.

**Week 7 (12/12):**
* Refactoring week (and accommodation of overflown work).

**Week 8 (12/19):**
* Overflown work. 

## References
* [1] [Analog Placement Based on Symmetry Island Formulation - by Po-Hung Lin](https://ieeexplore-ieee-org.ezproxy.lib.nctu.edu.tw/document/4957593)
* [2] [B*-Trees: A New Representation for Non-Slicing Floorplans - by Yun-Chih Chang](https://ieeexplore-ieee-org.ezproxy.lib.nctu.edu.tw/document/855354)

