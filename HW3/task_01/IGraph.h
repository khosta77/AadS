#ifndef _CPP_AADS_HW3_TASK_01_IGRAPH_H_
#define _CPP_AADS_HW3_TASK_01_IGRAPH_H_

#include <vector>
#include <cstddef>
#include <ostream>

struct IGraph
{
    virtual ~IGraph(){};

    // Добавление ребра от from к to.
    virtual void AddEdge( int from, int to ) = 0;

    virtual size_t VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices( int vertex ) const = 0;
    virtual std::vector<int> GetPrevVertices( int vertex ) const = 0;
};

#include "exceptions.h"

std::ostream& operator<<( std::ostream& os, const IGraph& graph );

#endif  // _CPP_AADS_HW3_TASK_01_IGRAPH_H_
