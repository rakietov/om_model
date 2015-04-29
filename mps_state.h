#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "mps_matrix.h"

#ifndef MPS_STATE_H
#define MPS_STATE_H



class Tmps_state{
public:
	Tmps_state( std::vector<Tmps_matrix> Vmat );

	long double overlap_with( const Tmps_state& s1);
	Tmps_matrix calc_reduced_density_matrix( int bipart_l );

	void act_with_Sz( int which_site );

	void load_matrix( Tmps_matrix& M1);
	void set_length( int L );



private:
	std::size_t length;
	std::vector< Tmps_matrix > _data_;
};














#endif












