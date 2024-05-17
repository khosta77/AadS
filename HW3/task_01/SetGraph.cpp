#include "SetGraph.h"

SetGraph::SetGraph( const IGraph& rhs ) : _graphSet(rhs.VerticesCount())
{
    for( size_t i = 0; i < _graphSet.size(); ++i )
        for( const auto& j : rhs.GetNextVertices(i) )
            _graphSet[i].insert(j);
}

void SetGraph::AddEdge( int from, int to )
{
    if( ( from > _graphSet.size() ) || ( to > _graphSet.size() ) )
        throw OutOfRangeFromTo( from, to, _graphSet.size() );
    _graphSet[from].insert(to);
}

std::vector<int> SetGraph::GetNextVertices( int vertex ) const
{
    if( vertex > _graphSet.size() )
        throw OutOfRangeVertex( vertex, _graphSet.size() );
    std::vector<int> nextVertices;
    for( const auto& to : _graphSet[vertex] )
        nextVertices.push_back(to);
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices( int vertex ) const
{
    if( vertex > _graphSet.size() )
        throw OutOfRangeVertex( vertex, _graphSet.size() );
    std::vector<int> result;
    for( size_t from = 0; from < _graphSet.size(); ++from )
        if( _graphSet[from].find(vertex) != _graphSet[from].end() )
        {
            result.push_back(from);
            break;
        }
    return result;
}


