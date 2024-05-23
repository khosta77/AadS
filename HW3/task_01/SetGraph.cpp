#include "SetGraph.h"

SetGraph::SetGraph( const IGraph& rhs ) : _graphSet(rhs.VerticesCount())
{
    for( size_t i = 0; i < _graphSet.size(); ++i )
        for( const auto& j : rhs.GetNextVertices(i) )
            _graphSet[i].insert(j);
}

SetGraph::~SetGraph()
{
    for( size_t i = 0; i < _graphSet.size(); ++i )
        _graphSet[i].clear();
    _graphSet.clear();
}

void SetGraph::AddEdge( int from, int to )
{
    if( ( from >= _graphSet.size() ) || ( to >= _graphSet.size() ) )
        throw OutOfRangeFromTo( from, to, _graphSet.size() );
    _graphSet[from].insert(to);
    _graphSet[to].insert(from);
}

std::vector<int> SetGraph::GetNextVertices( int vertex ) const
{
    if( vertex >= _graphSet.size() )
        throw OutOfRangeVertex( vertex, _graphSet.size() );
    std::vector<int> nextVertices;
    for( auto it = _graphSet[vertex].begin(); it != _graphSet[vertex].end(); ++it )
        nextVertices.push_back(*it);
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices( int vertex ) const
{
    if( vertex >= _graphSet.size() )
        throw OutOfRangeVertex( vertex, _graphSet.size() );
    std::vector<int> prevVertices;
    for( size_t i = 0, I = _graphSet.size(); i < I; ++i )
        if( _graphSet[i].find(vertex) != _graphSet[i].end() )
            prevVertices.push_back(i);
    return prevVertices;
}


