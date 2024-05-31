#ifndef _CPP_AADS_HW3_TASK_05_BOXMULLER_H_
#define _CPP_AADS_HW3_TASK_05_BOXMULLER_H_

#include "ListGraph.h"
#include "DSU.h"
#include <cassert>
#include <random>
#include <limits>
#include <ctime>

// https://ru.wikipedia.org/wiki/Преобразование_Бокса_—_Мюллера
class BoxMuller
{
    const double _MAX_DOUBLE;
    double genDouble();

    std::vector<std::pair<double, double>> z;

public:
    BoxMuller( const size_t& count );
    double operator()( const size_t& from, const size_t& to );
};

#endif  // _CPP_AADS_HW3_TASK_05_BOXMULLER_H_
