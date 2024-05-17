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
template <typename T>
void testAddEdge()
{
    T graph(5);
    graph.AddEdge( 0, 1 );
    graph.AddEdge( 0, 2 );
    graph.AddEdge( 0, 3 );
    graph.AddEdge( 1, 3 );
    graph.AddEdge( 2, 3 );
    graph.AddEdge( 2, 4 );
    std::stringstream ss;
    ss << graph;
    std::string myString = ss.str();
    std::string TEST_RESULT_01 = "0, 1\n0, 2\n0, 3\n1, 3\n2, 3\n2, 4\n";
    if( TEST_RESULT_01 != myString )
        throw MethodException( graph, "AddEdge" );
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


