#ifndef DEFINE_H
#define DEFINE_H

/* Simulated Annealing Algo inital value */
const double T_INIT             = 100000.0;
const double T_FROZEN           = 0.000001;
const double P_INIT             = 0.0;
const double L_INIT             = 0.0;
const double N_INIT             = 0.0;
const double K_INIT             = 0.0;
const double C_INIT             = 0.0;
const int    ITER_LIMIT         = 0;

const double W_INIT             = 0.0;
const double X_INIT             = 0.0;
const double Y_INIT             = 0.0;
const double Z_INIT             = 0.0;

/*  */
const double TIME_LIMIT         = 0.0;
const int    RANDOM_RANGE       = 0;
const int    NULL_SIZE          = 0;

const int    INIT_PURTURB_NUM   = 0;  //prob. size
const int    INIT               = 0;
const double RING_SPACING       = 0.0;

/* Stress Parameters */
const double SA                 = 0.0;
const double SA_REF             = 0.0;
const double MIN_NF             = 0.0;
const double MAX_SD             = 0.0;
const double TNOM               = 0.0;
const double TEMP               = 0.0;

const double TWPE               = 0.0;
const double SC_REF             = 0.0;

/* NMOS */
const double N_u0               = 0.0;
const double N_lkuo             = 0.0;
const double N_xl               = 0.0;
const double N_llodkuo          = 0.0;
const double N_wkuo             = 0.0;
const double N_xw               = 0.0;
const double N_wlod             = 0.0;
const double N_wlodkuo          = 0.0;
const double N_pkvto            = 0.0;
const double N_tkuo             = 0.0;
const double N_kuo              = 0.0;
const double N_kuowe            = 0.0;
const double N_lkvto            = 0.0;
const double N_llodkvto         = 0.0;
const double N_wkvto            = 0.0;
const double N_wlodkvto         = 0.0;
const double N_eta              = 0.0;
const double N_alpha            = 0.0;
const double N_beta             = 0.0;

/* PMOS */
const double P_u0               = 0.0;
const double P_lkuo             = 0.0;
const double P_xl               = 0.0;
const double P_llodkuo          = 0.0;
const double P_wkuo             = 0.0;
const double P_xw               = 0.0;
const double P_wlod             = 0.0;
const double P_wlodkuo          = 0.0;
const double P_kvtho            = 0.0;
const double P_tkuo             = 0.0;
const double P_kuo              = 0.0;
const double P_kuowe            = 0.0;
const double P_lkvto            = 0.0;
const double P_llodkvto         = 0.0;
const double P_wkvto            = 0.0;
const double P_wlodkvto         = 0.0;
const double P_lkvto            = 0.0;
const double P_llodkvto         = 0.0;
const double P_wkvto            = 0.0;
const double P_wlodkvto         = 0.0;
const double P_eta              = 0.0;
const double P_alpha            = 0.0;
const double P_beta             = 0.0;

/* Class Declaration */
class FloorPlanner;

class Block;
class BlockV;
class Pin;
class PinV;
class Net;

class BTree;
class ContourLine;
class ContourMgr;

class RecoverMsg;



#endif  //DEFINE_H