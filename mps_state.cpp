#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mps_matrix.h"
#include "mps_state.h"


// ----------------------------------------------------------------------------------------
Tmps_state::Tmps_state( std::vector<Tmps_matrix> Vmat ) : length( Vmat.size() ) 
{
	for( int i = 0; i<Vmat.size(); ++i)
	{
		_data_.push_back(Vmat[i]);
	}
}
// ========================================================================================

// ----------------------------------------------------------------------------------------
void Tmps_state::load_matrix( Tmps_matrix& M1 )
{
	_data_.push_back(M1);
}
// ========================================================================================

// ----------------------------------------------------------------------------------------
void Tmps_state::set_length( int L)
{
	length = L;
}
// ========================================================================================

// ----------------------------------------------------------------------------------------
long double Tmps_state::overlap_with( const Tmps_state& s1 )
{
	if(s1.length != this->length)
	{
		std::cout<<"ERROR: Different lengths"<<std::endl;
	}

	Tmps_matrix Res;
	Tmps_matrix A0( this->_data_[0] ), B0( s1._data_[0] );
	int p = A0.get_phys_i();
	int l = B0.get_left_i();
	int r = A0.get_left_i();
	Res.set_1matrix( 1 , l , r );
	for( int site = 0; site < s1.length; ++site)
	{
		Tmps_matrix A( this->_data_[site] ), B( s1._data_[site] );
		Tmps_matrix B_dag;
		B_dag = B.conjugate();

		std::cout<<"Printing A"<<std::endl;
		A.write_indices();
		A.write_ith_m_data(0);
		std::cout<<std::endl;
		A.write_ith_m_data(1);
		std::cout<<"Printing B_dag"<<std::endl;
		B_dag.write_indices();
		B_dag.write_ith_m_data(0);
		std::cout<<std::endl;
		B.write_ith_m_data(1);
//		std::cout<<"Printing Res"<<std::endl;
//		Res.write_indices();

		B_dag.multiply_by_matrix( Res );
		Res = B_dag * A;
/*
		std::cout<<"Printing Res"<<std::endl;
		Res.write_indices();
		Res.write_ith_m_data(0);
*/	
		Res.sum_phys_i();
/*
		std::cout<<"Printing Res after summation"<<std::endl;
		Res.write_indices();
		Res.write_ith_m_data(0);	
*/
	}	
	return Res.get_m_data( 0, 0, 0 );
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
Tmps_matrix Tmps_state::calc_reduced_density_matrix( int bipart_l )
{
	if(this->length < bipart_l)
	{
		std::cout<<"ERROR: Impossible bipartition"<<std::endl;
	}

	Tmps_matrix Res;
	Tmps_matrix A0( this->_data_[0] ), B0( this->_data_[0] );
	int p = A0.get_phys_i();
	int l = B0.get_left_i();
	int r = A0.get_left_i();
	Res.set_1matrix( 1 , l , r );
	for( int site = 0; site < bipart_l; ++site)
	{
		Tmps_matrix A( this->_data_[site] ), B( this->_data_[site] );
		Tmps_matrix B_dag;
		B_dag = B.conjugate();

/*
		std::cout<<"Printing A"<<std::endl;
		A.write_indices();
		A.write_ith_m_data(0);
		std::cout<<std::endl;
		A.write_ith_m_data(1);
		std::cout<<"Printing B_dag"<<std::endl;
		B_dag.write_indices();
		B_dag.write_ith_m_data(0);
		std::cout<<std::endl;
		B.write_ith_m_data(1);
*/
//		std::cout<<"Printing Res"<<std::endl;
//		Res.write_indices();

		B_dag.multiply_by_matrix( Res );
		Res = B_dag * A;
/*
		std::cout<<"Printing Res"<<std::endl;
		Res.write_indices();
		Res.write_ith_m_data(0);
*/	
		Res.sum_phys_i();
/*
		std::cout<<"Printing Res after summation"<<std::endl;
		Res.write_indices();
		Res.write_ith_m_data(0);	
*/
	}	
//	Res.write_indices();
	
	return Res;
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::act_with_Sz( int which_site )
{
	Tmps_matrix A(this->_data_[which_site]);
	
	A.multiply_by_double( -0.5, 0 );
	A.multiply_by_double(  0.5, 1 );

	(this->_data_[which_site]) = A;


}
// ----------------------------------------------------------------------------------------















