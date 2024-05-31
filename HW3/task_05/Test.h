#ifndef _CPP_AADS_HW3_TASK_05_TEST_H_
#define _CPP_AADS_HW3_TASK_05_TEST_H_

#include "ListGraph.h"
#include "DSU.h"
#include "BoxMuller.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <limits>
#include <ctime>

class Test
{
    const size_t _TEST_SIZE;
    const size_t _LEFT;
    const size_t _RIGHT;

    std::vector<double> testResult;

    ListGraph fillGraph( const size_t& verticesCount );
    void tester( const size_t& verticesCount );
    double calcAvg();
    double calcDispersion( const double& avg );
    void printResult( const size_t& verticesCount );
public:
    Test( const size_t& size, const size_t& l, const size_t& r ) : _TEST_SIZE(size), _LEFT(l), _RIGHT(r) {}

    void make();
};


#endif  // _CPP_AADS_HW3_TASK_05_TEST_H_
