#ifndef _CPP_AADS_HW3_TASK_01_ARCGRAPH_H_
#define _CPP_AADS_HW3_TASK_01_ARCGRAPH_H_

#include "IGraph.h"

class ArcGraph : public IGraph
{
    std::vector<std::pair<int, int>> _graphPair;
    size_t _verticesCount;

public:
    ArcGraph( size_t verticesCount ) : _verticesCount(verticesCount) {}
    ArcGraph( const IGraph& rhs );

    void AddEdge( int from, int to ) override;

    size_t VerticesCount() const override { return _verticesCount; }

    std::vector<int> GetNextVertices( int vertex ) const override;
    std::vector<int> GetPrevVertices( int vertex ) const override;
};

#endif  // _CPP_AADS_HW3_TASK_01_ARCGRAPH_H_
