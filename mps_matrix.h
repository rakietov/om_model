#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "matrix_from_txt.h"


#ifndef TMPS_MATRIX_H
#define TMPS_MATRIX_H


class Tmps_matrix{
public:

	Tmps_matrix();
	Tmps_matrix( const Tmps_matrix& M );
	~Tmps_matrix();

	Tmps_matrix( Tmps_matrix&& M);


	void get_matrix( matrix_from_txt& mft );
	void set_size(int p, int l, int r);
    void set_ith_m_data( int p,  std::vector < std::vector<long double> > mat);

	void write_indices();
	void write_ith_m_data(int i);

	int get_phys_i(){ return phys_i; };
	int get_left_i(){ return left_i; };
	int get_right_i(){ return right_i; };
	long double get_m_data(int p, int l, int r){ return (this->m_data)[p][l][r];};
    std::vector < std::vector<long double> > get_ith_m_data( int p ){ return (this->m_data)[p];};

	//bool is_left_normalized();
	Tmps_matrix& conjugate( );
	Tmps_matrix& sum_phys_i();
	Tmps_matrix& block_add( const Tmps_matrix& m1, int where_l, int where_r );
	//TString& operator=( const TString& s ); // operator przypisania kopiuj¹cy
	Tmps_matrix& operator=( const Tmps_matrix& mpsM1 );

	Tmps_matrix& operator*( const Tmps_matrix& mpsM1 );
	Tmps_matrix& multiply_by_double( long double M, int phys_i);
	Tmps_matrix& multiply_by_matrix( const Tmps_matrix& M);

	Tmps_matrix multiply_by_lambda_from_left( const std::vector < std::vector<long double> > lambda );
	Tmps_matrix multiply_by_lambda_from_right( const std::vector < std::vector<long double> > lambda );

	void set_1matrix( int p, int l, int r );
	//long double mps_overlap( const Tmps_matrix& M1, const Tmps_matrix& M2 );	

private:

	int phys_i, left_i, right_i;
	std:: vector < std::vector<long double> >* m_data = nullptr;

};


#endif




















