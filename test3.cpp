#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "setup_simulation.h"
#include "eigenstate_results.h"

#include <unistd.h>



int main(){

	int n_of_threads = 1;
	std::vector<int> chainsizes;
//	for(int i = 0; i < 34; ++i){
//		chainsizes.push_back( 3*i + 15);
//	}
//	chainsizes.push_back(120);
//	chainsizes.push_back(121);
//	chainsizes.push_back(122);
//	chainsizes.push_back(123);
//	chainsizes.push_back(200);
	chainsizes.push_back(120);


	std::vector<long double> values_Jz = {0.15};

	std::vector<long double> values_dwa = {2.0};
	std::vector<long double> values_T = {1.7};
	std::vector<long double> values_V = {0.0};

	setup_simulation sim1(  chainsizes.size()*values_Jz.size()*values_dwa.size()*values_T.size()*values_V.size(), 
									n_of_threads, chainsizes, values_Jz, values_dwa, values_T, values_V ) ;
//	sim1.run_simulation();std::cout<<"simulation done"<<std::endl;
//	sim1.get_energies();std::cout<<"get_energies"<<std::endl;
//	sim1.get_gaps();std::cout<<"get_gaps"<<std::endl;
//	sim1.get_structure_factors(); std::cout<<"get_structure_factors"<<std::endl;
//	sim1.get_entanglement_spectra( "0" );
//	sim1.get_entanglement_spectra( "1" );
 /*
    Tmps_state MPS0 = sim1.get_MPS_from_ith_folder( 0, "0");
    MPS0.project_on_positive_fermionic_parity();
    Tmps_state pMPSp = MPS0;
    Tmps_state MPS1 = sim1.get_MPS_from_ith_folder( 0, "0");
    MPS1.project_on_negative_fermionic_parity();
    Tmps_state pMPSn = MPS1;
 
    for(int i = 119; i > 0; --i)
    {
        Tmps_state MPSp = pMPSp;
        Tmps_state MPSn = pMPSn;
        MPSp.act_with_fermionic_cd( i );
        long double phi1 = MPSp.overlap_with( MPSn );
        
        Tmps_state MPSp1 = pMPSp;
        Tmps_state MPSn1 = pMPSn;
        MPSn1.act_with_fermionic_cd( i );
        long double phi2 = MPSn1.overlap_with( MPSp1 );        
 
        std::cout<<"phi_left_"<<i<<" = "<<phi1+phi2<<"phi_right_"<<i<<" = "<<phi1-phi2<<std::endl;
    }
    
    */
    //Tmps_state MPS1 = MPS0;
//    MPS1.act_with_fermionic_parity(0, 105);
//    std::cout<< MPS0.overlap_with( MPS0 )<<std::endl;
//    std::cout<< MPS1.overlap_with( MPS1 )<<std::endl;

    //MPS1.add_mps_state( MPS0 );
    //std::cout<< MPS1.overlap_with( MPS1 )<<std::endl;
    Tmps_state MPS0 = sim1.get_MPS_from_ith_folder( 0, "0"); 
    std::cout<<"Reduced density matrix spectrum: "<<std::endl;
    MPS0.write_entanglement_spectrum( 60 );
    MPS0.project_on_positive_fermionic_parity();
    std::cout<< MPS0.calc_fermionic_parity( 0, 120)<<std::endl;
    std::cout<<"Reduced density matrix spectrum: "<<std::endl;   
    MPS0.write_entanglement_spectrum( 60 );
    
    Tmps_state MPS1 = sim1.get_MPS_from_ith_folder( 0, "1");

    std::cout<<"Reduced density matrix spectrum: "<<std::endl;
    MPS1.write_entanglement_spectrum( 60 );
    MPS1.project_on_positive_fermionic_parity();
    std::cout<< MPS1.calc_fermionic_parity( 0, 120)<<std::endl;
    std::cout<<"Reduced density matrix spectrum: "<<std::endl;   
    MPS1.write_entanglement_spectrum( 60 );
    
    
//    Tmps_state MPS1 = sim1.get_MPS_from_ith_folder( 0, "1");
//    Tmps_state MPS2 = sim1.get_MPS_from_ith_folder( 0, "2");
//    Tmps_state MPS3 = sim1.get_MPS_from_ith_folder( 0, "3");
 
/*    std::cout<< MPS0.overlap_with( MPS0 )<<std::endl;
    std::cout<< MPS1.overlap_with( MPS1 )<<std::endl;             
    std::cout<< MPS0.overlap_with( MPS1 )<<std::endl;
    std::cout<< MPS0.overlap_with( MPS2 )<<std::endl;
    std::cout<< MPS1.overlap_with( MPS2 )<<std::endl;
*/
 /*   std::cout<<MPS0.calc_fermionic_parity(0, 105)<<std::endl;
    std::cout<<MPS1.calc_fermionic_parity(0, 105)<<std::endl;
    std::cout<<MPS2.calc_fermionic_parity(0, 105)<<std::endl;
    std::cout<<MPS3.calc_fermionic_parity(0, 105)<<std::endl;
*/
	return 0;
	
}

















