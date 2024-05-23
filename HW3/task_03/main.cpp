#include <iostream>
#include <vector>
#include <cstddef>
#include <ostream>
#include <fstream>
#include <exception>
#include <string>
#include <limits>
#include <algorithm>
#include <queue>
#include <set>

/*
// Задача 3.1 Города
Требуется отыскать самый выгодный маршрут между городами.
//// Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
//// Формат входных данных.
* Первая строка содержит число N – количество городов.
* Вторая строка содержит число M - количество дорог.
* Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
* Последняя строка содержит маршрут (откуда и куда нужно доехать).
//// Формат выходных данных.
* Вывести длину самого выгодного маршрута.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

//// Код

class IGraphException : public std::exception
{
private:
    std::string m_msg;

public:
    explicit IGraphException(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override
    {
		return m_msg.c_str();
	}
};

class OutOfRangeFromTo : public IGraphException
{
public:
    explicit OutOfRangeFromTo( const int& from, const int& to, const int& size ) : IGraphException(
            "Индекс для " + ( ( from < size ) ? ( "from(" + std::to_string(from) + ") " ) : (
            "to(" + std::to_string(to) + ") " ) ) + " больше чем " + std::to_string(size)
    ) {}
};

class OutOfRangeVertex : public IGraphException
{
public:
    explicit OutOfRangeVertex( const int& vertex, const int& count ) : IGraphException(
            "Индекс " + std::to_string(vertex) + " больше чем " + std::to_string(count)
    ) {}
};

struct IGraph
{
    // Конструктор копирования тоже опустил
    virtual ~IGraph(){};

    // Добавление ребра от from к to.
    virtual void AddEdge( int from, int to, int roadTime ) = 0;

    virtual size_t VerticesCount() const = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices( int vertex ) const = 0;
};

#if 1
// Ну так как из задания 2 известно, что графа не через список не проходят тесты, то остальные графы я сотру
class ListGraph : public IGraph
{
    std::vector<std::vector<std::pair<int, int>>> _graphList;

public:
    ListGraph( size_t size ) : _graphList(size) {}
    ~ListGraph()
    {
        for( size_t i = 0; i < _graphList.size(); ++i )
            _graphList[i].clear();
        _graphList.clear();
    }

    void AddEdge( int from, int to, int roadTime ) override
    {
        if( ( from >= _graphList.size() ) || ( to >= _graphList.size() ) )
            throw OutOfRangeFromTo( from, to, _graphList.size() );
        _graphList[from].emplace_back(roadTime, to);
        _graphList[to].emplace_back(roadTime, from);
    }

    size_t VerticesCount() const override { return _graphList.size(); }

    std::vector<std::pair<int, int>> GetNextVertices( int vertex ) const override
    {
        if( vertex >= _graphList.size() )
            throw OutOfRangeVertex( vertex, _graphList.size() );
        return _graphList[vertex];
    }
};
#endif

std::ostream &operator<<( std::ostream &os, const IGraph& graph )  // Для отладки
{
    for( size_t i = 0, I = graph.VerticesCount(); i < I; ++i )
        for( auto j : graph.GetNextVertices(i) )
            os << i << ", " << j.first << ", "<< j.second << "\n";
    return os;
}

size_t findTime( const IGraph& graph, const size_t& from, const size_t& to )
{
    std::vector<size_t> optimalTime( graph.VerticesCount(), std::numeric_limits<size_t>::max() );
    optimalTime[from] = 0;

    std::multiset<std::pair<int, int>> times;
    times.insert( std::pair<int, int>( optimalTime[from], from ) );

    while( !times.empty() )
    {
        size_t vertex = times.begin()->second;
        times.erase( times.begin() );
        for( const auto& it : graph.GetNextVertices(vertex) )
        {
            if( optimalTime[it.second] > ( optimalTime[vertex] + it.first ) )
            {
                if( optimalTime[vertex] != std::numeric_limits<size_t>::max() )
                    times.erase( std::pair<int, int>( optimalTime[vertex], vertex ) );
                optimalTime[it.second] = ( optimalTime[vertex] + it.first );
                times.insert( std::pair<int, int>( it.first, it.second ) );
            }
        }
    }
    return optimalTime[to];
}

size_t run( std::istream &in )
{
    size_t size = 0;
    in >> size;
    ListGraph graph(size);
    in >> size;
    size_t from = 0, to = 0;
    for (size_t i = 0, time = 0; ( ( i < size ) && ( in >> from >> to >> time ) ); ++i)
        graph.AddEdge(from, to, time);
    in >> from >> to;
    return findTime(graph, from, to);
}

//// Тест
#ifdef MAKETEST
class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

void makeTest( const std::string& input, const size_t& resultTrue )
{
    std::ifstream in( input );
    if( !in )
        throw FileNotOpen( input );

    size_t result = run( in );
    assert( ( result == resultTrue ) );
}
#endif

int main()
{
#ifdef MAKETEST
    makeTest( "./input/in01.txt", 9 );
#else
    std::cout << run( std::cin ) << std::endl;
#endif
    return 0;
}


