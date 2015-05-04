#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "mps_matrix.h"

#ifndef MPS_STATE_VIDAL_H
#define MPS_STATE_VIDAL_H


class Tmps_state_vidal{
public:






private:
    std::size_t length;
	std::vector< Tmps_matrix > _gamma_;
    std::vector< std::vector< std::vector<long double> > > _lambda_;

};





#endif


