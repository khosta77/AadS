#ifndef _CPP_AADS_HW3_TASK_05_LISTGRAPH_H_
#define _CPP_AADS_HW3_TASK_05_LISTGRAPH_H_

#include "IGraph.h"

class ListGraph : public IGraph
{
    std::vector<std::vector<std::pair<int, double>>> _graphList;

public:
    ListGraph( size_t size ) : _graphList(size) {}
    ListGraph( const IGraph& rhs );
    ~ListGraph();

    void AddEdge( int from, int to, double roadTime ) override;

    size_t VerticesCount() const override { return _graphList.size(); }

    std::vector<std::pair<int, double>> GetNextVertices( int vertex ) const override;
};

#endif  // _CPP_AADS_HW3_TASK_05_LISTGRAPH_H_
