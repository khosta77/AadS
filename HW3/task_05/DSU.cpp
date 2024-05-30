#include "DSU.h"

std::vector<std::pair<int, double>> DSU::findPrima( const IGraph& graph )
{
    std::vector<std::pair<int, double>> result;
    std::vector<bool> visited( graph.VerticesCount(), false);
    // Приоритетная очередь ~ set, но с операциями как у queue
    std::priority_queue<std::pair<int, std::pair<int, double>>,
                        std::vector<std::pair<int, std::pair<int, double>>>,
                        std::greater<>> graphs;

    int start = 0;
    visited[start] = true;

    for( const auto& pair : graph.GetNextVertices(start) )
        graphs.push( std::pair<int, std::pair<int, double>>( pair.second, pair ) );

    while( !graphs.empty() )
    {
        auto currentPair = graphs.top().second;
        graphs.pop();

        int currentVertex = currentPair.first;
        if( visited[currentVertex] )
            continue;
        visited[currentVertex] = true;
        result.push_back(currentPair);

        for( const auto& nextPair : graph.GetNextVertices(currentVertex) )
            if( !visited[nextPair.first] )
                graphs.push( std::pair<int, std::pair<int, double>>( nextPair.second, nextPair ) );
    }
    return result;
}

double DSU::calcTotalTime( const std::vector<std::pair<int, double>>& roads )
{
    int totalTime = 0;
    for( const auto& pair : roads )
        totalTime += pair.second;
    return totalTime;
}


