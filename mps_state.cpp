#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mps_matrix.h"
#include "mps_state.h"
#include <cmath>
#include <algorithm>

#include </home/sierant/Eigen324/Eigen/Dense>

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

/*		std::cout<<"Printing A"<<std::endl;
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
void Tmps_state::add_mps_state( Tmps_state& s1 )    
{
    if( this->length != s1.length )
    {
        std::cout<<"ERROR non-matching length in add_mps_state"<<std::endl;
    }    
/*
    std::cout<<"A matrix at 0:"<<std::endl;
    (this->_data_[0]).write_indices();
    (this->_data_[0]).write_ith_m_data( 0 );
    (this->_data_[0]).write_ith_m_data( 1 );  
    std::cout<<"B matrix at 0:"<<std::endl;
    (s1._data_[0]).write_indices();
    (s1._data_[0]).write_ith_m_data( 0 );
    (s1._data_[0]).write_ith_m_data( 1 );   
 */
    (this->_data_[0]).block_add( s1._data_[0], 0, (this->_data_[0]).get_right_i() );
/*
    std::cout<<"After addition  at 0:"<<std::endl;
    (this->_data_[0]).write_indices();
    (this->_data_[0]).write_ith_m_data( 0 );
    (this->_data_[0]).write_ith_m_data( 1 ); 
  */  
    for( int i = 1; i < this->length - 1; ++i)
    {
/*
         std::cout<<"A matrix at "<<i<<std::endl;
        (this->_data_[i]).write_indices();
        (this->_data_[i]).write_ith_m_data( 0 );
        (this->_data_[i]).write_ith_m_data( 1 );  
        std::cout<<"B matrix at "<<i<<std::endl;
        (s1._data_[i]).write_indices();
        (s1._data_[i]).write_ith_m_data( 0 );
        (s1._data_[i]).write_ith_m_data( 1 ); 
*/
        (this->_data_[i]).block_add( s1._data_[i], (this->_data_[i]).get_left_i(), (this->_data_[i]).get_right_i() );             
/*
    std::cout<<"After addition  at "<<i<<std::endl;
    (this->_data_[i]).write_indices();
    (this->_data_[i]).write_ith_m_data( 0 );
    (this->_data_[i]).write_ith_m_data( 1 ); 
*/       
    } 
/*
    std::cout<<"A matrix at l-1:"<<std::endl;
    (this->_data_[this->length-1]).write_indices();
    (this->_data_[this->length-1]).write_ith_m_data( 0 );
    (this->_data_[this->length-1]).write_ith_m_data( 1 );  
    std::cout<<"B matrix at l-1:"<<std::endl;
    (s1._data_[this->length-1]).write_indices();
    (s1._data_[this->length-1]).write_ith_m_data( 0 );
    (s1._data_[this->length-1]).write_ith_m_data( 1 );  
*/
    (this->_data_[this->length-1]).block_add( s1._data_[this->length-1], 
                                    (this->_data_[this->length-1]).get_left_i(), 0 );
/*
    std::cout<<"After addition  at l-1:"<<std::endl;
    (this->_data_[this->length-1]).write_indices();
    (this->_data_[this->length-1]).write_ith_m_data( 0 );
    (this->_data_[this->length-1]).write_ith_m_data( 1 ); 
*/
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
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::act_with_Sx( int which_site )
{
	Tmps_matrix A(this->_data_[which_site]);
	
	A.multiply_by_double(  0.5, 0 );
	A.multiply_by_double(  0.5, 1 );
//    std::cout<<"A matrix\n phys 0\n";   
//    A.write_ith_m_data( 0 );
//    std::cout<<"phys 1\n";
//    A.write_ith_m_data( 1 );   
   
    Tmps_matrix B( A );
    B.set_ith_m_data(0, A.get_ith_m_data(1) );
    B.set_ith_m_data(1, A.get_ith_m_data(0) );
//    std::cout<<"B matrix\n phys 0\n";  
//    B.write_ith_m_data( 0 );
//        std::cout<<"phys 1\n";
//    B.write_ith_m_data( 1 );   

	(this->_data_[which_site]) = B;

}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::act_with_sigma_plus( int which_site )
{
     
	Tmps_matrix A(this->_data_[which_site]);
	
	A.multiply_by_double(  2.0, 0 );
 /*
    std::cout<<"A matrix\n phys 0\n";   
    A.write_ith_m_data( 0 );
    std::cout<<"phys 1\n";
    A.write_ith_m_data( 1 );   
    A.write_indices();
*/
    Tmps_matrix B;
    B.set_size( A.get_phys_i(), A.get_left_i(), A.get_right_i() );
    B.set_ith_m_data(1, A.get_ith_m_data(0) );
/* 
    std::cout<<"B matrix\n phys 0\n";  
    B.write_ith_m_data( 0 );
        std::cout<<"phys 1\n";
    B.write_ith_m_data( 1 );  
    B.write_indices(); 
    std::cout<<"acted with sigma plus"<<std::endl;
*/
	(this->_data_[which_site]) = B;
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::act_with_sigma_minus( int which_site )
{
	Tmps_matrix A(this->_data_[which_site]);
	
	A.multiply_by_double(  2.0, 1 );
/* 
    std::cout<<"A matrix\n phys 0\n";   
    A.write_ith_m_data( 0 );
    std::cout<<"phys 1\n";
    A.write_ith_m_data( 1 );   
    A.write_indices();
*/
    Tmps_matrix B;
    B.set_size( A.get_phys_i(), A.get_left_i(), A.get_right_i() );
    B.set_ith_m_data(0, A.get_ith_m_data(1) );
/* 
    std::cout<<"B matrix\n phys 0\n";  
    B.write_ith_m_data( 0 );
        std::cout<<"phys 1\n";
    B.write_ith_m_data( 1 );  
    B.write_indices(); 
    std::cout<<"acted with sigma plus"<<std::endl;
*/
	(this->_data_[which_site]) = B;
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::act_with_fermionic_c( int which_site )
{
//fermionic c_j operator defined as sigma_minus * fermionic_parity(0, j)
    this->act_with_fermionic_parity( 0, which_site );
    this->act_with_sigma_minus( which_site );
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::act_with_fermionic_cd( int which_site )
{
//fermionic c_j^dag operator defined as sigma_plus * fermionic_parity(0, j)
    this->act_with_fermionic_parity( 0, which_site );
    this->act_with_sigma_plus( which_site );
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::multiply_by_double( int which_site, long double doubl )
{
	Tmps_matrix A(this->_data_[which_site]);
	
	A.multiply_by_double( doubl, 0 );
	A.multiply_by_double( doubl, 1 );

	(this->_data_[which_site]) = A;
	
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
long double Tmps_state::calc_fermionic_parity( int first_site, int last_site )
{
    Tmps_state MPSleft = *this;
    Tmps_state MPSright = *this;
    
    MPSright.act_with_fermionic_parity( first_site, last_site );
    
    //std::cout<<MPSright.overlap_with(MPSright)<<std::endl;
    return MPSleft.overlap_with( MPSright );
}
// ========================================================================================

void Tmps_state::act_with_fermionic_parity( int first_site, int last_site )
{
    for(int i = first_site; i < last_site; ++i)
    {  
        this->act_with_Sz( i );
        this->multiply_by_double( i, -2.0 );
    }	
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::project_on_positive_fermionic_parity()
{
    Tmps_state A, B;
    A = *this;
    B = *this;
    long double ferm_par = A.calc_fermionic_parity( 0, A.length );
//    std::cout<<"Fermionic parity of A is "<<ferm_par<<std::endl;
    if( fabs( ferm_par ) > 0.99 )
    {
        std::cout<<"Warning: projecting state with well defined fermionic parity"<<std::endl;
    }

    B.act_with_fermionic_parity( 0, B.length );
    A.add_mps_state( B );
    A.multiply_by_double( 0, 0.5 );

    A.normalize();

    *this = A;
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void Tmps_state::project_on_negative_fermionic_parity()
{
    Tmps_state A, B;
    A = *this;
    B = *this;
    long double ferm_par = A.calc_fermionic_parity( 0, A.length );
//    std::cout<<"Fermionic parity of A is "<<ferm_par<<std::endl;
    if( fabs( ferm_par ) > 0.99 )
    {
        std::cout<<"Warning: projecting state with well defined fermionic parity"<<std::endl;
    }

    B.act_with_fermionic_parity( 0, B.length );
    B.multiply_by_double( 0, -1.0);
    A.add_mps_state( B );
    A.multiply_by_double( 0, 0.5 );
    
    A.normalize();

    *this = A;
}
// ========================================================================================



// ----------------------------------------------------------------------------------------
void Tmps_state::normalize()
{
    long double norm_sq = this->overlap_with(*this);
    
    this->multiply_by_double(0, 1/sqrt(norm_sq));
}
// ========================================================================================

// ----------------------------------------------------------------------------------------
void Tmps_state::write_entanglement_spectrum( int bipart_l )
{
	Tmps_matrix reduced_dm;
	reduced_dm = this->calc_reduced_density_matrix( bipart_l );
	
	//reduced_dm.write_indices();
	//reduced_dm.write_ith_m_data(0);

	Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic> A;
	A.resize(reduced_dm.get_left_i(), reduced_dm.get_right_i());
	for(int il = 0; il < reduced_dm.get_left_i(); ++il)
	{
		for(int ir = 0; ir < reduced_dm.get_right_i(); ++ir)
		{
		A(il, ir) = reduced_dm.get_m_data(0, il, ir);
		}
	}
	//std::cout << "Here is the matrix A:\n" << A << std::endl;
	Eigen::SelfAdjointEigenSolver< Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic> > eigensolver(A);
	//std::cout<<"po diagonalizacji"<<std::endl;
	//if (eigensolver.info() != Success) abort();
	//std::cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << std::endl;
	//std::cout << "Here's a matrix whose columns are eigenvectors of A \n"
	//<< "corresponding to these eigenvalues:\n"
	//<< eigensolver.eigenvectors() << std::endl;
	// calculate spectrum and write it to file
	
	//fs1<< eigensolver.eigenvalues() << std::endl;

    std::cout << eigensolver.eigenvalues() <<std::endl;
}
// ========================================================================================



