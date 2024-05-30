#ifndef _CPP_AADS_HW3_TASK_05_DSU_H_
#define _CPP_AADS_HW3_TASK_05_DSU_H_

#include <vector>
#include <cstddef>
#include <algorithm>
#include <queue>
#include <set>
#include "IGraph.h"

class DSU
{
public:
    static std::vector<std::pair<int, double>> findPrima( const IGraph& graph );
    static double calcTotalTime( const std::vector<std::pair<int, double>>& roads );
};


#endif  // _CPP_AADS_HW3_TASK_05_DSU_H_
