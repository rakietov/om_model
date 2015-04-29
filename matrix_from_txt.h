#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#ifndef MATRIX_FROM_TXT_H
#define MATRIX_FROM_TXT_H

class matrix_from_txt
{
public:

	matrix_from_txt();
	matrix_from_txt( const char* fname );
	
	void load_file( const char* fname );

	std::ifstream file;

	
	void load_indices();
	void set_size();
	void load_data_();
	void load_data_alter();

	void write_indices();
	void write_ith_Mdata(int i);
	
	
	int get_phys_i() { return phys_i;  };
	int get_left_i() { return left_i;  };
	int get_right_i(){ return right_i; };

	std::vector< std:: vector < std::vector<long double> > >::iterator get_Mdata(){ return Mdata.begin(); };

private:
	int phys_i, left_i, right_i;
	std::vector< std:: vector < std::vector<long double> > > Mdata;
};


#endif


