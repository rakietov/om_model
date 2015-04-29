#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "setup_simulation.h"
#include "eigenstate_results.h"

#include <unistd.h>



int main(){
	
	//int noL = 3;
	int n_of_threads = 6;
	std::vector<int> chainsizes;
//	for(int i = 0; i < 34; ++i){
//		chainsizes.push_back( 3*i + 15);
//	}
//	chainsizes.push_back(120);
//	chainsizes.push_back(121);
//	chainsizes.push_back(122);
//	chainsizes.push_back(123);
	chainsizes.push_back(200);
	chainsizes.push_back(240);


	std::vector<long double> values_Jz = {0.15};

	std::vector<long double> values_dwa = {2.0};
	std::vector<long double> values_T = {1.3, 1.5, 1.7, 1.9, 2.1, 2.3, 2.5, 2.7,2.9, 3.1, 3.3, 3.5};
	std::vector<long double> values_V = {0.0};

	setup_simulation sim1(  chainsizes.size()*values_Jz.size()*values_dwa.size()*values_T.size()*values_V.size(), 
									n_of_threads, chainsizes, values_Jz, values_dwa, values_T, values_V ) ;
	sim1.run_simulation();std::cout<<"simulation done"<<std::endl;
	sim1.get_energies();std::cout<<"get_energies"<<std::endl;
	sim1.get_gaps();std::cout<<"get_gaps"<<std::endl;
	sim1.get_structure_factors(); std::cout<<"get_structure_factors"<<std::endl;
	sim1.get_entanglement_spectra( "0" );
	sim1.get_entanglement_spectra( "1" );

//	std::string T("alamakotainiemapsadrugiegopsa");
//	std::string W("psa");

	//KMP(T,W);
/*std::fstream fs;
  fs.open ("plik_utworzony.txt", std::fstream::out );

  fs << " more lorem ipsum";

  fs.close();*/

/*	
	std::string str1 = "mkdir";
	std::string str2 = " dupa1";

	int i = 12;
	
	
	int a = 10;
	std::stringstream ss;
	ss << a;
	std::string str = ss.str();
	
	std::cout<<str;

	str1.append(str2);

	std::system(str1.c_str());
*/
	return 0;
	
}

















