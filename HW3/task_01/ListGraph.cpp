#include "ListGraph.h"

ListGraph::ListGraph( const IGraph& rhs ) : _graphList(rhs.VerticesCount())
{
    for( size_t i = 0; i < _graphList.size(); ++i )
        _graphList[i] = rhs.GetNextVertices(i);
}

ListGraph::~ListGraph()
{
    for( size_t i = 0; i < _graphList.size(); ++i )
        _graphList[i].clear();
    _graphList.clear();
}

void ListGraph::AddEdge( int from, int to )
{
    if( ( from >= _graphList.size() ) || ( to >= _graphList.size() ) )
        throw OutOfRangeFromTo( from, to, _graphList.size() );
    _graphList[from].push_back(to);
}

std::vector<int> ListGraph::GetNextVertices( int vertex ) const
{
    if( vertex >= _graphList.size() )
        throw OutOfRangeVertex( vertex, _graphList.size() );
    return _graphList[vertex];
}

std::vector<int> ListGraph::GetPrevVertices( int vertex ) const
{
    if( vertex >= _graphList.size() )
        throw OutOfRangeVertex( vertex, _graphList.size() );
    std::vector<int> prevVertices;
    for( size_t i = 0, I = _graphList.size(); i < I; ++i )
        for( size_t j = 0, J = _graphList[i].size(); j < J; ++j )
            if( _graphList[i][j] == vertex )
            {
                prevVertices.push_back(i);
                break;
            }
    return prevVertices;
}


