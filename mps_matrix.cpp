#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "matrix_from_txt.h"
#include "mps_matrix.h"

Tmps_matrix::Tmps_matrix() 
{
   m_data = nullptr;
}

Tmps_matrix::~Tmps_matrix() 
{
   if(m_data)delete [] m_data;
}



Tmps_matrix::Tmps_matrix( Tmps_matrix &&M ) : 
		m_data(M.m_data), phys_i( M.phys_i ), left_i( M.left_i ), right_i( M.right_i )
{
	M.m_data = nullptr;
	M.phys_i = 0;
	M.left_i = 0;
	M.right_i = 0;
}



Tmps_matrix::Tmps_matrix( const Tmps_matrix& M ) : 
		m_data( nullptr ), phys_i( M.phys_i ), left_i( M.left_i ), right_i( M.right_i)
{
	auto ite = M.m_data;

	this->set_size(phys_i, left_i, right_i);

	for(int i1 = 0; i1 < phys_i; ++i1){
		for(int i2 = 0; i2 < left_i; ++i2){
			for(int i3 = 0; i3 < right_i; ++i3){
				(*(m_data+i1))[i2][i3] = (*(ite+i1))[i2][i3];
			}
		}
	}
}

void Tmps_matrix::get_matrix( matrix_from_txt& mft ){
	phys_i  = mft.get_phys_i();
	left_i  = mft.get_left_i();
	right_i = mft.get_right_i();

	auto ite= mft.get_Mdata();

	this->set_size(phys_i, left_i, right_i);

	for(int i1 = 0; i1 < phys_i; ++i1){
		for(int i2 = 0; i2 < left_i; ++i2){
			for(int i3 = 0; i3 < right_i; ++i3){
				(*(m_data+i1))[i2][i3] = (*(ite+i1))[i2][i3];
			}
		}
	}

}


void Tmps_matrix::set_size(int p, int l, int r){
	
	delete[] m_data;
	//std::cout<<"m_data deleted"<<std::endl;
	phys_i = p;
	left_i = l;
	right_i= r;
//	std::cout<<" p l r "<<p<<" "<<l<<" "<<r<<std::endl;
	m_data = new std::vector<std::vector<long double> > [p];
//	std::cout<<"set_size - m_data created"<<std::endl;	
	for(int i = 0; i < p; ++i){
		(m_data+i)->resize(l);
		auto it1 = (m_data+i)->begin();
		for(int j = 0; j < l; ++j){
			(it1+j)->resize(r);
		}
	}
	for(int i = 0; i < p; ++i){
		for(int i1 = 0; i1 < (m_data+i)->size(); ++i1){
			auto it0 = (m_data+i)->begin() + i1;
			for(int i2 = 0; i2 < it0->size(); ++i2){
				auto it1 = it0->begin();
				*(it1+i2) = 0.;
			}
		}
	}
	//std::cout<<"size set"<<std::endl;
}

bool Tmps_matrix::is_numerical_Id()
{
    Tmps_matrix M = *this;
	if( M.phys_i != 1 )
	{
		std::cout<<"ERROR: physical index != 1"<<std::endl;
	}
	
	if( M.left_i != M.right_i )
	{
		std::cout<<"ERROR: left_i != right_i"<<std::endl;
	}	
	

    long double sum_off_diag = 0.;
    long double sum_on_diag  = 0.;
    
    for(int a1 = 0; a1 < M.left_i; ++a1)
    {
        for(int a2 = 0; a2 < M.right_i; ++a2)
        {
            if(a1 != a2)
            {
                sum_off_diag += (*M.m_data )[a1][a2] * (*M.m_data )[a1][a2];
            }
            if(a1 == a2)
            {
                sum_on_diag += (*M.m_data )[a1][a2] * (*M.m_data )[a1][a2];
            }           
        }
    }
    bool is_Id = true;
    if(sum_on_diag < 0.9 * M.left_i)is_Id = false; 
    if(sum_off_diag > 0.00001)is_Id = false;  

    return is_Id;
}


void Tmps_matrix::set_ith_m_data( int ph,  std::vector < std::vector<long double> > mat)
{
    if( ph < 0 || ph > phys_i)
    {
        std::cout<<"ERROR in set_ith_m_data"<<std::endl;
    }
    if((m_data+ph)->size() != mat.size())
    {
        std::cout<<"ERROR in set_ith_m_data"<<std::endl;    
    }
    if(((m_data+ph)->begin())->size() != (mat.begin())->size())
    {
        std::cout<<"ERROR in set_ith_m_data"<<std::endl;    
    }
    for(int i1 = 0; i1 < (m_data+ph)->size(); ++i1)
    {
		auto it0 = (m_data+ph)->begin() + i1;
		for(int i2 = 0; i2 < it0->size(); ++i2){
			auto it1 = it0->begin();
			*(it1+i2) = mat[i1][i2];
		}
	}
    
}

void Tmps_matrix::write_indices(){
	std::cout<< "left_i = " << left_i<<std::endl;
	std::cout<< "right_i = " << right_i<<std::endl;
	std::cout<< "phys_i = " << phys_i<<std::endl;
}


void Tmps_matrix::write_ith_m_data(int i){
	for(int i1 = 0; i1 < (m_data+i)->size(); ++i1){
		auto it0 = (m_data+i)->begin() + i1;
		for(int i2 = 0; i2 < it0->size(); ++i2){
			auto it1 = it0->begin();
			std::cout<<*(it1+i2)<<" ";
		}
	std::cout<<std::endl;	
	}
}



void Tmps_matrix::set_1matrix(int p, int l, int r)
{
	if( l != r)
	{
		std::cout<<"ERROR: identrity matrix must have l == r"<<std::endl;
	}

	phys_i = p;
	left_i = l;
	right_i = r;

//		std::cout<<"indices assigned"<<std::endl;		
	this->set_size(phys_i, left_i, right_i);
//		std::cout<<"m_data created"<<std::endl;

	for(int i1 = 0; i1 < phys_i; ++i1){
		for(int i2 = 0; i2 < left_i; ++i2){
			for(int i3 = 0; i3 < right_i; ++i3){
					(*(m_data+i1))[i2][i3] = ((i2 == i3) ? 1. : 0.);
			}
		}
	}
}

Tmps_matrix& Tmps_matrix::operator=( const Tmps_matrix& mpsM1 )
{
	if( this != &mpsM1)
	{
		phys_i = mpsM1.phys_i;
		left_i = mpsM1.left_i;
		right_i = mpsM1.right_i;

//		std::cout<<"indices assigned"<<std::endl;		
		this->set_size(phys_i, left_i, right_i);
//		std::cout<<"m_data created"<<std::endl;

		for(int i1 = 0; i1 < phys_i; ++i1){
			for(int i2 = 0; i2 < left_i; ++i2){
				for(int i3 = 0; i3 < right_i; ++i3){
						(*(m_data+i1))[i2][i3] = (*(mpsM1.m_data+i1))[i2][i3];
				}
			}
		}

	}

	return *this;
}

Tmps_matrix& Tmps_matrix::operator*( const Tmps_matrix& mpsM1){
	
	if(mpsM1.left_i != this->right_i){
		std::cout<<"ERROR: non-matching matrix sizes in multiplication"<<std::endl;
	}	

	Tmps_matrix m_res;
	
	m_res.phys_i = this->phys_i;
	m_res.left_i = this->left_i;
	m_res.right_i = mpsM1.right_i;	
	
	m_res.set_size(this->phys_i, this->left_i, mpsM1.right_i);	
	
	for(int i = 0; i < this->phys_i; ++i){

		for(int a1 = 0; a1 < this->left_i; ++a1){
			for(int a3 = 0; a3 < mpsM1.right_i; ++a3){	

				for(int a2 = 0; a2 < mpsM1.left_i; ++a2){		
				(*(m_res.m_data + i))[a1][a3] +=  ((*(this->m_data + i))[a1][a2]) * ((*(mpsM1.m_data + i))[a2][a3]);
				}

			}
		}

	}
	*this = m_res;
	return *this;
}


Tmps_matrix& Tmps_matrix::multiply_by_matrix( const Tmps_matrix& M)
{	
	if( M.phys_i != 1 )
	{
		std::cout<<"ERROR: multiplying by matrix with physical indices"<<std::endl;
	}
	if(M.left_i != this->right_i){
		std::cout<<"ERROR: non-matching matrix sizes in multiplication"<<std::endl;
	}	

	Tmps_matrix m_res;
	
	m_res.phys_i = this->phys_i;
	m_res.left_i = this->left_i;
	m_res.right_i = M.right_i;	
	
	m_res.set_size(this->phys_i, this->left_i, M.right_i);	
	
	for(int i = 0; i < this->phys_i; ++i){

		for(int a1 = 0; a1 < this->left_i; ++a1){
			for(int a3 = 0; a3 < M.right_i; ++a3){	

				for(int a2 = 0; a2 < M.left_i; ++a2){		
				(*(m_res.m_data + i))[a1][a3] +=  ((*(this->m_data + i))[a1][a2]) * ((*(M.m_data))[a2][a3]);
				}

			}
		}

	}
	*this = m_res;
	return *this;
}


Tmps_matrix& Tmps_matrix::multiply_by_double( long double M, int phys_ind)
{	
	 	
	for(int i = 0; i < this->phys_i; ++i){

		for(int a1 = 0; a1 < this->left_i; ++a1){
			for(int a3 = 0; a3 < this->right_i; ++a3){	
				if( i == phys_ind)
				{
					(*(this->m_data + i))[a1][a3] =  M * (*(this->m_data + i))[a1][a3];
				}
				else
				{
					(*(this->m_data + i))[a1][a3] =  (*(this->m_data + i))[a1][a3];
				}
			}

			
		}

	}

	return *this;
}


Tmps_matrix& Tmps_matrix::conjugate(){

	Tmps_matrix m_res;
	m_res.phys_i = this->phys_i;
	m_res.right_i = this->left_i;
	m_res.left_i = this->right_i;	
	m_res.set_size(m_res.phys_i, m_res.left_i, m_res.right_i);	

	//std::cout<<"size set"<<std::endl;
	for(int i = 0; i < m_res.phys_i; ++i){

		for(int a1 = 0; a1 < m_res.left_i; ++a1){
			for(int a2 = 0; a2 < m_res.right_i; ++a2){	

	
				(*(m_res.m_data + i))[a1][a2] +=  ((*(this->m_data + i))[a2][a1]);


			}
		}

	}
//	std::cout<<"elements copied"<<std::endl;
	*this = m_res;
//	std::cout<<"exchanged"<<std::endl;
	return *this;
}


Tmps_matrix& Tmps_matrix::sum_phys_i(){

	Tmps_matrix m_res;
	m_res.phys_i = 1;
	m_res.left_i = this->left_i;
	m_res.right_i = this->right_i;	
	m_res.set_size(m_res.phys_i, m_res.left_i, m_res.right_i);	

	for(int i = 0; i < this->phys_i; ++i){

		for(int a1 = 0; a1 < m_res.left_i; ++a1){
			for(int a2 = 0; a2 < m_res.right_i; ++a2){	

	
				(*(m_res.m_data ))[a1][a2] +=  ((*(this->m_data + i))[a1][a2]);


			}
		}

	}

	*this = m_res;
	return *this;

}

Tmps_matrix& Tmps_matrix::block_add( const Tmps_matrix& m1, int where_l, int where_r )
{
    if(this->phys_i != m1.phys_i)
    {
        std::cout<<"ERROR non mathing phys_i in block_add"<<std::endl;
    }
	Tmps_matrix m_res;
	m_res.phys_i = this->phys_i;
	m_res.left_i = where_l + m1.left_i;
	m_res.right_i = where_r + m1.right_i;	

	m_res.set_size(m_res.phys_i, m_res.left_i, m_res.right_i);	

	for(int i = 0; i < this->phys_i; ++i){

		for(int a1 = 0; a1 < this->left_i; ++a1){
			for(int a2 = 0; a2 < this->right_i; ++a2){	
				(*(m_res.m_data +i))[a1][a2] =  ((*(this->m_data + i))[a1][a2]);
			}
		}
		
		for(int a1 = where_l; a1 < where_l + m1.left_i; ++a1){
			for(int a2 = where_r; a2 < where_r + m1.right_i; ++a2){	
				(*(m_res.m_data +i))[a1][a2] =  ((*(m1.m_data + i))[a1-where_l][a2-where_r]);
			}
		}
		
	}

	*this = m_res;
	return *this;

}


/*
long double Tmps_matrix::mps_overlap( const Tmps_matrix& M1, const Tmps_matrix& M2 ){

	Tmps_matrix m_res;

	m_res.phys_i = this->phys_i;
	m_res.left_i = this->left_i;
	m_res.right_i = mpsM1.right_i;	



}*/




















