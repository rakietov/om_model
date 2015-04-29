#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "matrix_from_txt.h" 


matrix_from_txt::matrix_from_txt(){

}

void matrix_from_txt::load_file( const char* fname ){
	file.close();
	file.open(fname) ;
	if(file.is_open()){
		load_indices();
		//write_indices();
		set_size();
		load_data_();
	}
	else {
		std::cout<<"ERROR cannot find *.txt file"<<std::endl;
	}
}

matrix_from_txt::matrix_from_txt( const char* fname ){
	file.open(fname) ;
	if(file.is_open()){
		load_indices();
	//	write_indices();
		set_size();
		load_data_();
	}
	else {
		std::cout<<"ERROR cannot find *.txt file"<<std::endl;
	}
}

void matrix_from_txt::load_data_(){

	file.seekg( 0 );
	std::string s;


	while(s !=  "\"data_\""){
		file >> s;
	} 

	for(int i = 0; i < 62; ++i){
		std::getline( file, s);
	}
	file >> s; // we are at the begining of data_ block now

	long double tmp = 0.;
	bool flag = false; 
	int total_n = 0;
	int Tphys_i = 0;
	int Tright_i = 0;
	int Tleft_i = 0;

	while(s != "GROUP"){
		do{
			file >> s;
//			std::cout<<"wczytalem "<<s<<std::endl;
			std::istringstream isstmp(s);
			flag = isstmp >>tmp;
//			std::cout <<"po konwersji "<< tmp << std::endl;
			if(flag){
//				std::cout << tmp << std::endl;

				if(left_i > Tleft_i){
					Mdata[Tphys_i][Tleft_i++][Tright_i] = tmp;
//					std::cout<<"zapisalem element: "<<Tphys_i-1<<" "<<Tleft_i<<" "<<Tright_i<<std::endl;
				}
				else{
					Tleft_i = 0;
					if(phys_i - 1 > Tphys_i ){
						Tphys_i++;
					}
					else {
						Tphys_i = 0;
						Tright_i++;
					}
					Mdata[Tphys_i][Tleft_i++][Tright_i] = tmp;
//					std::cout<<"zapisalem element: "<<Tphys_i-1<<" "<<Tleft_i<<" "<<Tright_i<<std::endl;
				}
				total_n ++;
				if(total_n > phys_i * left_i * right_i){
					std::cout<<"ERROR: too much data_ loaded"<<std::endl;
				} 
			}
		} while( flag );
	}
	if(total_n < phys_i * left_i * right_i){
		std::cout<<"ERROR: too few data_ loaded"<<std::endl;
	}


	file.seekg( 0 );


}

void matrix_from_txt::load_data_alter(){

	std::cout<<"using alternative version of data loader"<<std::endl;
	file.seekg( 0 );
	std::string s;


	while(s !=  "\"data_\""){
		file >> s;
	} 

	for(int i = 0; i < 62; ++i){
		std::getline( file, s);
	}
	file >> s; // we are at the begining of data_ block now

	long double tmp = 0.;
	bool flag = false; 
	int total_n = 0;
	int Tphys_i = 0;
	int Tright_i = 0;
	int Tleft_i = 0;

	while(s != "GROUP"){
		do{
			file >> s;
//			std::cout<<"wczytalem "<<s<<std::endl;
			std::istringstream isstmp(s);
			flag = isstmp >>tmp;
//			std::cout <<"po konwersji "<< tmp << std::endl;
			if(flag){
//				std::cout << tmp << std::endl;

				if(phys_i > Tphys_i){
					Mdata[Tphys_i++][Tleft_i][Tright_i] = tmp;
//					std::cout<<"zapisalem element: "<<Tphys_i-1<<" "<<Tleft_i<<" "<<Tright_i<<std::endl;
				}
				else{
					Tphys_i = 0;
					if(left_i - 1 > Tleft_i ){
						Tleft_i++;
					}
					else {
						Tleft_i = 0;
						Tright_i++;
					}
					Mdata[Tphys_i++][Tleft_i][Tright_i] = tmp;
//					std::cout<<"zapisalem element: "<<Tphys_i-1<<" "<<Tleft_i<<" "<<Tright_i<<std::endl;
				}
				total_n ++;
				if(total_n > phys_i * left_i * right_i){
					std::cout<<"ERROR: too much data_ loaded"<<std::endl;
				} 
			}
		} while( flag );
	}
	if(total_n < phys_i * left_i * right_i){
		std::cout<<"ERROR: too few data_ loaded"<<std::endl;
	}


	file.seekg( 0 );


}




void matrix_from_txt::load_indices(){
	file.seekg( 0 );
	std::string s;
	while(s !=  "\"left_i\""){
		file >> s;
	} 

	for(int i = 0; i < 51; ++i){
		std::getline( file, s);
	}
	file >> s;


	file >> left_i;
   //std::cout << s;
	file.seekg( 0 );


	while(s !=  "\"phys_i\""){
		file >> s;
	} 

	for(int i = 0; i < 51; ++i){
		std::getline( file, s);
	}
	file >> s;


	file >> phys_i;
   //std::cout << s;
	file.seekg( 0 );

	while(s !=  "\"right_i\""){
		file >> s;
	} 

	for(int i = 0; i < 51; ++i){
		std::getline( file, s);
	}
	file >> s;


	file >> right_i;
   //std::cout << s;
	file.seekg( 0 );

}


void matrix_from_txt::set_size(){
	Mdata.resize(phys_i);
	for(int i = 0; i<phys_i; ++i){	
		Mdata[i].resize(left_i);		
		for(int j = 0; j < left_i; ++j){
			Mdata[i][j].resize(right_i);
		}
	}
}

void matrix_from_txt::write_indices(){
	std::cout<< "left_i = " << left_i<<std::endl;
	std::cout<< "right_i = " << right_i<<std::endl;
	std::cout<< "phys_i = " << phys_i<<std::endl;
}


void matrix_from_txt::write_ith_Mdata(int i){
	for(int i1 = 0; i1 < Mdata[i].size(); ++i1){
		for(int i2 = 0; i2 < Mdata[i][i1].size(); ++i2){
			std::cout<<Mdata[i][i1][i2]<<" ";
		}
	std::cout<<std::endl;	
	}
	
}























