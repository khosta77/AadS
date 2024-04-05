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
//#define MAKETEST
#ifdef MAKETEST
#include <random>
static std::random_device engine;
#endif

template <typename T = int>  // Взял с семинара
bool defaultLess( const T &l, const T &r )
{
    return l < r;
}

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

        while( ( i != 0 ) && _cmp( _arr[i], _arr[j] ) )
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

        while( ( i < ( _image_size / 2 ) ) && ( _cmp( _arr[l], _arr[i] ) ||
                     ( _cmp(_arr[r], _arr[i] ) && ( r < _image_size ) ) ) )
        {
            i = ( _cmp( _arr[l], _arr[r] ) || ( r >= _image_size ) ) ? localSwap( _arr[l], _arr[i], l ) : 
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
        delete[] this->_arr;
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

template < typename L, typename R >
struct MyPair
{
public:
    L _l;
    R _r;

    MyPair() {}
    MyPair( const L& l, const R& r ) : _l(l), _r(r) {}
    ~MyPair() {}

    bool operator<( const MyPair< L, R >& rhs ) const
    {
        return ( _r < rhs._r );
    }

    bool operator>( const MyPair< L, R >& rhs ) const
    {
        return ( _r > rhs._r );
    }
};

void freeArrays( int** arrs, int* arr, size_t* M, size_t* cnt, const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
    {
        delete[] arrs[i];
    }
    delete[] arrs;
    delete[] arr;
    delete[] M;
    delete[] cnt;
}

template <typename T>
void printArray( T* arr, const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
        std::cout << arr[i] << ' ';
    std::cout << std::endl;
}

template <typename T>
void printArrays( T** arrs, size_t* M, const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
    {
        printArray<T>( arrs[i], M[i] );
    }
}

size_t getSize( size_t* M, const size_t& N )
{
    size_t size = 0;
    for( size_t i = 0; i < N; ++i)
        size += M[i];
    return size;
}

void pushFirstCnt( int** arrs, size_t* cnt, MyHeap< MyPair< size_t, int > >& heap,
                   const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
    {
        heap.push( MyPair< size_t, int >( i, arrs[i][0] ) );
        ++cnt[i];
    }
}

void revolverNew( int** arrs, size_t* M, size_t* cnt, int* arr,
                  MyHeap< MyPair< size_t, int > >& heap, const size_t& N, const size_t& SIZE )
{
    MyPair< size_t, int > mypair;
    for( size_t i = 0, index = 0; i < SIZE; ++i )
    {
        mypair = heap.pop();  // O(K), Прокручиваем значния
        index = mypair._l;
        arr[i] = mypair._r;
        if( cnt[index] < M[index] )
        {
            heap.push( MyPair< size_t, int >( index, arrs[index][cnt[index]] ) );
            ++cnt[index];
        }
    }

}

//// Тест
#ifdef MAKETEST
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
bool tester( MyHeap<T>& mh, const size_t& size, bool ( *cmp )( const T &l, const T &r ) = defaultLess )
{
    T *arr_test = new T[size];
    T *arr_heap = new T[size];
    randomGenerator<T>( &arr_test[0], size, -10, 10);
    fillHeapFromArr<T>( mh, &arr_test[0], size );
    fillArrFromHeap<T>( mh, &arr_heap[0], size );
    mySort<T>( &arr_test[0], 0, ( size - 1 ) );
    return arrComparator<T>( &arr_test[0], &arr_heap[0], size, cmp );
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

template <typename T = int>  // Взял с семинара
bool defaultMore( const T &l, const T &r )
{
    return l > r;
}

void test10()
{
    MyHeap mh( defaultMore );
    assert( tester( mh, 1000, defaultMore ) );
}

bool arrEqual( int* l, int* r, const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
    {
        if( l[i] != r[i] )
            return false;
    }
    return true;
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
    test10();
#else
    // Считываем
    size_t N = 0;
    std::cin >> N;
    int** arrs = new int*[N];
    size_t* M = new size_t[N];
    for( size_t i = 0; i < N; ++i )
    {
        std::cin >> M[i];
        arrs[i] = new int[M[i]];
        for( size_t j = 0; j < M[i]; ++j )
            std::cin >> arrs[i][j];
    }

    // Заполнеям кучу O(K)
    MyHeap< MyPair< size_t, int > > heap;
    size_t* cnt = new size_t[N];
    pushFirstCnt( arrs, cnt, heap, N );

    // Вычисляем общий размер
    const size_t SIZE = getSize( M, N );
    int* arr = new int[SIZE];
    revolverNew( arrs, M, cnt, arr, heap, N, SIZE );
    printArray<int>( arr, SIZE );

    // Очистка памяти
    freeArrays( arrs, arr, M, cnt, N );
#endif
    return 0;
}


