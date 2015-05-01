#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "mps_matrix.h"

#ifndef MPS_STATE_H
#define MPS_STATE_H



class Tmps_state{
public:

/*
	Tmps_matrix();
	Tmps_matrix( const Tmps_matrix& M );
	~Tmps_matrix();

	Tmps_matrix( Tmps_matrix&& M);
	
		Tmps_matrix& operator=( const Tmps_matrix& mpsM1 );

*/
    Tmps_state(){ length = 0; };
   	Tmps_state( std::vector<Tmps_matrix> Vmat );
    ~Tmps_state(){ };


	long double overlap_with( const Tmps_state& s1);
	long double calc_fermionic_parity( int first_site, int last_site );
	Tmps_matrix calc_reduced_density_matrix( int bipart_l );

    void multiply_by_double( int which_site, long double doubl );
	void act_with_Sz( int which_site );


	void load_matrix( Tmps_matrix& M1);
	void set_length( int L );



private:
	std::size_t length;
	std::vector< Tmps_matrix > _data_;
};














#endif












