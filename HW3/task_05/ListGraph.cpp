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

void ListGraph::AddEdge( int from, int to, double roadTime )
{
    if( ( from >= _graphList.size() ) || ( to >= _graphList.size() ) )
        throw OutOfRangeFromTo( from, to, _graphList.size() );
    _graphList[from].emplace_back( to, roadTime );
    _graphList[to].emplace_back( from, roadTime );
}

std::vector<std::pair<int, double>> ListGraph::GetNextVertices( int vertex ) const
{
    if( vertex >= _graphList.size() )
        throw OutOfRangeVertex( vertex, _graphList.size() );
    return _graphList[vertex];
}


