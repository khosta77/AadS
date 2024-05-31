#ifndef _CPP_AADS_HW3_TASK_05_UTILS_H_
#define _CPP_AADS_HW3_TASK_05_UTILS_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include "ListGraph.h"
#include "DSU.h"

double enumeration( const IGraph& graph );
size_t run( std::istream &in );
void makeTest( const std::string& input, const size_t& resultTrue );

#endif  // _CPP_AADS_HW3_TASK_05_UTILS_H_
