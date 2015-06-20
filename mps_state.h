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
    void normalize();
	long double calc_fermionic_parity( int first_site, int last_site );
	Tmps_matrix calc_reduced_density_matrix( int bipart_l );

    void write_entanglement_spectrum( int bipart_l );

    void compress( long double cutoff_singular_value );

    void multiply_by_double( int which_site, long double doubl );
	void act_with_Sz( int which_site );
    void act_with_Sx( int which_site );
    void act_with_sigma_plus( int which_site );
    void act_with_sigma_minus( int which_site );
	void act_with_fermionic_parity( int first_site, int last_site );
    void project_on_positive_fermionic_parity();
    void project_on_negative_fermionic_parity();

    void act_with_fermionic_cd( int which_site );
    void act_with_fermionic_c( int which_site );

    void add_mps_state( Tmps_state& s1 );    

	void load_matrix( Tmps_matrix& M1);
	void set_length( int L );
    int get_length(){ return length;};

    bool is_right_normalized();

private:
	std::size_t length;
	std::vector< Tmps_matrix > _data_;
};














#endif












