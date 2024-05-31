#include "utils.h"

double enumeration( const IGraph& graph )
{
    const size_t verticesCount = graph.VerticesCount();
    std::vector<int> vertices;
    for( size_t i = 0; i < verticesCount; ++i )
        vertices.push_back(i);

    double matrix[verticesCount][verticesCount];
    for( size_t i = 0; i < verticesCount; ++i )
        for( size_t j = 0; j < verticesCount; ++j )
            matrix[i][j] = 0;  // Заполним нулями
    for( size_t i = 0; i < verticesCount; ++i  )
        for( auto it : graph.GetNextVertices(i) )
            matrix[i][it.first] = it.second;

    std::vector<double> times;
    do
    {
        double time = 0;
        for( size_t i = 0; i < verticesCount; ++i )
            if( matrix[i][( i + 1 )] )
                time += matrix[i][( i + 1 )];
        times.push_back(time);
    }  // next_permutation - переборка
    while( std::next_permutation( vertices.begin(), vertices.end() ) );
    return *std::min_element( times.begin(), times.end() );
}

size_t run( std::istream& in )
{
    int size = 0;
    in >> size;
    ListGraph graph(size);
    in >> size;
    for (int i = 0, from = 0, to = 0, time = 0; ( ( i < size ) && ( in >> from >> to >> time ) ); ++i)
        graph.AddEdge( ( from - 1 ), ( to - 1 ), time );
    auto result = DSU::findPrima(graph);
    return DSU::calcTotalTime(result);
}

void makeTest( const std::string& input, const size_t& resultTrue )
{
    std::ifstream in( input );
    if( !in )
        throw FileNotOpen( input );

    size_t result = run( in );
    assert( ( result == resultTrue ) );
}

