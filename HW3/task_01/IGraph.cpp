#include "IGraph.h"

std::ostream &operator<<( std::ostream &os, const IGraph &graph )
{
    for( size_t i = 0, I = graph.VerticesCount(); i < I; ++i )
        for( auto j : graph.GetNextVertices(i) )
            os << i << ", " << j << "\n";
    return os;
}


