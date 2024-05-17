#ifndef _CPP_AADS_HW3_TASK_01_SETGRAPH_H_
#define _CPP_AADS_HW3_TASK_01_SETGRAPH_H_

#include "IGraph.h"
#include <set>

class SetGraph : public IGraph
{
    std::vector<std::set<int>> _graphSet;

public:
    SetGraph( size_t size ) : _graphSet(size) {}
    SetGraph( const IGraph& rhs );

    void AddEdge( int from, int to ) override;

    size_t VerticesCount() const override { return _graphSet.size(); }

    std::vector<int> GetNextVertices( int vertex ) const override;
    std::vector<int> GetPrevVertices( int vertex ) const override;
};

#endif  // _CPP_AADS_HW3_TASK_01_SETGRAPH_H_
