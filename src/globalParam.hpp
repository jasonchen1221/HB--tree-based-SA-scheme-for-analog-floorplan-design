#ifndef GLOBALPARAM_HPP
#define GLOBALPARAM_HPP

#include <string>
#include <map>

/* Simulated Annealing Algo inital value */
const double T_INIT             = 100000.0;
const double T_FROZEN           = 0.000001;
const double P_INIT             = 0.987;
const double L_INIT             = 0.0;
const double N_INIT             = 0.0;
const double K_INIT             = 7;
const double C_INIT             = 100;
const int    ITER_LIMIT         = 0;

const double W_INIT             = 0.0;
const double X_INIT             = 0.0;
const double Y_INIT             = 0.0;
const double Z_INIT             = 0.0;

/*  */
const double TIME_LIMIT         = 0.0;
const int    RANDOM_RANGE       = 0;
const int    NULL_SIZE          = 0;

const int    INIT_PURTURB_NUM   = 1000;  //prob. size
const int    INIT               = 50;
const double RING_SPACING       = 0.0;

/* Stress Parameters */
const double SA                 = 0.1750;   // left diffusion length
const double SA_REF             = 0.405;    // left diffusion length from PDK
const double MIN_FW             = 0.12;     // minumum # transistor_width/NF
const double MAX_SD             = 0.32;      // maxinum poly-to-poly spacing
const double TNOM               = 25.0;
const double TEMP               = 25.0;

const double TWPE               = 0.8;
const double SC_REF             = 1.0;

/* NMOS */
const double N_u0               = 0.05282;
const double N_lkuo             = 2.2 * 1.0e-07;
const double N_xl               = -1.46 * 1.0e-08;
const double N_llodkuo          = 1.0;
const double N_wkuo             = 1.2 * 1.0e-07;
const double N_xw               = 1.6e-08;
const double N_wlod             = 0.0;
const double N_wlodkuo          = 1.0;
const double N_pkvtho            = 0.0;
const double N_tkuo             = 0.0;
const double N_kuo              = -2.9e-08;
const double N_kuowe            = -0.0022;
const double N_lkvtho            = -0.4e-8;
const double N_llodkvtho         = 1.0;
const double N_wkvto            = 5e-8;
const double N_wlodkvtho         = 1.0;
const double N_eta              = 1.03;
const double N_alpha            = 0.076;
const double N_beta             = 0.48;

/* PMOS */
const double P_u0               = 0.0165;
const double P_lkuo             = -4.005 * 1.0e-08 ;
const double P_xl               = -1.09 * 1.0e-08;
const double P_llodkuo          = 1.0;
const double P_wkuo             = 0.0;
const double P_xw               = 1.6 * 1.0e-08;
const double P_wlod             = 0.0;
const double P_wlodkuo          = 1.0;
const double P_pkvtho            = -1.00 * 1e-17;
const double P_tkuo             = 0.0;
const double P_kuo              = -5.354 * 1e-10;
const double P_kuowe            = -0.0127;
const double P_lkvtho            = -4.33e-8;
const double P_llodkvtho         = 1.0;
const double P_wkvto            = 0.0;
const double P_wlodkvtho         = 1.0;
const double P_eta              = 0.49;
const double P_alpha            = 0.48;
const double P_beta             = 0.59;

/* Constraint Types */
enum E_ConstraintType {UNDEFINED_TYPE, SYMMETRY, SELF};
/* Device Types */
enum E_DeviceType {UNKNOWN_TYPE, NMOS, PMOS, RESISTOR, CAPACITOR};

/* Class Declaration */
class Placer;

class Block;
//class BlockV;
class Pin;
//class PinV;
class Net;

class BTree;
class ContourLine;
class ContourMgr;

class RecoverMsg;


struct LDEParam{
    double          SD = 0;
    double          STIWL = 0;
    double          Y2 = 0;
    double          LDE_SA = SA;
    double          LDE_SB = SA;
    E_DeviceType    eDeviceType = UNKNOWN_TYPE;
    bool            isBodyDetached = false;
    bool            BL = false;
    bool            BR = false;
    bool            BT = false;
    bool            BB = false;
    double          WL, WR, WT, WB;
    
    std::map<std::string, double> STIW;
    
    double          MEAN;
    double          DEVIATION;
    int             DUM = 0;
    std::string     CHECK = "";
};

struct Constraint{
    int              priority = 0;
    std::string      symAxisName = "";
    E_ConstraintType eConstraintType;
};

struct Contour{
    double   x, y; 
    double   w, h; 
    double   max;
    Contour* BACK = nullptr;
};



#endif  //GLOBALPARAM_HPP