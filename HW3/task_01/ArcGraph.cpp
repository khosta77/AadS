#include "ArcGraph.h"

ArcGraph::ArcGraph( const IGraph& rhs ) : _verticesCount(rhs.VerticesCount())
{
    for( size_t i = 0; i < _verticesCount; ++i )
        for( const auto& j : rhs.GetNextVertices(i) )
            _graphPair.emplace_back( i, j );
}

void ArcGraph::AddEdge( int from, int to )
{
    if( ( ( from > _verticesCount ) || ( to > _verticesCount ) ) )
        throw OutOfRangeFromTo( from, to, _verticesCount );
    _graphPair.emplace_back(from, to);
}

std::vector<int> ArcGraph::GetNextVertices( int vertex ) const
{
    if( vertex > _verticesCount )
        throw OutOfRangeVertex( vertex, _verticesCount );
    std::vector<int> nextVertices;
    for( auto it = _graphPair.begin(); it != _graphPair.end(); ++it )
        if( it->first == vertex )
            nextVertices.push_back(it->second);
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices( int vertex ) const
{
    if( vertex > _verticesCount )
        throw OutOfRangeVertex( vertex, _verticesCount );
    std::vector<int> prevVertices;
    for( auto it = _graphPair.begin(); it != _graphPair.end(); ++it )
        if( it->second == vertex )
            prevVertices.push_back(it->first);
    return prevVertices;
}


