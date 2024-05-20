#ifndef _CPP_AADS_HW3_TASK_01_MATRIXGRAPH_H_
#define _CPP_AADS_HW3_TASK_01_MATRIXGRAPH_H_

#include "IGraph.h"
#include <algorithm>

class MatrixGraph : public IGraph
{
    std::vector<std::vector<bool>> _graphMatrix;

public:
    MatrixGraph( size_t size );
    MatrixGraph( const IGraph& rhs );
    ~MatrixGraph();

    void AddEdge( int from, int to ) override;

    size_t VerticesCount() const override { return _graphMatrix.size(); }

    std::vector<int> GetNextVertices( int vertex ) const override;
    std::vector<int> GetPrevVertices( int vertex ) const override;
};

#endif  // _CPP_AADS_HW3_TASK_01_MATRIXGRAPH_H_
