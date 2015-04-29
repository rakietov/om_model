#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <complex>



#ifndef PI
#define PI 3.14159265358979323846

#ifndef eps
#define eps 0.0000000001

#ifndef EIGENSTATE_RESULTS_H
#define EIGENSTATE_RESULTS_H
class eigenstate_results{
public:

	void open_file_state( const char* fname, const char* which_state );

	void get_energy();
	void get_Sz_i_mean();
	void get_Sz_ij_corr();

	void write_Sz_ij_corr();

	std::complex<long double> calc_structure_factor( long double q, int range );
	void write_structure_factors( int range );

private:
// regards the .xml file
	int first_digit;
	int last_digit;
// the number of chain sites
	int n_of_sites;
// physical quantities
	long double energy;	
	std::vector<long double> Sz_i_mean;
	std::vector<std::vector<long double> > Sz_ij_corr;

};

#endif

bool KMP ( std::string &T, std::string &W );
int  KMPi( std::string &T, std::string &W );













/*class get_measurements{
public:

//	void calculate_structure_factor( long double q );


//	long double get_energy( int which_state);
//	long double get_ij_corr( int i, int j, int which_state );

	localizeEIGENSTATE( int which_state );


private:
// data regardining results
	int n_of_sites;
	int n_of_states;
};
*/




#endif
#endif










