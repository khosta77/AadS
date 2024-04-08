#include <iostream>
#include <fstream>
#include <random>

/*
// Задача 6.4
Дано множество целых чисел из [0..10^9] размера n. Используя алгоритм поиска k-ой порядковой статистики,
требуется найти следующие параметры множества:
* 10%  перцентиль
* медиана
* 90%  перцентиль

* Требования: к дополнительной памяти: O(n). 
* Среднее время работы: O(n)

Должна быть отдельно выделенная функция partition. 
Рекурсия запрещена. 
Решение должно поддерживать передачу функции сравнения снаружи.

Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
Описание для случая прохода от начала массива к концу:
* Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
* Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного.
  Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы.
  Последним элементом лежит опорный.
* Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
* Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
* Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
  Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
* В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

6_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

//// Код
constexpr size_t percentages_size = 3;
constexpr size_t percentages[percentages_size] = { 10, 50, 90 };

template <typename T = int>  // Взял с семинара
bool defaultLess( const T &l, const T &r )
{
    return l < r;
}

template <typename T = int>
class Statistic
{
private:
    T* _arr;
    size_t _size;
    bool (*_cmp)( const T&, const T& );

    size_t _from;
    size_t _to;

    std::random_device _engine;

    inline void mySwap( T& l, T& r )
    {
        T tmp = l;
        l = r;
        r = tmp;
    }

    inline size_t genPivot()
    {
        return std::uniform_real_distribution<size_t>( _from, _to )( _engine );
    }

    size_t partition()
    {
        const size_t pivotIndex = genPivot();
        const T pivot = _arr[pivotIndex];
        mySwap( _arr[pivotIndex], _arr[_to] );
        size_t i = _from, j = _to;
        while( _cmp( _arr[_to], _arr[i] ) )
        {
            while( ( i < _to ) && ( _cmp( _arr[i], pivot ) || _arr[i] == pivot ) )
                ++i;
            while( ( ( j < _to ) && _cmp( pivot, _arr[j] ) ) || ( j < i ) )
                ++j;

            ( ( j < _to ) && _cmp( _arr[j], _arr[i] ) ) ? mySwap( _arr[i++], _arr[j++] ) :
                mySwap( _arr[i], _arr[_to] );
        }
        return i;
    }

    T calc( const size_t& k = 50 )
    {
        _from = 0;
        _to = ( _size - 1 );
        size_t pivotIndex = partition();
        while( pivotIndex != k )
        {
            ( k < pivotIndex ) ? _to = ( --pivotIndex ) : _from = ( ++pivotIndex );
            pivotIndex = partition();
        }
        return _arr[pivotIndex];
    }

public:
    Statistic( T* arr, const size_t& size, bool ( *cmp )( const T& l, const T& r ) = defaultLess ) :
        _arr(arr), _size(size), _cmp(cmp) {}
    ~Statistic() {}

    T calcPercent( const size_t& k )
    {
        return calc( ( ( k * _size ) / 100 ) );
    }
};

void make( int *arr, const size_t& N )
{
    Statistic k( arr, N );
    for( size_t i = 0; i < percentages_size; ++i )
        std::cout << k.calcPercent( percentages[i] ) << std::endl;
}

//// Тест
#ifdef MAKETEST
const std::string IN_PATH = "input/";
const std::string OUT_PATH = "output/";

class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

class Fakap : public std::exception
{
public:
    explicit Fakap(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

void testFromFile( const std::string& ft, const std::string& fr )
{
    static size_t I = 1;
    std::ifstream in_test(( IN_PATH + ft )), in_result(( OUT_PATH + fr ));
    if(!in_test)
        throw FileNotOpen(( IN_PATH + ft ).c_str());
    if(!in_result)
        throw FileNotOpen(( IN_PATH + fr ).c_str());
    size_t tN = 0, rN = 0;
    in_test >> tN;
    int* df = new int[tN];
    for( size_t i = 0; i < tN; ++i )
        in_test >> df[i];
    in_result >> rN;
    int* results = new int[rN];
    for( size_t i = 0; i < rN; ++i )
        in_result >> results[i];

    Statistic k( df, tN );
    int buffer = 0;
    for( size_t i = 0; i < rN; ++i )
    {
        buffer = k.calcPercent( percentages[i] );
        if( buffer != results[i] )
            throw Fakap( "Test № " + std::to_string(I) + " wait: " + std::to_string(results[i])
                         + " out: " + std::to_string(buffer) );
    }
    ++I;
}
#endif

int main()
{
#ifdef MAKETEST
    testFromFile( "in01.txt", "out01.txt" );
#else

#endif
    return 0;
}

