#include "MatrixGraph.h"

MatrixGraph::MatrixGraph( size_t size ) : _graphMatrix(size)
{
    for( size_t i = 0; i < size; ++i )
    {
        _graphMatrix[i].resize(size);
        std::fill( _graphMatrix[i].begin(), _graphMatrix[i].end(), 0 );
    }
}

MatrixGraph::MatrixGraph( const IGraph& rhs ) : _graphMatrix(rhs.VerticesCount())
{
    size_t size = rhs.VerticesCount();
    for( size_t i = 0, I = rhs.VerticesCount(); i < size; ++i )
    {
        _graphMatrix[i].resize(I);
        auto neighbors = rhs.GetNextVertices(i);
        for( const auto& j : neighbors )
            _graphMatrix[i][j] = 1;
    }
}

void MatrixGraph::AddEdge( int from, int to )
{
    if( ( from > _graphMatrix.size() ) || ( to > _graphMatrix.size() ) )
        throw OutOfRangeFromTo( from, to, _graphMatrix.size() );
    _graphMatrix[from][to] = 1;
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
    if( vertex > _graphMatrix.size() )
        throw OutOfRangeVertex( vertex, _graphMatrix.size() );
    std::vector<int> nextVertices;
    for( size_t i = 0, I = _graphMatrix.size(); i < I; ++i )
        if( _graphMatrix[vertex][i] == 1 )
            nextVertices.push_back(i);
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    if( vertex > _graphMatrix.size() )
        throw OutOfRangeVertex( vertex, _graphMatrix.size() );
    std::vector<int> prevVertices;
    for( size_t i = 0, I = _graphMatrix.size(); i < I; ++i )
        if( _graphMatrix[i][vertex] == 1 )
            prevVertices.push_back(i);
    return prevVertices;
}


