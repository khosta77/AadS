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
// Задача 2.1 Количество различных путей
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то
вершинами. Найдите количество различных кратчайших путей между заданными вершинами.
//// Требования: сложность O(V+E). 
//// Формат ввода.
v: кол-во вершин (макс. 50000),
n: кол-во ребер (макс. 200000),
n пар реберных вершин, пара вершин u, w для запроса.
//// Формат вывода.
Количество кратчайших путей от u к w.
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
    virtual void AddEdge( int from, int to ) = 0;

    virtual size_t VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices( int vertex ) const = 0;
    //virtual std::vector<int> GetPrevVertices( int vertex ) const = 0;  // Данный метод не используется.
};

#if 0
// Хотел в начале взять из задания 1, но решил не перегружать файл
class ListGraph : public IGraph
{
    std::vector<std::vector<int>> _graphList;

public:
    ListGraph( size_t size ) : _graphList(size) {}
    ~ListGraph()
    {
        for( size_t i = 0; i < _graphList.size(); ++i )
            _graphList[i].clear();
        _graphList.clear();
    }

    void AddEdge( int from, int to ) override
    {
        if( ( from >= _graphList.size() ) || ( to >= _graphList.size() ) )
            throw OutOfRangeFromTo( from, to, _graphList.size() );
        _graphList[from].push_back(to);
    }

    size_t VerticesCount() const override { return _graphList.size(); }

    std::vector<int> GetNextVertices( int vertex ) const override
    {
        if( vertex >= _graphList.size() )
            throw OutOfRangeVertex( vertex, _graphList.size() );
        return _graphList[vertex];
    }
};
#endif

#if 0
class MatrixGraph : public IGraph
{
    std::vector<std::vector<bool>> _graphMatrix;

public:
    MatrixGraph( size_t size ) : _graphMatrix(size)
    {
        for( size_t i = 0; i < size; ++i )
        {
            _graphMatrix[i].resize(size);
            std::fill( _graphMatrix[i].begin(), _graphMatrix[i].end(), 0 );
        }
    }
    
    ~MatrixGraph()
    {
        for( size_t i = 0; i < _graphMatrix.size(); ++i )
            _graphMatrix[i].clear();
        _graphMatrix.clear();
    }

    void AddEdge( int from, int to ) override
    {
        if( ( from >= _graphMatrix.size() ) || ( to >= _graphMatrix.size() ) )
            throw OutOfRangeFromTo( from, to, _graphMatrix.size() );
        _graphMatrix[from][to] = 1;
    }

    size_t VerticesCount() const override { return _graphMatrix.size(); }

    std::vector<int> GetNextVertices( int vertex ) const override
    {
        if( vertex >= _graphMatrix.size() )
            throw OutOfRangeVertex( vertex, _graphMatrix.size() );
        std::vector<int> nextVertices;
        for( size_t i = 0, I = _graphMatrix.size(); i < I; ++i )
            if( _graphMatrix[vertex][i] == 1 )
                nextVertices.push_back(i);
        return nextVertices;
    }
};
#endif

#if 0
class SetGraph : public IGraph
{
    std::vector<std::set<int>> _graphSet;

public:
    SetGraph( size_t size ) : _graphSet(size) {}
    ~SetGraph()
    {
    for( size_t i = 0; i < _graphSet.size(); ++i )
        _graphSet[i].clear();
    _graphSet.clear();

    }

    void AddEdge( int from, int to ) override
    {
        if( ( from >= _graphSet.size() ) || ( to >= _graphSet.size() ) )
            throw OutOfRangeFromTo( from, to, _graphSet.size() );
        _graphSet[from].insert(to);
    }

    size_t VerticesCount() const override { return _graphSet.size(); }

    std::vector<int> GetNextVertices( int vertex ) const override
    {
        if( vertex >= _graphSet.size() )
            throw OutOfRangeVertex( vertex, _graphSet.size() );
        std::vector<int> nextVertices;
        for( auto it = _graphSet[vertex].begin(); it != _graphSet[vertex].end(); ++it )
            nextVertices.push_back(*it);
        return nextVertices;
    }
};
#endif

#if 1
class ArcGraph : public IGraph
{
    std::vector<std::pair<int, int>> _graphPair;
    size_t _verticesCount;

public:
    ArcGraph( size_t verticesCount ) : _verticesCount(verticesCount) {}
    ~ArcGraph() { _graphPair.clear(); }

    void AddEdge( int from, int to ) override
    {
        if( ( ( from >= _verticesCount ) || ( to >= _verticesCount ) ) )
            throw OutOfRangeFromTo( from, to, _verticesCount );
        _graphPair.emplace_back(from, to);
    }

    size_t VerticesCount() const override { return _verticesCount; }

    std::vector<int> GetNextVertices( int vertex ) const override
    {
        if( vertex >= _verticesCount )
            throw OutOfRangeVertex( vertex, _verticesCount );
        std::vector<int> nextVertices;
        for( auto it = _graphPair.begin(); it != _graphPair.end(); ++it )
            if( it->first == vertex )
                nextVertices.push_back(it->second);
        return nextVertices;
    }
};
#endif

std::ostream &operator<<( std::ostream &os, const IGraph& graph )  // Для отладки
{
    for( size_t i = 0, I = graph.VerticesCount(); i < I; ++i )
        for( auto j : graph.GetNextVertices(i) )
            os << i << ", " << j << "\n";
    return os;
}

size_t findMinsPaths( const IGraph& graph, const size_t& from, const size_t& to )
{
    std::vector<size_t> counterMin( graph.VerticesCount(), 0 ), distance( graph.VerticesCount(), 
                                                                    std::numeric_limits<size_t>::max() );
    counterMin[from] = 1;
    distance[from] = 0;

    std::queue<size_t> queuePaths;
    queuePaths.push(from);

    while( !queuePaths.empty() )
    {
        size_t vertex = queuePaths.front();
        queuePaths.pop();
        for( const auto& neigh : graph.GetNextVertices(vertex) )
        {
            if( counterMin[neigh] == 0 )
            {
                distance[neigh] = ( distance[vertex] + 1 );
                counterMin[neigh] = counterMin[vertex];
                queuePaths.push(neigh);
            }
            else if( distance[neigh] == ( distance[vertex] + 1 ) )
                counterMin[neigh] += counterMin[vertex];
        }
    }
    return counterMin[to];
}

size_t run( std::istream &in )
{
    size_t size = 0, from = 0, to = 0;
    in >> size;
    ArcGraph graph(size);
    in >> size;
    for( size_t i = 0; ( ( i < size ) && ( in >> from >> to ) ); ++i )
        graph.AddEdge( from, to );
    in >> from >> to;
    return findMinsPaths( graph, from, to );
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
    makeTest( "./input/in01.txt", 2 );
#else
    std::cout << run( std::cin ) << std::endl;
#endif
    return 0;
}


