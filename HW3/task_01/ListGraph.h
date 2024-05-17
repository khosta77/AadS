#ifndef _CPP_AADS_HW3_TASK_01_LISTGRAPH_H_
#define _CPP_AADS_HW3_TASK_01_LISTGRAPH_H_

#include "IGraph.h"
#include <queue>

class ListGraph : public IGraph
{
    std::vector<std::vector<int>> _graphList;

public:
    ListGraph( size_t size ) : _graphList(size) {}
    ListGraph( const IGraph& rhs );

    void AddEdge( int from, int to ) override;

    size_t VerticesCount() const override { return _graphList.size(); }

    std::vector<int> GetNextVertices( int vertex ) const override;
    std::vector<int> GetPrevVertices( int vertex ) const override;
};

#endif  // _CPP_AADS_HW3_TASK_01_LISTGRAPH_H_
