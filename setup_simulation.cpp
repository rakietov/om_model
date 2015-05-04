#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include<time.h>
#include <math.h> 
#include </home/jumper/Eigen324/Eigen/Dense>



#include "setup_simulation.h"
#include "eigenstate_results.h"
#include "matrix_from_txt.h" 




setup_simulation::setup_simulation(int n_of_points, int n_of_threads, const std::vector<int> &chain_sizes,
												 const std::vector<long double> &values_Jz,
												 const std::vector<long double> &values_dwa,
												 const std::vector<long double> &values_T,
												 const std::vector<long double> &values_V){
	number_of_points = n_of_points;
	number_of_threads = n_of_threads;
	for( auto L : chain_sizes ){
			//std::cout<<L<<std::endl;	
			chainL.push_back(L);
	} 
	for( auto Jz : values_Jz ){
			//std::cout<<L<<std::endl;	
			parm_Jz.push_back(Jz);
	} 
	for( auto dwa : values_dwa ){
			//std::cout<<L<<std::endl;	
			parm_dwa.push_back(dwa);
	} 
	for( auto T : values_T ){
			//std::cout<<L<<std::endl;	
			parm_T.push_back(T);
	} 
	for( auto V : values_V ){
			//std::cout<<L<<std::endl;	
			parm_V.push_back(V);
	} 
	create_in_files();
}



void setup_simulation::create_in_files(){

	for( int i = 0; i < number_of_points; ++i){
		// create folders and input files
		std::string mkdir("mkdir " + loop_return_string( i ) );
		std::string folder_dir("./" + loop_return_string( i ) + "/input.py");
		system( mkdir.c_str() );
	
		std::fstream fs1, fs2;
		//std::cout << folder_dir.c_str() << std::endl;

	
		fs1.open( folder_dir.c_str(), std::fstream::out);
		fs2.open( "input_file.py" , std::fstream::in);

		if(fs1.is_open())std::cout<<"Created "<< folder_dir<< std::endl;
		//set values of parameters		
		for( auto i = 0; i < 10; ++i){
			std::string str1;
			std::getline( fs2, str1);
			fs1 << (str1.append("\n"));
		}
		std::string str2,str3;
		std::getline( fs2, str2 );
		str3.append("'L': ");
		std::stringstream ss1 ;
		ss1 << loop_return_L( i );
		str3.append( ss1.str() );
		str3.append(",\n");
		fs1 << str3;

		std::string str4,str5;
		std::getline( fs2, str4 );
		str5.append("'Jz': ");
		std::stringstream ss2 ;
		ss2 << loop_return_Jz( i );
		str5.append( ss2.str() );
		str5.append(",\n");
		fs1 << str5;

		std::string str6,str7;
		std::getline( fs2, str6 );
		str7.append("'dwa': ");
		std::stringstream ss3 ;
		ss3 << loop_return_dwa( i );
		str7.append( ss3.str() );
		str7.append(",\n");
		fs1 << str7;

		std::string str8,str9;
		std::getline( fs2, str8 );
		str9.append("'T': ");
		std::stringstream ss4;
		ss4 << loop_return_T( i );
		str9.append( ss4.str() );
		str9.append(",\n");
		fs1 << str9;

		std::string str10,str11;
		std::getline( fs2, str10 );
		str11.append("'V': ");
		std::stringstream ss5 ;
		ss5 << loop_return_V( i );
		str11.append( ss5.str() );
		str11.append(",\n");
		fs1 << str11; //write_V( , loop_return_V(i) );

		for( auto i = 0; i < 16; ++i){
			std::string str1;
			std::getline( fs2, str1);
			fs1 << (str1.append("\n"));
		}


		fs1.close();
		fs2.close();		
	}
}
/*
void setup_simulation::do_mps_optim(){
	system("python input.py > out.txt");
}*/

void setup_simulation::run_simulation(){

	int proc = 0;

	std::fstream fs1;
	fs1.open("simulation_status.txt", std::fstream::out);

	while( proc < number_of_points ){
		std::vector<std::thread> threads;
		for(int i =0 ; i<number_of_threads; ++i){
			fs1 << "Starting: "<<loop_return_string( proc )<<std::endl;
			std::string curr_dir =  "./"+loop_return_string( proc)+"/";
			proc ++;
			chdir( curr_dir.c_str() );
			system("pwd");
			threads.push_back( std::thread( system,"python input.py > out.txt" ) );
			sleep(1);// sleep in order to start simulation before chdir ("..")
			chdir( ".." );
		}
		for(auto &t : threads){
			t.join();
		}


	}
	fs1.close();

}


void setup_simulation::get_energies(){
	
	std::fstream fs1, fs2;

	
	for( int i =0 ; i< number_of_points; ++i ){
		if(i == 0)
		{
			fs1.open("energies"+loop_return_TVstr( i )+".dat", std::fstream::out);
		}
		else
		{
		fs1.open("energies"+loop_return_TVstr( i )+".dat", std::fstream::out | std::fstream::app);
		}
		fs1.seekg(0,fs1.end);	
		std::string curr_dir =  "./"+loop_return_string(i)+"/";
		chdir( curr_dir.c_str() );
		
		fs2.open( "results.task1.out.xml" , std::fstream::in);

		//get the length of the file		
		fs2.seekg (0, fs2.end);
		int length = fs2.tellg();
		fs2.seekg (0, fs2.beg);

		while( fs2.tellg() < length-5 ){
			std::string str1, str2("<SCALAR_AVERAGE name=\"Energy\">"), str3("<MEAN>"), str4("</MEAN>");
			std::getline( fs2, str1);
			if( KMP(str1, str2) ){
				int first_digit = KMPi(str1, str3) + 5 ; 
				//std::cout<<first_digit<<std::endl;
				int number_of_digits = KMPi(str1,str4) - first_digit;
				char buffer[150];
				std::size_t length1 = str1.copy(buffer,number_of_digits-1,first_digit);
				buffer[length1]='\0';
				fs1 << buffer<<"\n"; 
			}

		}

		fs2.close();
		chdir( ".." );

		fs1.close();
	}
}

void setup_simulation::get_gaps()
{
	for(int i1 = 0; i1<parm_T.size();++i1){
		for(int i2 =0 ; i2<parm_V.size();++i2){
			std::fstream fs1, fs2, fs3, fs4;
			std::string ene, gap1, gap2, gap3, strT,strV;
		
			std::cout<<"i1 "<<i1<<" i2 "<<i2<<std::endl;		
	
			std::stringstream ssT,ssV;
			ssT<<parm_T[i1];
			ssV<<parm_V[i2];
			strT = "_T_"+ssT.str();
			strV = "_V_"+ssV.str(); 

			ene = "energies"+strT+strV+".dat";
			gap1 = "gap1"+strT+strV+".dat";
			gap2 = "gap2"+strT+strV+".dat";
			gap3 = "gap3"+strT+strV+".dat";

			std::cout<<ene<<" "<<gap1<<" "<<gap2<<" "<<gap3<<std::endl;

			fs1.open(ene.c_str(), std::fstream::in);	
			fs2.open(gap1.c_str(),std::fstream::out);
			fs3.open(gap2.c_str(),std::fstream::out);	
			fs4.open(gap3.c_str(),std::fstream::out);	
	
			fs1.seekg (0, fs1.end);
			int length = fs1.tellg();
			fs1.seekg (0, fs1.beg);
			int i = 0;
			std::vector<long double> energies;
			while( fs1.tellg() < length-2 )
			{
				std::string str1, str2;
				long double en;
				fs1 >> en;
				energies.push_back(en);
			}
			std::cout<<"loaded "<<energies.size()<<"energies"<<std::endl;
			for(int i=0;i<energies.size()-1;++i){
				long double gap;
				gap = energies[i+1]-energies[i];
				if( i%4 == 0 ){
					fs2 << loop_return_L( (i)/4 * number_of_points / chainL.size() )<<" "<<gap<<std::endl;
				}
				if( i%4 == 1 ){
					fs3 << loop_return_L( (i)/4 * number_of_points / chainL.size()) <<" "<<gap<<std::endl;
				}
				if( i%4 == 2 ){
					fs4 << loop_return_L( (i)/4 * number_of_points / chainL.size()) <<" "<<gap<<std::endl;
				}
			}

			fs4.close();
			fs3.close();
			fs2.close();
			fs1.close();
		}
	}

}





void setup_simulation::get_structure_factors()
{
	for( int i =0 ; i< number_of_points; ++i ){
		std::fstream fs1;
		std::string curr_dir =  "./"+loop_return_string(i)+"/";


		fs1.open("str_f"+loop_return_string(i) +".dat", std::fstream::out);	
		chdir( curr_dir.c_str() );		


		eigenstate_results er1;
		er1.open_file_state( "results.task1.out.xml" , "0" );
		er1.get_Sz_ij_corr();

		
		int range = 0;
		for(int j = 0; j < loop_return_L( i ); ++j)
		{
			long double q = (2.*PI/loop_return_L( i ))*j;
			//std::cout<<q<<std::endl;
			std::complex<long double> sf = er1.calc_structure_factor( q, range );

			//std::cout<<sf<<std::endl;
			fs1 << q <<" "<<sf<<std::endl;

		}			

		chdir( ".." );
		fs1.close();
	}





}

// ----------------------------------------------------------------------------------------
Tmps_state setup_simulation::get_MPS_from_ith_folder( int i, const char* which_state)
{
	std::string curr_dir =  "./"+loop_return_string(i)+"/";	
    chdir( curr_dir.c_str() );
    
    std::string str1("results.task1.out.");
	str1.append(which_state);
	str1.append(".chkp");
		
	chdir ( str1.c_str() );
	sleep(0.2);
    
	for(int i0 = 0; i0 < loop_return_L( i ); ++i0)
	{
		std::string s1("~/h5dump mps");
		std::stringstream ss1;
		ss1 << i0;
		s1.append(ss1.str());
		s1.append(".h5 > mps");
		s1.append(ss1.str());
		s1.append(".txt");
		system(s1.c_str());
	//	system( "pwd" );
	//	std::cout<<"executing: "<<s1.c_str()<<std::endl;
	}


	matrix_from_txt mft;
	std::vector <Tmps_matrix> Vmps_matrix;
	for(int i0 = 0; i0 < loop_return_L( i ); ++i0){
		std::stringstream ss;	
		ss << i0;
		std::string s1("./mps");
		std::string s2(ss.str());
		std::string s3(".txt");
       	//std::cout<< (s1+s2+s3).c_str()<<std::endl;
		//system("pwd");
		mft.load_file( (s1+s2+s3).c_str() );
		Tmps_matrix M1;
		M1.get_matrix( mft );	
		Vmps_matrix.push_back(M1);
	}

	for(int i0 = 0; i0 < loop_return_L( i ); ++i0)
	{
		std::string s1("rm mps");
		std::stringstream ss1;
		ss1 << i0;
		s1.append(ss1.str());
		s1.append(".txt");
		system(s1.c_str());
	}	
	chdir ( ".." );
	chdir( ".." );
	
	Tmps_state MPS1( Vmps_matrix );
    return MPS1;
}
// ========================================================================================


// ----------------------------------------------------------------------------------------
void setup_simulation::get_entanglement_spectra( const char* which_state )
{	
	for( int i =0 ; i< number_of_points; ++i ){
		std::fstream fs1;
		//std::string curr_dir =  "./"+loop_return_string(i)+"/";

		std::string str_dm("dm_spectr_state");
		std::string str_en("ent_spectr_state");
		str_dm.append(which_state);
		str_en.append(which_state);
		str_dm.append( loop_return_string(i) );
		str_en.append( loop_return_string(i) );
		str_dm.append(".dat");
		str_en.append(".dat");

		fs1.open(str_dm.c_str(), std::fstream::out);	
		//chdir( curr_dir.c_str() );		
	    /*
		std::string str1("results.task1.out.");
		str1.append(which_state);
		str1.append(".chkp");
		
		chdir ( str1.c_str() );
		sleep(0.2);*/
		//decode .h5 files
        
        Tmps_state MPS1;
        MPS1 = get_MPS_from_ith_folder( i, which_state);
		Tmps_matrix reduced_dm;
		reduced_dm = MPS1.calc_reduced_density_matrix( loop_return_L( i )/2 );
	
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
		
		fs1<< eigensolver.eigenvalues() << std::endl;


	
		//sleep(0.2);

		fs1.close();
	
		fs1.open(str_dm.c_str(), std::fstream::in);	
		std::fstream fs2;
		fs2.open(str_en.c_str(), std::fstream::out);	

		std::vector<long double > eigV;

		for(int j = 0; j < reduced_dm.get_left_i(); ++j)
		{
			long double tmp1;
			fs1 >> tmp1;
			eigV.push_back( - std::log(tmp1) );
		}
		std::sort (eigV.begin(), eigV.end() );
		for(int j = 0; j < reduced_dm.get_left_i(); ++j)
		{
			fs2 << eigV[j] <<std::endl;
		}	

	
		fs1.close();
		fs2.close();

	}
}





std::string setup_simulation::loop_return_TVstr( int i0 )
{


	int tmp = 0;

	for( auto L : chainL ){
	for( auto Jz : parm_Jz ){
	for( auto dwa : parm_dwa ){
	for( auto T : parm_T ){
	for( auto V : parm_V ){

		if(i0 < ++tmp){
			std::string s1;

			std::stringstream ssL, ssJz,ssdwa, ssT, ssV;
			ssL << L;
			ssJz << Jz;
			ssdwa << dwa;
			ssT << T;
			ssV << V;
		
			s1 = "_T_"+ssT.str()+"_V_"+ssV.str();

			return s1;
		}


	}
	}
	}
	}
	}




}




int setup_simulation::loop_return_L( int i0 ){

	int tmp = 0;

	for( auto L : chainL ){
	for( auto Jz : parm_Jz ){
	for( auto dwa : parm_dwa ){
	for( auto T : parm_T ){
	for( auto V : parm_V ){

		if(i0 < ++tmp){
			return L;
		}


	}
	}
	}
	}
	}


}

long double setup_simulation::loop_return_Jz( int i0 ){

	int tmp = 0;

	for( auto L : chainL ){
	for( auto Jz : parm_Jz ){
	for( auto dwa : parm_dwa ){
	for( auto T : parm_T ){
	for( auto V : parm_V ){

		if(i0 < ++tmp){
			return Jz;
		}


	}
	}
	}
	}
	}


}

long double setup_simulation::loop_return_dwa( int i0 ){

	int tmp = 0;

	for( auto L : chainL ){
	for( auto Jz : parm_Jz ){
	for( auto dwa : parm_dwa ){
	for( auto T : parm_T ){
	for( auto V : parm_V ){

		if(i0 < ++tmp){
			return dwa;
		}


	}
	}
	}
	}
	}


}

long double setup_simulation::loop_return_T( int i0 ){

	int tmp = 0;

	for( auto L : chainL ){
	for( auto Jz : parm_Jz ){
	for( auto dwa : parm_dwa ){
	for( auto T : parm_T ){
	for( auto V : parm_V ){

		if(i0 < ++tmp){
			return T;
		}


	}
	}
	}
	}
	}


}

long double setup_simulation::loop_return_V( int i0 ){

	int tmp = 0;

	for( auto L : chainL ){
	for( auto Jz : parm_Jz ){
	for( auto dwa : parm_dwa ){
	for( auto T : parm_T ){
	for( auto V : parm_V ){

		if(i0 < ++tmp){
			return V;
		}


	}
	}
	}
	}
	}


}

std::string setup_simulation::loop_return_string( int i0 ){

	int tmp = 0;

	for( auto L : chainL ){
	for( auto Jz : parm_Jz ){
	for( auto dwa : parm_dwa ){
	for( auto T : parm_T ){
	for( auto V : parm_V ){

		if(i0 < ++tmp){
			std::string s1;

			std::stringstream ssL, ssJz,ssdwa, ssT, ssV;
			ssL << L;
			ssJz << Jz;
			ssdwa << dwa;
			ssT << T;
			ssV << V;
		
			s1 = "L_"+ssL.str()+"_Jz_"+ssJz.str()+"_dwa_"+ssdwa.str()+"_T_"+ssT.str()+"_V_"+ssV.str();

			return s1;
		}


	}
	}
	}
	}
	}


}



































