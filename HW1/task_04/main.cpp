#include <iostream>
#include <cassert>

/*
// Задача 4.1
Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
//// Требования:
    Время работы O(N * logK)
    Куча должна быть реализована в виде шаблонного класса.
    Решение должно поддерживать передачу функции сравнения снаружи.
    Куча должна быть динамической.
//// Формат ввода:
    Сначала вводится количество массивов K. Затем по очереди размер каждого массива и элементы массива.
    Каждый массив упорядочен по возрастанию.
////Формат вывода:
    Итоговый отсортированный массив.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <random>
static std::random_device engine;
#endif
template <typename T = int>  // Взял с семинара
bool defaultLess( const T &l, const T &r )
{
    return l < r;
}

//// Код
template <typename T = int>
class MyHeap
{
private:
    size_t _size;
    size_t _image_size;
    bool (*_cmp)( const T&, const T& );
    T* _arr;

    inline void mySwap( T& l, T& r )
    {
        T tmp = l;
        l = r;
        r = tmp;        
    }

    size_t localSwap( T& l, T& r, const size_t li )
    {
        mySwap( l, r );
        return li;
    }

    void reBuild()
    {
        _size *= 2;
        T* buffer = new T[_size];
        for( size_t i = 0; i < _image_size; ++i)
            buffer[i] = _arr[i];
        delete[] _arr;
        _arr = buffer;
    }

    inline size_t root( const size_t& I ) const { return ( ( I != 0 ) ? ( ( I - 1 ) / 2 ) : 0 ); }
    
    inline size_t left( const size_t& I ) const { return ( ( 2 * I ) + 1 ); }
    
    inline size_t right( const size_t& I ) const { return ( ( 2 * I ) + 2 ); }

    void sieveUp( const size_t& I )
    {
        size_t i = I;
        size_t j = root(i);

        while( ( i != 0 ) && ( _arr[i] < _arr[j] ) )
        {
            i = localSwap( _arr[j], _arr[i], j );
            j = root(i);
        }
    }

    void sieveDown( const size_t& I )
    {
        size_t i = I;
        size_t l = left(i);
        size_t r = right(i);

        while( ( i < ( _image_size / 2 ) ) && ( ( _arr[l] < _arr[i] ) ||
                    ( ( _arr[r] < _arr[i] ) && ( r < _image_size ) ) ) )
        {
            i = ( ( _arr[l] < _arr[r] ) || ( r >= _image_size ) ) ? localSwap( _arr[l], _arr[i], l ) : 
                                                                    localSwap( _arr[r], _arr[i], r );
            l = left(i);
            r = right(i);
        }
    }

public:
    MyHeap( bool ( *cmp )( const T& l, const T& r ) = defaultLess ) : _size(1), _image_size(0), _cmp(cmp)
    {
        _arr = new T[_size];
    }

    ~MyHeap()
    {
        delete[] _arr;
    }

    void push( T element )
    {
        if ( _image_size == _size )
            reBuild();
        _arr[_image_size++] = element;
        sieveUp( ( _image_size - 1 ) );
    }

    T pop()
    {
        if ( _image_size == 0 )
            assert(0);
        mySwap( _arr[0], _arr[( _image_size - 1 )] );
        T result = _arr[--_image_size];
        sieveDown(0);
        return result;
    }

};


//// Тест
#ifdef MAKETEST
template <typename T = int>  // Взял с семинара
bool defaultLess( const T &l, const T &r )
{
    return l < r;
}

template <typename T = int>
void mySort( T *arr, int l, int r, bool ( *cmp )( const T &l, const T &r ) = defaultLess )
{
    for ( int i = l; i < r; i++ )
    {
        for ( int j = l; j < r; j++ )
        {
            if ( cmp( arr[j + 1], arr[j] ) )
            {
                std::swap( arr[j + 1], arr[j] );
            }
        }
    }
}

template <typename T = int>
bool arrComparator( T* lm, T* rm, const size_t& N, bool ( *cmp )( const T &l, const T &r ) = defaultLess )
{
    for( size_t i = 0; i < N; ++i )
        if( cmp( lm[i], rm[i] ) )
            return false;
    return true;
}

template <typename T = int>
T genT(T from, T to) {
    return std::uniform_real_distribution<T>(from, to)(engine);
}

template <typename T = int>
void randomGenerator( T* arr, const size_t& N, T from, T to )
{
    for( size_t i = 0; i < N; ++i)
        arr[i] = genT(from, to);
}

template <typename T = int>
void fillHeapFromArr( MyHeap<T>& mh, T* arr, const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
        mh.push(arr[i]);
}

template <typename T = int>
void fillArrFromHeap( MyHeap<T>& mh, T* arr, const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
        arr[i] = mh.pop();
}

template <typename T = int>
bool tester( MyHeap<T>& mh, const size_t& size )
{
    T *arr_test = new T[size];
    T *arr_heap = new T[size];
    randomGenerator<T>( &arr_test[0], size, -10, 10);
    fillHeapFromArr<T>( mh, &arr_test[0], size );
    fillArrFromHeap<T>( mh, &arr_heap[0], size );
    mySort<T>( &arr_test[0], 0, ( size - 1 ) );
    return arrComparator<T>( &arr_test[0], &arr_heap[0], size );
}

void test1()
{
   MyHeap mh;
   assert( tester( mh, 1 ) );
}

void test2()
{
   MyHeap mh;
   assert( tester( mh, 2 ) );
}

void test3()
{
   MyHeap mh;
   assert( tester( mh, 3 ) );
}

void test4()
{
   MyHeap mh;
   assert( tester( mh, 5 ) );
}

void test5()
{
   MyHeap mh;
   assert( tester( mh, 16 ) );
}

void test6()
{
   MyHeap mh;
   assert( tester( mh, 100 ) );
}

void test7()
{
   MyHeap mh;
   assert( tester( mh, 1000 ) );
}

void test8()
{
   MyHeap<float> mh;
   assert( tester<float>( mh, 1000 ) );
}

void test9()
{
   MyHeap<double> mh;
   assert( tester<double>( mh, 1000 ) );
}

#endif

int main()
{
#ifdef MAKETEST
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
/*
    MyHeap mh;
    for( int i = 10; i > 0; --i )
        mh.push(i);

    for( int i = 0; i < 10; ++i )
        std::cout << mh.pop() << " ";
    std::cout << std::endl;
*/
#else
    size_t N = 0, SIZE = 0;
    MyHeap mh;
    int buffer;
    std::cin >> N;
    for( size_t i = 0, j = 0, k = 0; i < N; ++i)
    {
        std::cin >> j;
        for( k = 0; k < j; ++k )
        {
            std::cin >> buffer;
            mh.push(buffer);
        }
        SIZE += j;
    }
    for( size_t i = 0; i < SIZE; ++i )
        std::cout << mh.pop() << " ";
    std::cout << std::endl;
#endif
    return 0;
}


