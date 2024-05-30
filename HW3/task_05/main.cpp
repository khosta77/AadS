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
//// Задача 5.2 Приближенное решение метрической неориентированной задачи коммивояжера.
Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости)
с помощью минимального остовного дерева. Оцените качество приближения на случайном наборе точек, нормально
распределенном на плоскости с дисперсией 1. Нормально распределенный набор точек получайте с помощью
преобразования Бокса-Мюллера.

При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения. Вычислите
среднее значение и среднеквадратичное отклонение качества приближения для данного N.

Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10]. Автоматизируйте запуск
экспериментов.

В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с
реализацией. 

* Вариант 1. Для построения минимального остовного дерева используйте алгоритм Крускала.
* Вариант 2. Для построения минимального остовного дерева используйте алгоритм Прима.

В контесте протестируйте работу алгоритма построения минимального остовного дерева. (Варианты в контесте - 
не те, который описаны здесь. Правильные варианты - здесь.)
*/

#if 1  // 0, если в Яндекс контесте
#include "ListGraph.h"
#include "DSU.h"
#include <cassert>
#include <random>
#include <limits>
#include <ctime>

double enumeration( const IGraph& graph )
{
    int verticesCount = graph.VerticesCount();
    std::vector<int> vertices;
    for (int i = 0; i < verticesCount; i++) {
        vertices.push_back(i);
    }

    double matrix[verticesCount][verticesCount];
    for( size_t i = 0; i < verticesCount; ++i )
        for( size_t j = 0; j < verticesCount; ++j )
            matrix[i][j] = 0;
    for( size_t i = 0; i < verticesCount; ++i  )
        for( auto it : graph.GetNextVertices(i) )
            matrix[i][it.first] = it.second;

    std::vector<double> weights;
    do
    {
        double weight = 0;
        double edgeWeight = 0;
        for( int i = 0; i < verticesCount; ++i )
        {
            edgeWeight = matrix[i][( i + 1)];
            if( edgeWeight != -1 )
                weight += edgeWeight;
        }

        weights.push_back(weight);
    }
    while( std::next_permutation( vertices.begin(), vertices.end() ) );

    return *std::min_element(weights.begin(), weights.end());
}

size_t run( std::istream &in )
{
    int size = 0;
    in >> size;
    ListGraph graph(size);
    in >> size;
    for (int i = 0, from = 0, to = 0, time = 0; ( ( i < size ) && ( in >> from >> to >> time ) ); ++i)
        graph.AddEdge( ( from - 1 ), ( to - 1 ), time );
    auto result = DSU::findPrima(graph);
    std::cout << enumeration(graph) << ' ' << DSU::calcTotalTime(result) << std::endl;
    return DSU::calcTotalTime(result);
}

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

// https://ru.wikipedia.org/wiki/Преобразование_Бокса_—_Мюллера
class BoxMuller
{
    const double MAX_DOUBLE;
    double genDouble()
    {
        std::random_device engine;
        double ran =  std::uniform_real_distribution<double>( 0, MAX_DOUBLE )(engine);
        return ( ran / MAX_DOUBLE );
    }

    std::vector<std::pair<double, double>> z;

public:
    BoxMuller( const size_t& count ) : MAX_DOUBLE(std::numeric_limits<double>::max())
    {
        for( size_t i = 0; i < count; ++i )
        {
            double x = 0.0, y = 0.0, s = 0.0;
            do
            {
                x = genDouble();
                y = genDouble();
                s = ( std::pow( x, 2 ) + std::pow( y, 2 ) );
            }
            while( ( ( s > 1 ) || ( s == 0 ) ) );
            z.emplace_back(std::pair<double, double>( ( x * std::sqrt(-2 * std::log(s) / s) ),
                                                      ( y * std::sqrt(-2 * std::log(s) / s) ) ));
        }
        //for( const auto& it : points )
        //    std::cout << it.first << ' ' << it.second << std::endl;
    }

    double operator()( const size_t& from, const size_t& to )
    {
        if( from == to )
            return 0;
        return std::sqrt(
            std::pow( z[from].first - z[to].second, 2 ) + std::pow( z[from].second - z[to].second, 2 )
        );
    }
};

class Test
{

public:
    Test( const size_t& count )
    {

    }

    void launch()
    {
        
    }
};

int main()
{
    makeTest( "./input/in01.txt", 7 );
    makeTest( "./input/in02.txt", 107923 );
    Test test;
    test.launch();
    return 0;
}
#else
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
        if( ( from > _graphList.size() ) || ( to > _graphList.size() ) )
            throw OutOfRangeFromTo( from, to, _graphList.size() );
        _graphList[from].emplace_back( to, roadTime );
        _graphList[to].emplace_back( from, roadTime );
    }

    size_t VerticesCount() const override { return _graphList.size(); }

    std::vector<std::pair<int, int>> GetNextVertices( int vertex ) const override
    {
        if( vertex >= _graphList.size() )
            throw OutOfRangeVertex( vertex, _graphList.size() );
        return _graphList[vertex];
    }
};

std::ostream &operator<<( std::ostream &os, const IGraph& graph )  // Для отладки
{
    for( size_t i = 0, I = graph.VerticesCount(); i < I; ++i )
        for( auto j : graph.GetNextVertices(i) )
            os << i << ", " << j.first << ", "<< j.second << "\n";
    return os;
}

class DSU
{
public:
    static std::vector<std::pair<int, int>> findPrima( const IGraph& graph )
    {
        std::vector<std::pair<int, int>> result;
        std::vector<bool> visited( graph.VerticesCount(), false);
        // Приоритетная очередь ~ set, но с операциями как у queue
        std::priority_queue<std::pair<int, std::pair<int, int>>,
                            std::vector<std::pair<int, std::pair<int, int>>>,
                            std::greater<>> graphs;

        int start = 0;
        visited[start] = true;

        for( const auto& pair : graph.GetNextVertices(start) )
            graphs.push( std::pair<int, std::pair<int, int>>( pair.second, pair ) );

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
                    graphs.push( std::pair<int, std::pair<int, int>>( nextPair.second, nextPair ) );
        }
        return result;
    }

    static int calcTotalTime( const std::vector<std::pair<int, int>>& roads )
    {
        int totalTime = 0;
        for( const auto& pair : roads )
            totalTime += pair.second;
        return totalTime;
    }
};

size_t run( std::istream &in )
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
    makeTest( "./input/in01.txt", 7 );
    makeTest( "./input/in02.txt", 107923 );
#else
    std::cout << run( std::cin ) << std::endl;
#endif
    return 0;
}
#endif


