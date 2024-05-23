#include <iostream>

/*
// Задача 1 Представление графа
Дан базовый интерфейс для представления ориентированного графа:
```C++
struct IGraph {
    virtual ~IGraph() {}
    
    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};
```
Необходимо написать несколько реализаций интерфейса:
    - ListGraph, хранящий граф в виде массива списков смежности,
    - MatrixGraph, хранящий граф в виде матрицы смежности,
    - SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
    - ArcGraph, хранящий граф в виде одного массива пар {from, to}.
Также необходимо реализовать конструктор, принимающий const IGraph&. Такой конструктор должен скопировать
переданный граф в создаваемый объект.

Для каждого класса создавайте отдельные h и cpp файлы. Число вершин графа задается в конструкторе
каждой реализации.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#include <sstream>
#include <strstream>
#endif

//// Код

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

//// Тест
#ifdef MAKETEST
const std::string TEST_RESULT_01 = "0, 1\n0, 2\n0, 3\n1, 0\n1, 3\n2, 0\n2, 3\n2, 4\n3, 0\n3, 1\n3, 2\n4, 2\n";

const std::vector<int> TEST_RESULT_02 = { 1, 2, 3 };
const std::vector<int> TEST_RESULT_03 = { 0, 3 };
const std::vector<int> TEST_RESULT_04 = { 0, 3, 4 };

const std::vector<int> TEST_RESULT_05 = { 0, 3 };
const std::vector<int> TEST_RESULT_06 = { 0, 3, 4 };
const std::vector<int> TEST_RESULT_07 = { 0, 1, 2 };
const std::vector<int> TEST_RESULT_08 = { 2 };

void printVector( const std::vector<int>& vector )
{
    for( const int& it : vector )
        std::cout << it << ' ';
    std::cout << std::endl;
}

template <typename T>
std::string readStringStream( T& graph )
{
    std::stringstream ss;
    ss << graph;
    return ss.str();
}

template <typename T>
void testAddEdge()
{
    T graph(5);
    // Тест на вывод ошибки в VerticesCount и конструктор
    if( 5 != graph.VerticesCount() )
        throw MethodException( graph, "VerticesCount и конструктор" );

    graph.AddEdge( 0, 1 );
    graph.AddEdge( 0, 2 );
    graph.AddEdge( 0, 3 );
    graph.AddEdge( 1, 3 );
    graph.AddEdge( 2, 3 );
    graph.AddEdge( 2, 4 );

    // Тест на вывод ошибки в AddEdge
    try { graph.AddEdge( 5, 5 ); }
    catch( const OutOfRangeFromTo& oorft ) {}
    catch( ... ) { throw MethodException( graph, "AddEdge" ); }
    // Данный тест не учитывает состояние когда у нас в принципе не прописана проверка на на большие входные 
    // данные. предполагается, что хотя бы какая-то уже есть, котороая возвращает ошибку, чтобы не городить
    // код. В дальнейшем это будет учитыватся и в других тестах

    // Тест на вывод ошибки в VerticesCount
    // Данный тест еще нужен чтобы уточнить, что после добавления размер не менялся
    if( 5 != graph.VerticesCount() )
        throw MethodException( graph, "VerticesCount" );

    // Тест на вывод ошибки в VerticesCount
    if( graph.GetNextVertices(0) != TEST_RESULT_02 )
        throw MethodException( graph, "GetNextVertices(0)" );
    if( graph.GetNextVertices(1) != TEST_RESULT_03 )
        throw MethodException( graph, "GetNextVertices(1)" );
    if( graph.GetNextVertices(2) != TEST_RESULT_04 )
        throw MethodException( graph, "GetNextVertices(2)" );
    try { graph.GetNextVertices(5); }
    catch( const OutOfRangeVertex& oorft ) {}
    catch( ... ) { throw MethodException( graph, "GetNextVertices" ); }

    // Тест на вывод ошибки в VerticesCount
    if( graph.GetPrevVertices(1) != TEST_RESULT_05 )
        throw MethodException( graph, "GetPrevVertices(1)" );
    if( graph.GetPrevVertices(2) != TEST_RESULT_06 )
        throw MethodException( graph, "GetPrevVertices(2)" );
    if( graph.GetPrevVertices(3) != TEST_RESULT_07 )
        throw MethodException( graph, "GetPrevVertices(3)" );
    if( graph.GetPrevVertices(4) != TEST_RESULT_08 )
        throw MethodException( graph, "GetPrevVertices(4)" );
    try { graph.GetPrevVertices(5); }
    catch( const OutOfRangeVertex& oorft ) {}
    catch( ... ) { throw MethodException( graph, "GetPrevVertices" ); }

    // Тест на вывод ошибки в VerticesCount
    std::string result = readStringStream<T>(graph);
    if( TEST_RESULT_01 != result )
        throw MethodException( graph, "operator<<" );

    // Тест конструктора копирования
    T graphCopy(graph);
    std::string resultCopy = readStringStream<T>(graphCopy);
    if( TEST_RESULT_01 != resultCopy )
        throw MethodException( graph, "Конструктор копирования" );
}
#endif

int main()
{
#ifdef MAKETEST
    testAddEdge<ListGraph>();
    testAddEdge<MatrixGraph>();
    testAddEdge<SetGraph>();
    testAddEdge<ArcGraph>();
#else

#endif
    return 0;
}


