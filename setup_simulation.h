#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <complex>
#include "mps_state.h"

#ifndef SETUP_SIMULATION_H
#define SETUP_SIMULATION_H

class setup_simulation {
public:

	setup_simulation(int n_of_points, int n_of_threads, const std::vector<int> &chain_sizes,
																		 const std::vector<long double> &values_Jz,
												 						 const std::vector<long double> &values_dwa,
																		 const std::vector<long double> &values_T,
																		 const std::vector<long double> &values_V);

     
    
	void create_in_files();
	void run_simulation();
	void get_energies();
   void get_gaps();
	void get_entanglement_spectra( const char* which_state );
	void get_structure_factors();


	Tmps_state get_MPS_from_curr_folder( int );


	std::string loop_return_string( int i0 );
	int loop_return_L( int i0 );
	long double loop_return_Jz( int i0 );
	long double loop_return_dwa( int i0 );
	long double loop_return_T( int i0 );
	long double loop_return_V( int i0 );
	std::string loop_return_TVstr( int i0 );

private:
	int number_of_points;
	std::vector< int > chainL;
	std::vector< long double > parm_Jz;
	std::vector< long double > parm_dwa;
	std::vector< long double > parm_T;	
	std::vector< long double > parm_V;

	int number_of_threads;
};

#endif

void Pref( std:: vector<int> &P, std::string &S );
bool KMP ( std::string &T, std::string &W );
int  KMPi( std::string &T, std::string &W );








