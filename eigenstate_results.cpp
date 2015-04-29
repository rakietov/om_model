#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>  
#include <iomanip>
#include "setup_simulation.h"
#include <complex>
#include <cmath>

#include "eigenstate_results.h"


void eigenstate_results::open_file_state( const char* fname, const char* which_state ){
	std::ifstream file;
	file.open( fname );

	std::string str1;
	std::getline( file, str1);
		
	//get the length of the file		
	file.seekg (0, file.end);
	int length = file.tellg();
	file.seekg (0, file.beg);
	
	while( file.tellg() < length-5 ){
		std::string str1, str2("<PARAMETER name=\"L\">"), str3("</PARAMETER>");
		std::getline( file, str1);
		if( KMP(str1, str2) ){
			int f_digit = KMPi(str1, str2) + 19 ; 
			int nu_of_digits = KMPi(str1,str3) - f_digit;
			char buffer[150];
			std::size_t length1 = str1.copy(buffer,nu_of_digits-1,f_digit);
			buffer[length1]='\0';
			std::istringstream isstmp( buffer );
			isstmp >> n_of_sites;
//			std::cout<<buffer;
		}
	}
//	std::cout<<"L = "<<n_of_sites<<std::endl;
	
	file.seekg (0, file.beg);
	while( file.tellg() < length-5 ){
		std::string str1, str2("<EIGENSTATE number=\"");
		str2.append(which_state);
		str2.append("\"");
		std::getline( file, str1);
		if( KMP(str1, str2) ){
			first_digit = file.tellg();
			break;
		}
	}

	while( file.tellg() < length-5 ){
		std::string str1, str2("</EIGENSTATE>");
		std::getline( file, str1);
		if( KMP(str1, str2) ){
			last_digit = file.tellg();
			break;
		}
	}


	//std::cout<<"poczatek: "<<first_digit<<" koniec: "<<last_digit<<std::endl;

/*	file.seekg (last_digit, file.beg);
	std::string str14;
	std::getline( file, str14);
	std::cout<<str14;
*/
	file.close();
}


void eigenstate_results::get_energy(){
	std::ifstream file;

	file.seekg (first_digit, file.beg);
	while( file.tellg() < last_digit ){
			std::string str1, str2("<SCALAR_AVERAGE name=\"Energy\">"), str3("<MEAN>"), str4("</MEAN>");
			std::getline( file, str1);
			if( KMP(str1, str2) ){
				int f_digit = KMPi(str1, str3) + 5 ; 
				//std::cout<<first_digit<<std::endl;
				int nu_of_digits = KMPi(str1,str4) - f_digit;
				char buffer[150];
				std::size_t length1 = str1.copy(buffer,nu_of_digits-1,f_digit);
				buffer[length1]='\0';
				std::istringstream isstmp( buffer );
				isstmp >> std::setprecision(21)>> energy;
				//std::cout<<buffer;
			}
		}
	//std::cout<<std::setprecision(21)<<energy<<std::endl;
	file.close();
}



void eigenstate_results::get_Sz_ij_corr(){

	//std::cout<<std::endl<<"jestem w funckji"<<std::endl;
	std::ifstream file;
	file.open("results.task1.out.xml");

	Sz_ij_corr.resize(n_of_sites);
	for(int i = 0; i < n_of_sites; ++i){
		Sz_ij_corr[i].resize(n_of_sites);
	}

	file.seekg (first_digit, file.beg);


	int corr_begin, corr_end;
	while( file.tellg() < last_digit ){
		std::string str1, str2("<VECTOR_AVERAGE name=\"off-diadonal spin correlations\">");
		std::getline( file, str1);
		
		//std::cout<<str1.c_str()<<std::endl;

		if( KMP(str1, str2) ){
			corr_begin = file.tellg();
			//std::cout<<"poczatek danych z korelacjami w "<<str1<<std::endl;
			break;
		}
	}

	while( file.tellg() < last_digit ){
		std::string str1, str2("</VECTOR_AVERAGE>");
		std::getline( file, str1);
		if( KMP(str1, str2) ){
			corr_end = file.tellg();
			//std::cout<<"koniec danych z korelacjami w "<<str1<<std::endl;
			break;
		}
	}

	file.seekg( corr_begin, file.beg );

	while( file.tellg() < corr_end - 30){
//        <SCALAR_AVERAGE indexvalue="( 118 ) -- ( 71 )"><MEAN>-0.010167762439308383</MEAN></SCALAR_AVERAGE>
//        <SCALAR_AVERAGE indexvalue="( 9 ) -- ( 8 )"><MEAN>-0.16881650624516706</MEAN></SCALAR_AVERAGE>
		std::string str1, str2("=\"( "), str3(" ) -- ( "), str4("<MEAN>"),str5("</MEAN>");
		std::getline( file, str1);
		int f_i_digit = KMPi(str1, str2)  + 3;
		int l_i_digit = KMPi(str1, str3) ;
		int f_j_digit = l_i_digit + 7;
		int l_j_digit = KMPi(str1, str4) - 4;
		int f_v_digit = l_j_digit + 9;
		int l_v_digit = KMPi(str1, str5)-1;

		//std::cout<<f_i_digit<<" "<<l_i_digit<<" "<<f_j_digit<<" "<<l_j_digit<<" "<<f_v_digit<<" "<<l_v_digit<<std::endl;

		char buffer_i[150];
		char buffer_j[150];
		char buffer_v[150];

		std::size_t length_i = str1.copy( buffer_i, l_i_digit - f_i_digit, f_i_digit);
		buffer_i[length_i] = '\0';
		std::size_t length_j = str1.copy( buffer_j, l_j_digit - f_j_digit, f_j_digit);
		buffer_j[length_j] = '\0';
		std::size_t length_v = str1.copy( buffer_v, l_v_digit - f_v_digit, f_v_digit);
		buffer_v[length_v] = '\0';

		//std::cout<<buffer_i<<" "<<buffer_j<< " "<<buffer_v<<std::endl;
	

		std::istringstream iss_i( buffer_i ), iss_j( buffer_j ), iss_v( buffer_v );
		int i, j;
		iss_i >> i;
		iss_j >> j;
		iss_v>>std::setprecision(21)>>Sz_ij_corr[i][j];
	}
	for(int i = 0; i < n_of_sites; ++i){
		Sz_ij_corr[i][i] = 0.25;//works only for spin 1/2
	}
/*
	bad algorithm :)
	for(int i = 0; i < n_of_sites; ++i){
		for(int j = 0; j < n_of_sites; ++j){
			file.seekg (first_digit, file.beg);
//			std::cout << file.tellg() <<" "<< last_digit <<std::endl;
			while( file.tellg() < last_digit ){
					std::string str1, str2("<SCALAR_AVERAGE indexvalue=\"( "), str3("<MEAN>"), str4("</MEAN>");
					std::stringstream ss_i, ss_j;					
					ss_i << i;
					ss_j << j;
					str2.append(ss_i.str() + " ) -- ( " + ss_j.str() + " )\">");
//					std::cout<<str2<<std::endl;
					std::getline( file, str1);
					if( KMP(str1, str2) ){
						int f_digit = KMPi(str1, str3) + 5 ; 
						//std::cout<<first_digit<<std::endl;
						int nu_of_digits = KMPi(str1,str4) - f_digit;
						char buffer[150];
						std::size_t length1 = str1.copy(buffer,nu_of_digits-1,f_digit);
						buffer[length1]='\0';
						std::istringstream isstmp( buffer );
						isstmp >> std::setprecision(21)>> Sz_ij_corr[i][j];

						//std::cout<<str2<<Sz_ij_corr[i][j]<<std::endl;
					}
				}
			if( i ==j ){
				Sz_ij_corr[i][i] = 0.25; //WORKS ONLY FOR S = 1/2
			}
		}
	}
*/
	file.close();
}




void eigenstate_results::write_Sz_ij_corr(){

	for(int i = 0; i < n_of_sites; ++i){
		for(int j = 0; j  < n_of_sites; ++j){
			std::cout<<i<<" -- "<<j<<" "<<std::setprecision(21)<<Sz_ij_corr[i][j]<<std::endl;
		}
	}

}

std::complex<long double> eigenstate_results::calc_structure_factor( long double q, int range ){

	bool acc_q = false;
	for(int i =0 ; i< n_of_sites; ++i){
		if( fabs(q - 2.*i*PI/n_of_sites) < eps )acc_q = true;
	}
	if(!acc_q){
		std::cout<<"ERROR: q is not a reciprocal lattice vector"<<std::endl;
	}


	int n_of_terms = 0;
	std::complex<long double> structure_factor(0., 0.);
	std::complex<long double> imag_unit(0.,1.);
	for(int i = range; i < n_of_sites - range; ++i){
		for(int j = 0; j < n_of_sites; j++){
			//calculate the sum, multiplying by a suitable factor at each step
	 		n_of_terms ++;
			long double abs = i -j;
			if(abs < 0.)abs *= -1.;
			structure_factor += std::exp( imag_unit * q * abs ) * Sz_ij_corr[i][j];
		}
	}
	structure_factor /= n_of_terms;

	//std::cout<<structure_factor<<std::endl;
	return structure_factor;
}







void eigenstate_results::write_structure_factors( int range ){

	std::fstream fs1;
	fs1.open("structure_factors.dat", std::fstream::out);
	for(int i = 0; i< n_of_sites; ++i){

		

		long double q = (2.*PI/n_of_sites)*i;
		std::complex<long double> sf = calc_structure_factor( q, range );
		fs1<<q<<" "<<sf<<std::endl;

		//TODO write the results to a newyl created file
	}



}





