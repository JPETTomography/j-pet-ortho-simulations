#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "TMath.h"
#include <iostream>

const long double fine_structure_const_ = 0.0072973525664L; //(17)
const long double h_bar_SI = 1.054571800L*TMath::Power(10, -34);// [J*s]
const long double h_bar_eV = 6.582119514L*TMath::Power(10, -16);// [eV*s]
const long double light_speed_SI = 299792458L; // [m/s]
const long double e_mass_SI = 9.10938356L * TMath::Power(10, -31);// [kg]
const long double e_mass_MeV = 0.5109989461L; // (13) [MeV/c^2]
const long double r_Compton_SI= h_bar_eV*light_speed_SI/(e_mass_MeV*1000000);

///
/// \brief PrintConstants Prints to the standard output all physics constants defined in constants.h
///
inline void PrintConstants()
{
    std::cout<<"\n******    CONSTANTS    ******"<<std::endl;
    std::cout<<"* FINE STRUCTURE: "<<fine_structure_const_<<std::endl;
    std::cout<<"* H BAR: "<<h_bar_SI<<" [J*s]"<<std::endl;
    std::cout<<"* H_BAR: "<<h_bar_eV<<" [eV*s]"<<std::endl;
    std::cout<<"* C (SI): "<<light_speed_SI<<" [m/s]"<<std::endl;
    std::cout<<"* ELECTRON MASS: "<<e_mass_SI<<" [kg]"<<std::endl;
    std::cout<<"* ELECTRON MASS: "<<e_mass_MeV<<" [MeV/c^2]"<<std::endl;
    std::cout<<"* COMPTON R: "<<r_Compton_SI<<" [m]"<<std::endl;
    std::cout<<"******************************\n"<<std::endl;
}

#endif // CONSTANTS_H
